#ifndef AVL_TREE_AUGMENTED_H
#define AVL_TREE_AUGMENTED_H

#include <stdlib.h>
#include "treeio.h"

#define __max(a, b) ((a) > (b) ? (a) : (b))

struct _node {
	struct _node *left;
	struct _node *right;
	char bfactor;
	int key;
};

extern struct _node *_node(int key);
extern int _get_height(const struct _node* node);
extern struct _node *_set_bfactor(struct _node *node);
extern struct _node *_rotate_right(struct _node *node);
extern struct _node *_rotate_left(struct _node *node);
extern struct _node *_rebalance(struct _node *node, int key);
extern struct _node *search(struct _node *node, int key);
extern struct _node *insert(struct _node *node, int key);
extern const char *_bfactor_to_str(const struct _node *node);
extern void print_node(const struct _node *node);
extern void print(const struct _node *node, int indent);

#endif
