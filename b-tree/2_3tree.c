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

struct _node {
	bool is_full;
	int low_key;
	int high_key;
	struct _node *left, *middle, *right;
	struct _node *parent;
};

static inline struct _node *_node(struct _node init)
{
	struct _node *node = (struct _node *) malloc(sizeof(struct _node));

	if (!node)
		return NULL;

	return (*node = init, node);
}

static inline struct _node *_split(struct _node *node, int key)
{
	int keys[] = __sort_keys(node->low_key, node->high_key, key);

	 _node((struct _node) {
		.left = _node((struct _node) { false, keys[0] }),
		.middle = _node((struct _node) { false, keys[2] }),
		.is_full = false,
		.low_key = keys[1],
	});
}

static inline struct _node *_insert_key(struct _node *node, int key)
{
	if (node->is_full)
		return _split(node, key);

	if (node->low_key < key) {
		node->high_key = key;
	} else {
		node->high_key = node->low_key;
		node->low_key = key;
	}
	
	return (node->is_full = true, node);
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

	if (key > node->low_key && key < node->high_key)
		return search(node->middle, key);

	return search(node->right, key);
}

struct _node *insert(struct _node *node, int key)
{
	if (!node)
		return _node((struct _node) { false, key });

	if (!node->left)
		return _insert_key(node, key);

	if (key <= node->low_key)
		node->left = insert(node->left, key);

	else if (key > node->low_key && key < node->high_key)
		node->middle = insert(node->middle, key);

	else
		node->right = insert(node->right, key);

	return _merge(node, key);
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