- [AVL Tree](#avl-tree)
	- [Introduction](#introduction)
	- [Implementation](#implementation)
- [2-3 tree](#2-3-tree)
	- [Introduction](#introduction-1)
	- [Implementation](#implementation-1)
- [Comparison](#comparison)
- [References](#references)
   

# AVL Tree
## Introduction

AVL tree, where AVL stands for its creators - *Adelson-Velsky and Landis*, is a self balancing binary search tree. It is was the first such tree to be invented.

AVL tree has `O(logn)` time complexity for its operations. It is often compared to *red-black* tree due to them both being height balanced.

Each node in AVL tree has a *balance factor*, which is calculated by substracting the height of the left subtree from the height of the right subtree. I.e.:

`B(N) = H(R(N)) - H(L(N))`,

where `B(N)` is balance factor of the node `N`, `R(N)` is the right subtree of the node `N`, `L(N)` is the left subtree of the node `N`, and `H(T)` is a function to calculate height of the subtree `T`.

Binary search tree is defined as an AVL tree, if and only if, each node of the tree has balance factor of **0**, **1**, or **-1**. Node `N` with balance factor of **-1** is *left heavy*, node `N` with balance factor of **1** is *right heavy*, node `N` with balance factor of **0** is *balanced*.

## Implementation

This implementation includes only *search* and *insert* operations.
For each node we can define a structure:

```c
struct _node {
	struct _node *left;
	struct _node *right;
	char bfactor;
	long key;
};
```

where `bfactor` stands for *balance factor*.

Insertion operation of the AVL tree is similar to one of regular binary search tree. Simple recursive traversal will do the trick. Important bit comes after the fact of insertion, which is the rebalancing of tree.

Each time we insert a new node, we calculate balance factor of that node, and if it is not **-1**, **0**, or **1**, then we need to rebalance our tree. This can be done with node rotation operations, such as *left rotation*, and *right rotation*.

To left rotate node is to set its *right subtree* to *left subtree of its right subtree*, and set itself as the *left subtree* of its *right subtree*. I.e.:

```c
struct _node *rotate_left(struct _node *node)
{
	struct _node *right = node->right;

	node->right = right->left;
	right->left = node;

	set_bfactor(node);
	return set_bfactor(right);
}
```

Right rotation is the same process as the left rotation, but instead of setting *right subtree* of the node, we set its *left subtree* to right subtree of its *left subtree*. And after that, similar to the left rotation, we set the node itself as the *right subtree* of its *left subtree*. I.e.:

```c
struct _node *rotate_right(struct _node *node)
{
	struct _node *left = node->left;

	node->left = left->right;
	left->right = node;

	set_bfactor(node);
	return set_bfactor(left);
}
```

When new node is inserted to AVL tree, there are [4 cases][1]:

1. Left left case.
2. Left Right case.
3. Right Right case.
4. Right Left case.

Based on these cases, we can define rebalancing routine as follows:
```c
struct _node *rebalance(struct _node *node, long key)
{
	if (!node)
		return NULL;

	if (node->left && node->bfactor > 1) {

		if (key < node->left->key)
			return rotate_right(node);

		node->left = rotate_left(node->left);
		return rotate_right(node);
	}

	if (node->right && node->bfactor < -1) {

		if (key > node->right->key)
			return rotate_left(node);

		node->right = rotate_right(node->right);
		return rotate_left(node);
	}

	return node;
}
```

# 2-3 tree
## Introduction

2-3 tree is a self-balancing search tree, where each node (ofter referred as internal node) has either *2 children* and *1 key* or *three children* and *2 keys*.

2-3 trees are always balanced and the logic behind it is quite beautiful.

## Implementation

This implementation includes only *search* and *insert* operations.

# Comparison

# References

* [www.geeksforgeeks.org, AVL Tree | Set 1 (Insertion)][1]

[1]: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/

2020, FIIT STU, Bratislava, Slovakia.
