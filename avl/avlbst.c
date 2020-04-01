#include <stdlib.h>
#include <stdbool.h>
#include "../treeio.h"

#define putws(n) {		\
	typeof(n) _n = n;	\
	while (_n-- > 0)	\
		putchar(' ');	\
}

struct _node {
	struct _node *left;
	struct _node *right;
	char bfactor;
	long key;
};

static inline long max(long a, long b)
{
	return a > b ? a : b;
}

static inline struct _node *new_node(long key)
{
	struct _node *new = (struct _node *) malloc(sizeof(struct _node));

	if (!new)
		return NULL;

	*new = (struct _node) { NULL, NULL, 0, key };
	return new;
}

static inline struct _node *set_bfactor(struct _node *node)
{
	node->bfactor = height(node->left) - height(node->right);
	return node;
}

static inline struct _node *rotate_right(struct _node *node)
{
	struct _node *left = node->left;

	node->left = left->right;
	left->right = node;

	set_bfactor(node);
	return set_bfactor(left);
}

static inline struct _node *rotate_left(struct _node *node)
{
	struct _node *right = node->right;

	node->right = right->left;
	right->left = node;

	set_bfactor(node);
	return set_bfactor(right);
}

static inline struct _node *rebalance(struct _node *node, long key)
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

long height(const struct _node* node)
{
	if (!node)
		return -1L;
		
	return max(height(node->left), height(node->right)) + 1L;
}

struct _node *search(struct _node *node, long key)
{
	if (!node || key == node->key)
		return node;

	if (key < node->key)
		return search(node->left, key);

	return search(node->right, key);
}

struct _node *insert(struct _node *node, long key)
{
	if (!node)
		return new_node(key);

	if (key < node->key) {
		node->left = insert(node->left, key);
	} else {
		node->right = insert(node->right, key);
	}

	return rebalance(set_bfactor(node), key);
}

static inline const char *bfactor_to_str(const struct _node *node)
{
	return (char []) {
		node->bfactor < 0 ? '-' : '+',
		'0' + abs(node->bfactor)
	};
}

/**
 * Printing function source: https://stackoverflow.com/a/26699993
 */

void print_node(const struct _node *node)
{
	msg(CLR_CYAN "%ld\n" CLR_RESET, node->key);
}

void print(const struct _node *node, int indent)
{
	if (!node)
		return;

	if (node->right)
		print(node->right, indent + 4);

	if (indent)
		putws(indent + 1);

	if (node->right) {
		msg(CLR_YELLOW " /\n" CLR_RESET);
		putws(indent + 1);
	}

	msg(CLR_CYAN "%ld\n " CLR_RESET, node->key);

	if (node->left) {
		putws(indent + 1);
		msg(CLR_YELLOW " \\\n" CLR_RESET);
		print(node->left, indent + 4);
	}
}