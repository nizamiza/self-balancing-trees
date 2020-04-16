#ifndef B2_3TREE_AUGMENTED_H
#define B2_3TREE_AUGMENTED_H

/**
 * Implementation of 2-3 tree.
 * Nizomiddin Toshpulatov <tremul27@gmail.com>
 * 2020, FIIT STU, Bratislava, Slovakia
 */

#include <stdlib.h>
#include <stdbool.h>
#include "treeio.h"

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

extern struct _node *_node(struct _node init);
extern struct _node *_update_node(struct _node *node, struct _node update);
extern int *_sort_keys(int a, int b, int c);
extern struct _node *_merge_with_parent(struct _merge_args args);
extern struct _node *_split(struct _node *node, int key);
extern struct _node *_add_key(struct _node *node, int key);
extern struct _node *_get_root(struct _node *node);
extern struct _node *_insert_key(struct _node *node, int key);
extern struct _node *search(struct _node *node, int key);
extern struct _node *insert(struct _node *node, int key);
extern void clear(struct _node *node);

#endif
