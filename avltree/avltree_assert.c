#include <assert.h>
#include <stdbool.h>
#include "../include/avltree_augmented.h"

static inline struct _node *_copy_node(struct _node *node)
{
	if (!node)
		return NULL;
		
	struct _node *copy = _node(node->key);

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
	struct _node *node_copy = _copy_node(node);
	node = _rotate_right(node);

	assert(node != NULL && node->key == node_copy->left->key);
	free(node_copy);
}

static void _rotate_left_test(struct _node *node)
{
	struct _node *node_copy = _copy_node(node);
	node = _rotate_left(node);

	assert(node != NULL && node->key == node_copy->right->key);
	free(node_copy);
}

static void _rebalance_test(struct _node *node, int key)
{
	node = _rebalance(node, key);
	assert(node != NULL && (node->bfactor < 2 && node->bfactor > -2));
}

static void _search_test(struct _node *node, int key)
{
	struct _node *result = search(node, key);
	
	if (node == NULL) {
		assert(result == NULL);
	} else {
		assert(result != NULL && result->key == key);
		free(result);
	}
}

static void _insert_test(struct _node *node, int key)
{
	struct _node *node_copy = _copy_node(node);
	struct _node *search_res = NULL;

	node = insert(node, key);
	assert(node != NULL);

	if (node_copy == NULL) {
		assert(node->key == key);
		return;
	}

	search_res = search(node, key);
	assert(search_res != NULL && search_res->key == key);
	
	free(node_copy);
	free(search_res);
}

void run_internal_tests()
{
	_node_alloc_test(-15);
	_node_alloc_test(2);
	_node_alloc_test(2153);
	_node_alloc_test(21);
	_node_alloc_test(5);
	_node_alloc_test(13729);
	_node_alloc_test(675);
	_search_test(NULL, 15);
	_insert_test(NULL, 89);
}
