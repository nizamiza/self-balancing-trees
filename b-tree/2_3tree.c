#include <stdlib.h>
#include <stdio.h>

#define ORDER 3
#define INDENT_INC 4

enum _node_children { LEFT, MIDDLE, RIGHT, EXTRA };
enum _node_keys { LOW, MID, HIGH };

struct _internal_node {
	struct _internal_node *children[ORDER + 1];
	struct _internal_node *parent;
	char keys_count;
	char children_count;
	int keys[ORDER];
};

static inline struct _internal_node *new_node(int key)
{
	struct _internal_node *node =
		(struct _internal_node *) malloc(sizeof(struct _internal_node));

	if (!node)
		return NULL;

	*node = (struct _internal_node) { { NULL }, NULL, 1, 0, { key, 0 } };
	return node;
}

static inline int compare_keys(const void *key_a, const void *key_b)
{
	return *(int *) key_a - *(int *) key_b;
}

static inline
struct _internal_node *add_key(struct _internal_node *node, int key)
{
	if (!node)
		return new_node(key);

	node->keys[node->keys_count++] = key;
	qsort(node->keys, node->keys_count, sizeof(int), compare_keys);

	return node;
}

static inline
struct _internal_node *add_child(
	struct _internal_node *node, struct _internal_node *child
) {
	node->children[node->children_count++] = child;
	return node;
}

static inline
struct _internal_node *split_node(struct _internal_node *node)
{
	if (node->keys_count < ORDER)
		return node;

	if (node->children_count < ORDER + 1) {
		return add_child(
			add_child(
				add_key(node->parent, node->keys[MID]),
				new_node(node->keys[LOW])
			), new_node(node->keys[HIGH])
		);
	}

	return add_child(
		add_child(
			add_key(node->parent, node->keys[MID]),
			add_child(
				add_child(
					new_node(node->keys[LOW]),
					node->children[LEFT]
				), node->children[MIDDLE]
			)
		), add_child(
			add_child(
				new_node(node->keys[HIGH]),
				node->children[RIGHT]
			), node->children[EXTRA]
		)
	);
}

struct _internal_node *insert(struct _internal_node *node, int key)
{
	return split_node(add_key(node, key));
}

struct _internal_node *search(struct _internal_node *node, int key)
{
	return NULL;
}

void print_node(const struct _internal_node *node)
{
	if (!node)
		return;

	for (int i = 0; i < node->keys_count; i++)
		printf("[%d] ", node->keys[i]);
	printf("\n");
}

void print(const struct _internal_node *node, int indent)
{
	if (!node)
		return;

	int i;

	putchar('[');

	for (i = 0; i < node->keys_count; i++)
		printf("%d ", node->keys[i]);

	putchar(']');

	for (i = 0; i < indent; i++)
		putchar(' ');

	printf("--|\n|--");

	for (i = 0; i < node->children_count; i++)
		print(node->children[i], indent + INDENT_INC);
}