#ifndef B2_3TREE_H
#define B2_3TREE_H

#ifndef INTERNAL_TESTING

typedef struct _node *Node;

extern Node search(Node node, int key);
extern Node insert(Node node, int key);
extern void print(const Node node, int indent);
extern void print_node(const Node node);

#else
#include <stdbool.h>

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

#endif
#endif