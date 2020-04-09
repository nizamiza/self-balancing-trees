#ifndef RB_TREE_H
#define RB_TREE_H

typedef struct rbNode *Node;

extern Node search(Node node, int key);
extern Node insert(Node node, int key);
extern void print_node(const Node node);
extern void print(const Node node, int indent);

#endif
