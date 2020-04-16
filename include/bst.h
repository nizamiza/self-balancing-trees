#ifndef BST_H
#define BST_H

typedef struct _node *Node;

extern Node search(Node node, int key);
extern Node insert(Node node, int key);
extern void clear(Node node);
extern void print_node(const Node node);
extern void print(const Node node, int indent);

#endif
