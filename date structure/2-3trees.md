# 2-3 trees


## Definition

1. there are 2 kinds of nodes, 2-nodes & 3-nodes.

    2-nodes has a value ***v*** & 2 children, l & r.

    3-nodes has 2 values ***x***、***y*** and 3 children，l、m、r.

    and l、m、r keep those rules.

2. keep balance.

    length(l) [ = length(m) ] = length(r)

3. keep order between nodes

    for 2-nodes, all v in left subtree <= v <= all v in right subtree

    for 3-nodes, all v in left subtree <= x <= all v in middle subtree <= y <= all v in right subtree


## Result in

> $length(tree) <= log_2(n + 1)$


## Manipulate

First, downward manipulation will result in length between subtrees are different, it break Rules 2.

So, we have to convert downward to upward.


### Insertion

1. The hole is in a 2-nodes

    It's easy, the 2-nodes absorb the value,  still keep the rules


2. The hole is in a 3-nodes

    - absorb the value first, we get a 4-nodes
    - split the 4-nodes into a tree with a 2-nodes and 2 subtrees

    after this the length of the tree is +1, but it grows upward, the rule 3 still keep


3. any other cases can convert to case 1 and 2



### Deletion


1. Parent is 2-nodes

    1. sibling is 2-nodes

        - merge parent and sibling into a 3-nodes

        length(tree) - 1

    2. sibling is 3-nodes

        - merge parent and sibling into a 4-nodes
        - split 4-nodes into a tree with a 2-nodes and 2 subtrees

        length(tree) is unchanged


2. Parent is 3-nodes


    - downward a parent to the hole & parent changes from 3-nodes to 2-nodes

        length(tree) is unchanged



## Reference

[2-3-trees.pdf](https://www.cs.princeton.edu/~dpw/courses/cos326-12/ass/2-3-trees.pdf)
