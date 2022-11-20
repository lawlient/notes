# how to make install software with apt


## Backgroup

All of those are starting from an idea : why installing a software in ubuntu is so ease ?

The answer is **launchpad**. 

> Launchpad is an open source suite of tools that help people and teams to work together on software projects.

> Using a Personal Package Archive (PPA), you can distribute software and updates directly to Ubuntu users. Create your source package, upload it and Launchpad will build binaries and then host them in your own apt repository.

The tutorial is going to record the solution to install your software with ppa.

## Steps


### 1. Setting up an account in [launchpad](https://launchpad.net/). 

It needs a name and an email address you are using.

### 2. Activate your ppa. 

Create a ppa in your profile page if you don't have one. All you have to do is enter a name and a title for the archive and an optional description, then click the `Activate` button.

### 3. Upload your openGPG keys for verify your archives. 

- 3.1 Generate a gpg key with command like this:
```
gpg --full-generate-key

gpg --list-keys # to list your public key
```
You have to add a **passphrase** for your key, remember it please.

- 3.2 Upload your key to keyserver (to store Public key all over the world), execute command:
```
gpg --keyserver https://keyserver.ubuntu.com --send-keys [your key id] 
```
- 3.3 Import gpg key into your launchpad account.
```
gpg --fingerprint
```
Copy the fingerprint into website and click `Import key`, after this an encrypted email will be send to you.

At this step, you may wait more than ten minutes before you keep going on.

- 3.4 Confirm your gpg key.

You have to decrypt the content in confirm email with your private gpg key. 

Cause of your email reader is likely to unsupport decrypt.

You have to copy the text between ***-----BEGIN PGP MESSAGE-----*** and ***-----END PGP MESSAGE-----*** into a file as tmp.txt.

```
gpg --decrypt tmp.txt
```

you will see the confirm instructions. Follow it to finish adding the key to your Launchpad account.

At here, everything you need is prepared.

### 4. Archive your software.

4.1 Source code, like a hello world process with c.
```bash
mkdir hello_world && cd hello_world

touch hello_world.c
```

and then copy next c code into `hello_world.c` file.

```c
#include <stdio.h>

int main() {
    printf("Hello world!\n");
}
```

4.2 Makefile for your project as follows:

Create a makefile as follow:
```
BINDIR := /usr/bin

all:
    gcc hello_world.c -o helloworld

install:
    mkdir -p ${DESTDIR}${BINDIR}
    cp helloworld ${DESTDIR}${BINDIR}/
```

4.3 Software metadata and control files based on your source code.
```bash
dh_make -p helloworld_0.0.0.1 --single --native --copyright mit --email $youremail

rm debian/*.{ex,EX}
```

After commands executing you will get a directory named debian.

You have to update your changelog and control file in ***debian*** folder.

4.4 Build your deb.

```
debuild -S -k$(your key id) | tee /tmp/debuild.log 2>&1
```

4.5 Upload deb to your ppa.

```
dput ppa:$yourname/$ppaname helloworld_0.0.0.1_source.changes
```

Wait a moment, you will receive an email for the result of your uploading.



### 5. Installing software with ppa.

```
sudo apt-add-repository ppa:$yourname/$ppaname

sudo apt update
```

May be you have to wait a long time, after you just upload your deb.



## Reference

- [Building a Debian (`.deb`) source package, and publishing it on an Ubuntu PPA](https://saveriomiroddi.github.io/Building-a-debian-deb-source-package-and-publishing-it-on-an-ubuntu-ppa/#preparing-the-source-package)

- [Personal Package Archives](https://help.launchpad.net/Packaging/PPA)

- [Debian 新维护者手册](https://www.debian.org/doc/manuals/maint-guide/)
