#ifndef AVLBST_H
#define AVLBST_H

typedef struct _node *Node;

extern Node search(Node node, long key);
extern Node insert(Node node, long key);
extern void print_node(const Node node);
extern void print(const Node node, int indent);

#endif