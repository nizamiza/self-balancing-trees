#ifndef BST_AUGMENTED_H
#define BST_AUGMENTED_H

#include <stdlib.h>
#include "../include/treeio.h"

struct _node {
	int key;
	struct _node *left;
	struct _node *right;
};

extern struct _node *_node(int key);
extern struct _node *search(struct _node *node, int key);
extern struct _node *insert(struct _node *node, int key);
extern void clear(struct _node *node);

#endif
