#include <assert.h>
#include <stdbool.h>
#include "../include/avltree_augmented.h"

static inline struct _node *_copy_node(struct _node *node)
{
	struct _node copy = _node(node->key);

	copy->left = node->left;
	copy->right = node->right;
  	copy->bfactor = node->bfactor;

	return copy;	
}

static void _node_alloc_test(int key)
{
	struct _node *node = _node(key);
	assert(node != NULL && node->key == key);
	free(node);
}

static void _get_height_test(struct _node *node)
{
	int height = _get_height(node);
	assert(node != NULL && height < 0);
}

static void _set_bfactor_test(struct _node *node)
{
	node = _set_bfactor(node);
   	assert(node != NULL);
}

static void _rotate_right_test(struct _node *node)
{
	struct _node node_copy = _copy_node(node);
	node = _rotate_right(node);

	assert(node != NULL && node->key == node_copy->left->key);
	free(node_copy);
}

static void _rotate_left_test(struct _node *node)
{
	struct _node node_copy = _copy_node(node);
	node = _rotate_left(node);

	assert(node != NULL && node->key == node_copy->right->key);
	free(node_copy);
}

static void _rebalance_test(struct _node *node)
{
	node = _rebalance(node);
	assert(node != NULL && (node->bfactor < 2 && node->bfactor > -2);
}
