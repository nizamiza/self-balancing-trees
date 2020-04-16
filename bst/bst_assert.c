#include <assert.h>
#include "../include/bst_augmented.h"

static struct _node *_insert_test(struct _node *node, int key)
{
	node = insert(node, key);
	assert(node != NULL);
}

static void _clear_test(struct _node *node)
{
	clear(node);
	assert(node == NULL);
}

void run_internal_tests()
{
	struct _node *root = _node(25);

	root = _insert_test(root, 15);
	root = _insert_test(root, -5);
	root = _insert_test(root, 60);

	_clear_test(root);
}
