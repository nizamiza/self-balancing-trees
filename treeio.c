#include <stdio.h>
#include <stdlib.h>
#include "treeio.h"
#include "b-tree/2_3tree.h"

static inline void exit_program(int code)
{
	msg(CLR_YELLOW EXIT_MSG CLR_RESET);
	exit(code);
}

static inline void insert_nodes(Node *root)
{
	long key;

	while (scanf("%ld", &key) == 1)
		*root = insert(*root, key);
}

static inline void search_node(Node root)
{
	long key;

	scanf("%ld", &key);
	getchar();

	Node node = search(root, key);

	if (node) {
		print_node(node);
	} else {
		msg(NODE_NFOUND_MSG "\n");
	}
}

static inline void handle_cmd(char cmd, Node *root)
{
	switch (cmd) {
		case 'i'	: insert_nodes(root); 			break;
		case 'p'	: print(*root, 0); 				break;
		case 'b'	: print(*root, 0); 				break;
		case 's'	: search_node(*root);			break;
		case 'q'	: exit_program(EXIT_SUCCESS);	break;
		case 'r'	: *root = NULL;					break;
		case '\n'	: 								break;
		default		: throw(UNKNOWN_CMD_ERR); 		break;
	}
}

int main(void)
{
	char input;
	long key;

	Node root = NULL;

	while (scanf("%c", &input) == 1)
		handle_cmd(input, &root);
	
	return 0;
}