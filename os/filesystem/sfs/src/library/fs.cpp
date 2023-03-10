// fs.cpp: File System

#include "sfs/fs.h"

#include <algorithm>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



bool FileSystem::SuperBlock::sanity_check(uint32_t blocks) const {
    if (MagicNumber != MAGIC_NUMBER) return false;
    if (Blocks != blocks) return false;
    if (InodeBlocks != ceil(0.1 * blocks)) return false;
    if (Inodes != InodeBlocks * INODES_PER_BLOCK) return false;
    return true;
};


// Debug file system -----------------------------------------------------------


void FileSystem::debug(Disk *disk) {
    Block block;

    // Read Superblock
    disk->read(0, block.Data);

    printf("SuperBlock:\n");
if (block.Super.MagicNumber == MAGIC_NUMBER) {
    printf("    magic number is valid\n");
}
    printf("    %u blocks\n"         , block.Super.Blocks);
    printf("    %u inode blocks\n"   , block.Super.InodeBlocks);
    printf("    %u inodes\n"         , block.Super.Inodes);

    // Read Inode blocks
    uint32_t inoden = block.Super.InodeBlocks;
    for (uint32_t i = 0; i < inoden; i++) {
        disk->read(i+1, block.Data);
        for (uint32_t j = 0; j < INODES_PER_BLOCK; j++) {
            const auto& ino = block.Inodes[j];
            if (ino.Valid) {
                printf("Inode %d:\n", i * INODES_PER_BLOCK + j);
                printf("    size: %d bytes\n", ino.Size);
                printf("    direct blocks:");
                for (uint32_t k = 0; k < POINTERS_PER_INODE; k++) {
                    uint32_t d = ino.Direct[k];
                    if (d == 0) break;
                    printf(" %d", d);
                }
                printf("\n");
                if (ino.Indirect) {
                    Block indirect_block;
                    disk->read(ino.Indirect, indirect_block.Data);
                    printf("    indirect block: %d\n", ino.Indirect);
                    printf("    indirect data blocks:");
                    for (uint32_t ii = 0; ii < POINTERS_PER_BLOCK; ii++) {
                        if (0 == indirect_block.Pointers[ii]) break;
                        printf(" %d", indirect_block.Pointers[ii]);
                    }
                    printf("\n");
                }
            }
        }
    }
}

// Format file system ----------------------------------------------------------

bool FileSystem::format(Disk *disk) {
    if (disk->mounted()) {
        return false;
    }
    Block block;
    block.Super.MagicNumber = MAGIC_NUMBER;
    block.Super.Blocks = disk->size();
    block.Super.InodeBlocks = ceil(0.1 * disk->size());
    block.Super.Inodes = block.Super.InodeBlocks * INODES_PER_BLOCK;
    disk->write(0, (char *)&block);

    // Clear all other blocks
    memset(block.Data, 0, sizeof(Block));
    for (size_t i = 1; i < disk->size(); i++) {
        disk->write(i, block.Data);
    }
    return true;
}

// Mount file system -----------------------------------------------------------

bool FileSystem::mount(Disk *disk) {
    if (super.MagicNumber == MAGIC_NUMBER) {
        // Already mounted
        return false;
    }

    // Read superblock
    Block b;
    disk->read(0, b.Data);
    // Check whether disk is broken or not
    if (!b.Super.sanity_check(disk->size())) {
        return false;
    }

    // Set device and mount
    disk->mount();

    // Copy metadata
    super = b.Super;
    this->disk  = disk;

    // Allocate free block bitmap
    uint32_t inoden = b.Super.InodeBlocks;
    free_inode.reserve(inoden);
    for (uint32_t i = 0; i < inoden; i++) {
        disk->read(i+1, b.Data);
        for (uint32_t j = 0; j < INODES_PER_BLOCK; j++) {
            if (b.Inodes[j].Valid && b.Inodes[j].Size) {
                free_inode[i].data[j/8] |= 1 << (j%8);
            }
        }
    }

    /// printf("==========\n");
    /// for (uint32_t i = 0; i < inoden; i++){
    ///     for (int j = 0; j < 16; j++)
    ///         printf("%d\t", free_inode[i].data[j]);
    ///     printf("\n");
    /// }

    return true;
}

// Create inode ----------------------------------------------------------------

ssize_t FileSystem::create() {
    // Locate free inode in inode table
    ssize_t inumber = -1;
    uint32_t i, j, k;
    for (i = 0; i < super.InodeBlocks; i++) {
        for (j = 0; j < INODES_PER_BLOCK; j = j+8) {
            auto slice = free_inode[i].data[j/8];
            for (k = 0; k < 8; k++) if (!(slice & (1 << k))) break;
            if (k == 8) continue;

            inumber = i * INODES_PER_BLOCK + j + k;
            break;
        }
        if (-1 != inumber) break;
    }

    // Record inode if found
    free_inode[i].data[j/8] |= 1 << k;
    Block b;
    try {
        if (-1 != inumber) {
            free_inode[i].data[j/8] |= 1 << k;
            disk->read(1+i, b.Data);
            memset(b.Inodes + j + k, 0, sizeof(Inode));
            b.Inodes[j+k].Valid = 1;

            disk->write(1+i, b.Data);
        }
    } catch (std::exception& e) {
        printf("%s\n", e.what());
    }
    return inumber;
}

// Remove inode ----------------------------------------------------------------

bool FileSystem::remove(size_t inumber) {
    // Load inode information
    uint32_t i, j;
    i = inumber / INODES_PER_BLOCK;
    j = inumber % INODES_PER_BLOCK;
    if (!(free_inode[i].data[j/8] & (1 << (j%8)))) {
        return false;
    }
    Block b;
    disk->read(1+i, b.Data);
    auto& ino = b.Inodes[j];
    free_inode[i].data[j/8] &= ~(1 << (j%8));

    // Free direct blocks
    memset(ino.Direct, 0, sizeof(ino.Direct));

    // Free indirect blocks
    ino.Indirect = 0;

    // Clear inode in inode table
    ino.Size = 0;
    ino.Valid = 0;
    disk->write(1+i, b.Data);
    return true;
}

// Inode stat ------------------------------------------------------------------

ssize_t FileSystem::stat(size_t inumber) {
    // Load inode information
    uint32_t i, j;
    i = inumber / INODES_PER_BLOCK;
    j = inumber % INODES_PER_BLOCK;
    Block b;
    disk->read(1+i, b.Data);
    auto& ino = b.Inodes[j];
    if (!ino.Valid) {
        return -1;
    }
    return ino.Size;
}

// Read from inode -------------------------------------------------------------

ssize_t FileSystem::read(size_t inumber, char *data, size_t length, size_t offset) {
    // Load inode information
    uint32_t i, j;
    i = inumber / INODES_PER_BLOCK;
    j = inumber % INODES_PER_BLOCK;
    Block b;
    disk->read(1+i, b.Data);
    auto& ino = b.Inodes[j];
    if (!ino.Valid) {
        return -1;
    }
    if (offset >= ino.Size) {
        return -1;
    } 

    // Adjust length
    uint32_t db_idx = 0;
    if (offset < Disk::BLOCK_SIZE * 5) {
        db_idx = ino.Direct[offset / Disk::BLOCK_SIZE];
    } else {
        Block ind;
        disk->read(ino.Indirect, ind.Data);
        db_idx = ind.Pointers[(offset - Disk::BLOCK_SIZE * 5) / Disk::BLOCK_SIZE];
    }

    // Read block and copy to data
    ssize_t size = 0;
    disk->read(db_idx, data);
    data[Disk::BLOCK_SIZE] = '\0';
    size = strlen(data);
    data[size] = '\0';
    return size;
}

// Write to inode --------------------------------------------------------------

ssize_t FileSystem::write(size_t inumber, char *data, size_t length, size_t offset) {
    // Load inode
    
    // Write block and copy to data
    return 0;
}
