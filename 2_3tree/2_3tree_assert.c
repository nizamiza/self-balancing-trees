#include <assert.h>
#include "../include/2_3tree_augmented.h"

#define INTERNAL_TESTING
#define AUTO_CLEANUP

#ifdef AUTO_CLEANUP

#define __END_NODE_TEST__ 	\
	free(node);				\
	return NULL;			\

#else

#define __END_NODE_TEST__	\
	return node;			\

#endif

static inline bool _nodes_equal(struct _node node_a, struct _node node_b)
{
	return (
		node_a.low_key 	== node_b.low_key	&&
		node_a.high_key == node_b.high_key 	&&
		node_a.isfull 	== node_b.isfull 	&&
		node_a.left 	== node_b.left 		&&
		node_a.middle 	== node_b.middle 	&&
		node_a.right 	== node_b.right 	&&
		node_a.parent 	== node_b.parent
	);
}

static inline int _compare_keys(const void *a, const void *b)
{
	return *(int *) a - *(int *) b;
}

static struct _node *_node_alloc_test(struct _node init)
{
	struct _node *node = _node(init);
	assert(node != NULL && _nodes_equal(*node, init));

	__END_NODE_TEST__;
}

static struct _node *_update_node_test(struct _node *node, struct _node update)
{
	node = _update_node(node, update);
	assert(node != NULL && _nodes_equal(*node, update));

	__END_NODE_TEST__;
}

static void _sort_keys_test(int a, int b, int c)
{
	assert(a != b && a != c && b != c);

	int *arr = (int *) calloc(3, sizeof(int));
	int *arr_qsort = (int *) calloc(3, sizeof(int));

	arr_qsort[0] = a, arr_qsort[1] = b, arr_qsort[2] = c;

	arr = _sort_keys(a, b, c);
	qsort(arr_qsort, 3, sizeof(int), _compare_keys);

	for (int i = 0; i < 3; i++)
		assert(arr[i] == arr_qsort[i]);
}

static void _merge_with_parent_test(struct _merge_args args)
{
	__DESTRUCT_MERGE_ARGS__;

	struct _node *node_copy = _node(*node);
	struct _node *parent_copy = _node(*node->parent);
	struct _node *merged_node = _merge_with_parent(args);

	assert(merged_node != NULL);

	if (!node_copy->parent) {
		assert(_nodes_equal(*merged_node, (struct _node) {
			.isfull 	= false,
			.low_key 	= prom_key,
			.left 		= left,
			.middle 	= middle
		}));
	}

	if (!parent_copy->isfull) {
		if (parent_copy->low_key < prom_key) {
			assert(_nodes_equal(*merged_node, (struct _node) {
				.isfull 	= true,
				.low_key 	= parent_copy->low_key,
				.high_key 	= prom_key,
				.left 		= parent_copy->left,
				.middle 	= left,
				.right 		= middle
			}));
		} else {
			assert(_nodes_equal(*merged_node, (struct _node) {
				.isfull 	= true,
				.low_key 	= prom_key,
				.high_key 	= parent_copy->low_key,
				.left 		= left,
				.middle 	= middle,
				.right 		= parent_copy->middle
			}));
		}
	}

	free(node_copy);
	free(parent_copy);
	free(merged_node);
}

static struct _node *_split_test(struct _node *node, int key)
{
	struct _node *node_copy = _node(*node);
	node = _split(node, key);

	assert(node != NULL);

	if (key == node_copy->low_key || key == node_copy->high_key)
	assert(_nodes_equal(*node, *node_copy));

	free(node_copy);
	__END_NODE_TEST__;
}

static struct _node *_add_key_test(struct _node *node, int key)
{
	assert(!node->isfull);
	struct _node *node_copy = _node(*node);
	
	node = _add_key(node, key);
	assert(node != NULL && node->isfull);

	if (key == node->low_key) {
		assert(_nodes_equal(*node, *node_copy));
	} else if (node_copy->low_key < key) {
		assert(node->high_key == key);
	} else {
		assert(node->high_key == node_copy->low_key && node->low_key == key);
	}

	free(node_copy);
	__END_NODE_TEST__;
}

static struct _node *_search_test(struct _node *node, int key)
{
	struct _node *result = search(node, key);

	if (!node) {
		assert(result == NULL);
	} else {
		assert(
			result != NULL &&
			(result->low_key == key || result->high_key == key)
		);
	}

	free(result);
	__END_NODE_TEST__;
}

static struct _node *_insert_test(struct _node *node, int key)
{
	struct _node *node_copy = _node(*node);
	struct _node *search_res = NULL;
	
	node = insert(node, key);
	assert(node != NULL);

	if (!node_copy) {
		assert(_nodes_equal(*node, (struct _node) {
			.isfull = false,
			.low_key = key
		}));
	} else {
		search_res = search(node, key);
		assert(
			search_res != NULL &&
			(search_res->low_key == key ||
			search_res->high_key == key)
		);
	}

	free(node_copy);
	free(search_res);

	__END_NODE_TEST__;
}

void run_internal_tests()
{
	_node_alloc_test((struct _node) {
		.isfull = true, .low_key = 25, .high_key = 50
	});

	_node_alloc_test((struct _node) {
		.isfull = false,
		.low_key = 0,
		.high_key = -50,
		.left = (struct _node *) malloc(sizeof(struct _node)),
	});

	_node_alloc_test((struct _node) {
		.isfull = true,
		.low_key = 12,
		.high_key = 7,
		.left = (struct _node *) malloc(sizeof(struct _node)),
		.middle = (struct _node *) malloc(sizeof(struct _node)),
	});

	_node_alloc_test((struct _node) {
		.isfull = true,
		.low_key = 5329,
		.high_key = -4136,
		.left = (struct _node *) malloc(sizeof(struct _node)),
		.middle = (struct _node *) malloc(sizeof(struct _node)),
		.right = (struct _node *) malloc(sizeof(struct _node)),
	});

	_node_alloc_test((struct _node) {
		.isfull = false,
		.low_key = -5682,
		.high_key = 5,
		.left = NULL,
		.middle = NULL,
		.right = (struct _node *) malloc(sizeof(struct _node)),
	});

	_update_node_test(_node((struct _node) {
			.isfull = false,
			.low_key = 15,
			.high_key = 9000,
			.left = NULL,
			.right = (struct _node *) malloc(sizeof(struct _node)),
		}), (struct _node) {
			.isfull = true,
			.low_key = -542682,
			.high_key = 0,
	});

	_sort_keys_test(15, -7, 12);
	_sort_keys_test(1, 90000, 215679);
	_sort_keys_test(-785, -6365, -120000);
	_sort_keys_test(1256, 84139, 637);
	_sort_keys_test(3721, 164739, 102);
	_sort_keys_test(6936, -11, 12);
}
