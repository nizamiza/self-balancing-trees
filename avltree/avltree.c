#include <stdlib.h>
#include "../treeio.h"

#define __max(a, b) ((a) > (b) ? (a) : (b)) 

struct _node {
	struct _node *left;
	struct _node *right;
	char bfactor;
	int key;
};

static inline struct _node *_node(int key)
{
	struct _node *new = (struct _node *) malloc(sizeof(struct _node));

	if (!new)
		return NULL;

	*new = (struct _node) { NULL, NULL, 0, key };
	return new;
}

static inline int _get_height(const struct _node* node)
{
	if (!node)
		return -1;
		
	return __max(_get_height(node->left), _get_height(node->right)) + 1;
}

static inline struct _node *set_bfactor(struct _node *node)
{
	node->bfactor = _get_height(node->left) - _get_height(node->right);
	return node;
}

static inline struct _node *_rotate_right(struct _node *node)
{
	struct _node *left = node->left;

	node->left = left->right;
	left->right = node;

	set_bfactor(node);
	return set_bfactor(left);
}

static inline struct _node *_rotate_left(struct _node *node)
{
	struct _node *right = node->right;

	node->right = right->left;
	right->left = node;

	set_bfactor(node);
	return set_bfactor(right);
}

static inline struct _node *_rebalance(struct _node *node, int key)
{
	if (!node)
		return NULL;

	if (node->left && node->bfactor > 1) {

		if (key < node->left->key)
			return _rotate_right(node);

		node->left = _rotate_left(node->left);
		return _rotate_right(node);
	}

	if (node->right && node->bfactor < -1) {

		if (key > node->right->key)
			return _rotate_left(node);

		node->right = _rotate_right(node->right);
		return _rotate_left(node);
	}

	return node;
}

struct _node *search(struct _node *node, int key)
{
	if (!node || key == node->key)
		return node;

	if (key < node->key)
		return search(node->left, key);

	return search(node->right, key);
}

struct _node *insert(struct _node *node, int key)
{
	if (!node)
		return _node(key);

	if (key < node->key) {
		node->left = insert(node->left, key);
	} else if (key > node->key) {
		node->right = insert(node->right, key);
	} else {
		throw(EDUPNODE);
	}

	return _rebalance(set_bfactor(node), key);
}

static inline const char *_bfactor_to_str(const struct _node *node)
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
	msg(CLR_CYAN "%d\n" CLR_RESET, node->key);
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

	msg(CLR_CYAN "%d\n " CLR_RESET, node->key);

	if (node->left) {
		putws(indent + 1);
		msg(CLR_YELLOW " \\\n" CLR_RESET);
		print(node->left, indent + 4);
	}
}