/**
 * Implementation of 2-3 tree.
 * Nizomiddin Toshpulatov <tremul27@gmail.com>
 * 2020, FIIT STU, Bratislava, Slovakia
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ORDER 3
#define INDENT_INC 8

#define __make_array(a, b, c) ((typeof(a)) { (a), (b), (c) })

#define __sort_keys(a, b, c) (										\
	((a) <= (b) && (b) <= (c)) 	? __make_array(a, b, c) :			\
	((a) <= (c) && (c) < (b)) 	? __make_array(a, c, b) : 			\
	((b) <= (a) && (a) < (c)) 	? __make_array(b, a, c) : 			\
	((b) <= (c) && (c) < (a)) 	? __make_array(b, c, a) : 			\
	((c) <= (a)) ? __make_array(c, a, b) : __make_array(c, b, a)	\
)

#define __destruct_merge_args__(args)			\
	struct _node *node 		= (args).node;		\
	struct _node *left 		= (args).left;		\
	struct _node *middle 	= (args).middle;	\
	int prom_key 			= (args).prom_key;	\

struct _node {
	bool isfull;
	int low_key;
	int high_key;
	struct _node *left, *middle, *right;
	struct _node *parent;
};

struct _merge_args {
	struct _node *node, *left, *middle;
	int prom_key;
};

static inline struct _node *_node(struct _node init)
{
	struct _node *node = (struct _node *) malloc(sizeof(struct _node));

	if (!node)
		return NULL;

	return (*node = init, node);
}

static inline struct _node *_merge_with_parent(struct _merge_args args)
{
	__destruct_merge_args__(args);

	if (!node->parent) {
		
		left->parent = middle->parent = node;

		return _node((struct _node) {
			.isfull 	= false,
			.low_key 	= prom_key,
			.left 		= left,
			.middle 	= middle
		});
	}

	if (!node->parent->isfull) {

		if (node->parent->low_key < prom_key) {

			node->parent->high_key = prom_key;
			node->parent->middle = left;
			node->parent->right = middle;

		} else {

			node->parent->high_key = node->parent->low_key;
			node->parent->low_key = prom_key;

			node->parent->left = left;
			node->parent->right = node->parent->middle;
			node->parent->middle = middle;
		}

		return (node->parent->isfull = true, node->parent);
	}

	int merge_prom_key;
	struct _node *parent, *parent_left, *parent_middle;

	if (prom_key <= node->parent->low_key) {

		parent_left = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= prom_key,
			.left 		= left,
			.middle 	= middle,
			.parent 	= node->parent->parent
		});

		parent_middle = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= node->parent->high_key,
			.left 		= node->parent->middle,
			.middle 	= node->parent->right,
			.parent 	= node->parent->parent
		});

		merge_prom_key = node->parent->low_key;

	} else if (prom_key >= node->parent->high_key) {

		parent_left = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= node->parent->low_key,
			.left 		= node->parent->left,
			.middle 	= node->parent->middle,
			.parent 	= node->parent->parent
		});

		parent_middle = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= prom_key,
			.left 		= left,
			.middle 	= middle,
			.parent 	= node->parent->parent
		});

		merge_prom_key = node->parent->high_key;

	} else {

		parent_left = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= node->parent->low_key,
			.left 		= node->parent->left,
			.middle 	= left,
			.parent 	= node->parent->parent
		});

		parent_middle = _node((struct _node) {
			.isfull 	= false,
			.low_key 	= node->parent->high_key,
			.left 		= middle,
			.middle 	= node->parent->right,
			.parent 	= node->parent->parent
		});

		merge_prom_key = prom_key;
	}

	return _merge_with_parent((struct _merge_args) {
		.node 		= node->parent,
		.left 		= parent_left,
		.middle 	= parent_middle,
		.prom_key 	= merge_prom_key
	});
}

static inline struct _node *_split(struct _node *node, int key)
{
	int keys[] = __sort_keys(node->low_key, node->high_key, key);
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

	return _merge_with_parent((struct _merge_args) {
		.node 		= node,
		.left 		= left,
		.middle 	= middle,
		.prom_key 	= keys[1]
	});
}

static inline struct _node *_insert_key(struct _node *node, int key)
{
	if (node->isfull)
		return _split(node, key);

	if (node->low_key < key) {
		node->high_key = key;
	} else {
		node->high_key = node->low_key;
		node->low_key = key;
	}
	
	return (node->isfull = true, node);
}

static inline struct _node *_merge(struct _node *node, int key)
{
	if (!node->parent)
		return node;
}

struct _node *search(struct _node *node, int key)
{
	if (!node)
		return NULL;

	if (key == node->low_key || key == node->high_key)
		return node;

	if (key <= node->low_key)
		return search(node->left, key);

	if (key >= node->high_key)
		return search(node->right, key);

	return search(node->middle, key);
}

struct _node *insert(struct _node *node, int key)
{
	if (!node) {
		return _node((struct _node) { 
			.isfull = false,
			.low_key = key 
		});
	}

	if (!node->left)
		return _insert_key(node, key);

	if (key <= node->low_key)
		return insert(node->left, key);

	if (key > node->low_key && key < node->high_key)
		return insert(node->middle, key);

	return insert(node->right, key);
}












// void print_node(const struct _node *node)
// {
// 	if (!node)
// 		return;

// 	for (int i = 0; i < node->keys_count; i++)
// 		printf("[%d] ", node->keys[i]);
// 	printf("\n");
// }

// static inline int dcount(int n)
// {
// 	int count = 0;

// 	while (n / 10)
// 		count++;

// 	return count;
// }

// void print(const struct _node *node, int indent)
// {
// 	if (!node)
// 		return;

// 	int i, indent_shift = 0;

// 	for (i = 0; i < indent; i++)
// 		putchar(' ');

// 	printf("|--[");

// 	for (i = 0; i < node->keys_count; i++) {

// 		printf("%d", node->keys[i]);
		
// 		if (i < node->keys_count - 1)
// 			putchar(' ');

// 		indent_shift += dcount(node->keys[i]);
// 	}

// 	indent_shift += node->keys_count - 1;
// 	printf("]--|\n");

// 	for (i = 0; i < node->children_count; i++)
// 		print(node->children[i], indent + indent_shift + INDENT_INC);
// }