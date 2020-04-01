#ifndef B2_3TREE_H
#define B2_3TREE_H

typedef struct _internal_node *Node;

extern Node search(Node node, int key);
extern Node insert(Node node, int key);
extern void print(const Node node, int indent);
extern void print_node(const Node node);

#endif