/**
 * Implementation of 2-3 tree.
 * Nizomiddin Toshpulatov <tremul27@gmail.com>
 * 2020, FIIT STU, Bratislava, Slovakia
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../treeio.h"

#define ORDER 3
#define INDENT_INC 4

#define __indent(current) ((current) + INDENT_INC)

#define __ASSIGN_SORTED_KEYS(a, b, c) (arr[0] = a, arr[1] = b, arr[2] = c)

#define __DESTRUCT_MERGE_ARGS__					\
	struct _node *node		= args.node;		\
	struct _node *left		= args.left;		\
	struct _node *middle	= args.middle;		\
	int prom_key			= args.prom_key;	\

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

	if (!node) {
		throw(ENOMEM);
		return NULL;
	}

	if (init.left)
		init.left->parent = node;

	if (init.middle)
		init.middle->parent = node;

	if (init.right)
		init.right->parent = node;

	return (*node = init, node);
}

static inline struct _node *_update_node(struct _node *node, struct _node update)
{
	return (*node = update, node);
}

static inline int *_sort_keys(int a, int b, int c)
{
	int *arr = (int *) calloc(ORDER, sizeof(int));

	if (a < b && b < c)
		return (__ASSIGN_SORTED_KEYS(a, b, c), arr);

	if (a < c && c < b)
		return (__ASSIGN_SORTED_KEYS(a, c, b), arr);

	if (b < a && a < c)
		return (__ASSIGN_SORTED_KEYS(b, a, c), arr);

	if (b < c && c < a)
		return (__ASSIGN_SORTED_KEYS(b, c, a), arr);

	if (c < a)
		return (__ASSIGN_SORTED_KEYS(c, a, b), arr);

	return (__ASSIGN_SORTED_KEYS(c, b, a), arr);
}

static struct _node *_merge_with_parent(struct _merge_args args)
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

		parent->isfull = true;
		free(node);

		return parent;
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

static inline struct _node *_split(struct _node *node, int key)
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

static inline struct _node *_add_key(struct _node *node, int key)
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

static inline struct _node *_get_root(struct _node *node)
{
	if (!node->parent)
		return node;

	struct _node *current = node->parent;

	while (current->parent)
		current = current->parent;
	return current;
}

static inline struct _node *_insert_key(struct _node *node, int key)
{
	if (node->isfull) {
		node = _split(node, key);
	} else {
		node = _add_key(node, key);
	}
		
	return _get_root(node);
}

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

	if (key < node->low_key)
		return insert(node->left, key);

	if (node->right && key > node->high_key)
		return insert(node->right, key);

	if (key == node->low_key || key == node->high_key) {
		throw(EDUPNODE);
		return _get_root(node);
	}

	return insert(node->middle, key);
}

/* Printing logic */
void print_node(const struct _node *node)
{
	if (!node) {
		msg(ENONODE);
		return;
	}

	if (node->isfull) {
		msg("[%d %d]\n", node->low_key, node->high_key);
	} else {
		msg("[%d]\n", node->low_key);
	}
}

void print(const struct _node *node, int indent)
{
	if (!node)
		return;

	for (int i = 0; i < indent; i++) {

		if ((i + 1) % INDENT_INC) {
			putchar(' ');
		} else {
			msg(CLR_MAGENTA "|" CLR_RESET);
		}
	}

	msg(CLR_MAGENTA "-" CLR_RESET);

	if (node->isfull) {
		msg(CLR_YELLOW "[%d %d]\n" CLR_RESET, node->low_key, node->high_key);
	} else {
		msg(CLR_YELLOW "[%d]\n" CLR_YELLOW, node->low_key);
	}

	if (!node->left)
		return;

	print(node->left, __indent(indent));

	if (node->right) {
		print(node->middle, __indent(indent));
		print(node->right, __indent(indent));
	} else {
		print(node->middle, __indent(indent));
	}
}