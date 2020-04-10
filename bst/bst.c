#include <stdlib.h>
#include "../include/treeio.h"

struct _node {
	int key;
	struct _node *left;
	struct _node *right;
};

struct _node *_node(int key)
{
	struct _node *new = (struct _node *) malloc(sizeof(struct _node));

	if (!new)
		return NULL;
	return (new->key = key, new);
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

	return node;
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