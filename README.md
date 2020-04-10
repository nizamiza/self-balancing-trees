- [AVL tree](#avl-tree)
	- [Introduction](#introduction)
	- [Implementation](#implementation)
- [2-3 tree](#2-3-tree)
	- [Introduction](#introduction-1)
	- [Implementation](#implementation-1)
- [Red-black tree](#red-black-tree)
	- [Introduction](#introduction-2)
	- [Implementation](#implementation-2)
- [Comparison](#comparison)
- [Testing](#testing)
- [References](#references)
   

# AVL tree
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

where `bfactor` stands for the *balance factor*.

Search routine is the same as for the regular Binary Search Tree, so we will skip it and go right to the insertion.

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

2-3 tree is a self-balancing search tree, where each node (often referred to as the internal node) has either *2 children* and *1 key* or *three children* and *2 keys*.

Leaves of 2-3 tree are always on the level, meaning that tree is always balanced. [Traversal algorithm][2] of 2-3 tree goes as follows:

1. Let `T` be a 2–3 tree and `d` be the data element we want to find. If `T` is empty, then `d` is not in `T` and we're done.
2. Let `r` be the root of `T`.
3. Suppose `r` is a leaf. If `d` is not in `r`, then `d` is not in `T`. Otherwise, `d` is in `T`. In particular, `d` can be found at a leaf node. We need no further steps and we're done.
4. Suppose `r` is a 2-node with left child `L` and right child `R`. Let e be the data element in `r`. There are three cases:
	* If `d` is equal to e, then we've found `d` in `T` and we're done.
	* If `d < e`, then set `T` to `L`, which by definition is a 2–3 tree, and go back to step 2.
	* If `d > e`, then set `T` to `R` and go back to step 2.
5. Suppose `r` is a 3-node with left child `L`, middle child `M`, and right child `R`. Let `a` and `b` be the two data elements of `r`, where `a < b`. There are four cases:
	* If `d` is equal to `a` or `b`, then `d` is in `T` and we're done.
	* If `d < a`, then set `T` to `L` and go back to step 2.
	* If `a < d < b`, then set `T` to `M` and go back to step 2.
	* If `d > b`, then set `T` to `R` and go back to step 2.

To insert a new node to the tree, we search for the correct position for the node with traversal algorithm and insert it there. If the node becomes a 4-node, then we split it into 2 nodes, promoting middle key to the parent node. This process repeats recursively until we reach the 2-node node, or the root, in which case we split the root as well, making a new one out of the middle key. 

## Implementation

This implementation includes only *search* and *insert* operations.

For each internal node we can define a structure:

```c
struct _node {
	bool isfull;
	int low_key;
	int high_key;
	struct _node *left, *middle, *right;
	struct _node *parent;
};
```

where `isfull` indicates whether node is 3-node (*has 2 keys and 3 children*).

Search routine for the 2-3 tree is similar to the regular BST, only difference being that we have an extra condition for the recursive call when the node is a 3-node, and search value is between keys of the node. I.e.:

```c
struct _node *search(struct _node *node, int key)
{
	if (!node)
		return NULL;

	if (key == node->low_key || (node->isfull && key == node->high_key))
		return node;

	if (key < node->low_key)
		return search(node->left, key);

	if (node->right && key > node->high_key)
		return search(node->right, key);

	return search(node->middle, key);
}
```

Insertion is the most complex part of this implementation. Steps are the same as described in the *Introduction*. After traversing the tree and finding a spot for the new node, we insert the key:

```c
struct _node *_insert_key(struct _node *node, int key)
{
	if (node->isfull) {
		node = _split(node, key);
	} else {
		node = _add_key(node, key);
	}
		
	return _get_root(node);
}
```

If node is not full, then we just compare new key with the existing key and insert it to the correct position. If new key is a duplicate, then we throw `EDUPNODE` (error duplicate node) and simply return unchanged node:

```c
struct _node *_add_key(struct _node *node, int key)
{
	if (key == node->low_key) {
		throw(EDUPNODE);
		return node;
	}

	if (node->low_key < key) {
		node->high_key = key;
	} else {
		node->high_key = node->low_key;
		node->low_key = key;
	}

	return (node->isfull = true, node);
}
```

But if node is full, then we are at the step **5** of our insertion algorithm, meaning we have to split our node. To do so, we sort all 3 keys (two keys of the node, and the key that is being inserted) and create **2** new nodes from the smallest and the largest keys. The middle size key gets promoted to the parent: 

```c
struct _node *_split(struct _node *node, int key)
{
	if (key == node->low_key || key == node->high_key) {
		throw(EDUPNODE);
		return node;
	}

	int *keys = _sort_keys(node->low_key, node->high_key, key);
	int prom_key = keys[1];

	struct _node *left, *middle;

	left = _node((struct _node) {
		.isfull 	= false,
		.low_key 	= keys[0],
		.parent 	= node->parent
	});

	middle = _node((struct _node) {
		.isfull 	= false,
		.low_key 	= keys[2],
		.parent 	= node->parent
	});

	free(keys);
	return _merge_with_parent((struct _merge_args) {
		.node 		= node,
		.left 		= left,
		.middle 	= middle,
		.prom_key 	= prom_key
	});
}
```

After that we need to merge our **2** new nodes with the parent node, as well as add the promoted key to its list of keys. This step can be divided into **3** scenarios:

1. Node does not have a parent.
2. Parent node is not full (2-node).
3. Parent node is full (3-node).

In the **first** case, we simply return a new node, which has a single key (promoted key) and two children (newly made 2 nodes).

In the **second** case, we add promoted key to the parent and attach **2** new children alongside his single child.

In the **third** case, we split the parent itself and promote its middle key to its parent.

As expectedly, this process repeats recursively until we reach the first 2-node node, or the root.

```c
struct _node *_merge_with_parent(struct _merge_args args)
{
	__DESTRUCT_MERGE_ARGS__;

	if (!node->parent) {
		left->parent = middle->parent = node;
		return _update_node(node, (struct _node) {
			.isfull 	= false,
			.low_key 	= prom_key,
			.left 		= left,
			.middle 	= middle
		});
	}

	struct _node *parent = node->parent;
	free(node);

	if (!parent->isfull) {
		if (parent->low_key < prom_key) {
			parent->high_key = prom_key;
			parent->middle = left;
			parent->right = middle;
		} else {
			parent->high_key =parent->low_key;
			parent->low_key = prom_key;

			parent->left = left;
			parent->right = parent->middle;
			parent->middle = middle;
		}

		return (parent->isfull = true, parent);
	}

	int merge_prom_key;
	struct _node *parent_left, *parent_middle;

	if (prom_key < parent->low_key) {
		parent_left = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= prom_key,
			.left 		= left,
			.middle 	= middle,
			.parent 	= parent->parent
		});

		parent_middle = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= parent->high_key,
			.left 		= parent->middle,
			.middle 	= parent->right,
			.parent 	= parent->parent
		});

		merge_prom_key = parent->low_key;	
	} else if (prom_key > parent->high_key) {
		parent_left = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= parent->low_key,
			.left 		= parent->left,
			.middle 	= parent->middle,
			.parent 	= parent->parent
		});

		parent_middle = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= prom_key,
			.left 		= left,
			.middle 	= middle,
			.parent 	= parent->parent
		});

		merge_prom_key = parent->high_key;
	} else {
		parent_left = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= parent->low_key,
			.left 		= parent->left,
			.middle 	= left,
			.parent 	= parent->parent
		});

		parent_middle = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= parent->high_key,
			.left 		= middle,
			.middle 	= parent->right,
			.parent 	= parent->parent
		});

		merge_prom_key = prom_key;
	}

	return _merge_with_parent((struct _merge_args) {
		.node 		= parent,
		.left 		= parent_left,
		.middle 	= parent_middle,
		.prom_key 	= merge_prom_key
	});
}
```

# Red-black tree
## Introduction

Red-black tree is a self-balancing binary search tree, where each node is colored either [red or black][3]:

* Root of the red-black tree is always black.
* Each leaf is colored black.
* If node is red, then both of its children are black.
* Every path from a given node to any of its descendant NIL nodes goes through the same number of black nodes.

Like AVL tree, Red-black tree is height balanced. The balancing rule is:

> The path from the root to the farthest leaf is no more than twice as long as the path from the root to the nearest leaf

## Implementation

This implementation was taken from [www.programiz.com][4] and it serves only for comparison purposes. Although, several alterations to the source code had to be made. These were primarily to adapt code for the testing and comparison:

* global variable `root` was removed;
* `insertion` function was renamed to `insert`, and was tweaked to work without global variable;
* `deletion` function was renamed to `delete` and tweaked to work without global variable (*although it's never used*);
* additional `search` function was implemented;
* additional `print_node` function was implemented;
* additional `print` function was implemented.

This implementation defined an internal node as a structure:

```c
struct rbNode {
	int data, color;
	struct rbNode *link[2];
};
```

# Comparison

All three implementations were tested for time efficiency during insertion and search with this set of keys:

```

```

# Testing

Nearly each function of the implementations was tested with unit tests (except for the Red-black tree). Test functions including some test scenarios can be found in the same directory of the source code of the implementation. I.e., tests for the 2-3 tree implementation, are located in `2_3tree/2_3tree_assert.c`.

# References

* [en.wikipedia.org, AVL tree][0]
* [www.geeksforgeeks.org, AVL tree | Set 1 (Insertion)][1]
* [en.wikipedia.org, 2-3 tree][2]
* [en.wikipedia.org, Red-black tree][3]
* [www.programiz.com, Red-black tree][4]

[0]: https://en.wikipedia.org/wiki/AVL_tree
[1]: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
[2]: https://en.wikipedia.org/wiki/2%E2%80%933_tree
[3]: https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
[4]: https://www.programiz.com/dsa/red-black-tree

2020, FIIT STU, Bratislava, Slovakia.
