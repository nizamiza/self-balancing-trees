#include <stdlib.h>
#include <time.h>
#include "include/treeio.h"
#include "include/avltree.h"
#include "include/avltree_assert.h"

#define BILLION 1000000000.0
#define __calc_delta_time(start, end) (	\
	((end).tv_sec - (start).tv_sec) + 	\
	((end).tv_nsec - (start).tv_nsec) 	\
	/ BILLION							\
)

#define __time_d(t) (													\
	printf(CLR_CYAN ">> %s: %lf seconds --\n" CLR_RESET, __func__, t)	\
)


static inline void exit_program(int code)
{
	msg(CLR_YELLOW EXIT_MSG CLR_RESET);
	exit(code);
}

static inline void insert_nodes(Node *root)
{
	int key;
	double time_d;
	
	struct timespec start, end;

	while (scanf("%d", &key) == 1) {
		time_d = 0.0;
		clock_gettime(CLOCK_REALTIME, &start);
		
		*root = insert(*root, key);
		clock_gettime(CLOCK_REALTIME, &end);
		
		time_d = __calc_delta_time(start, end);
		__time_d(time_d);
	}
}

static inline void search_node(Node root)
{ 
	int key;
	double time_d = 0.0;

	struct timespec start, end;

	if (scanf("%d", &key) != 1)
		return;
	getchar();
	
	clock_gettime(CLOCK_REALTIME, &start);
	Node node = search(root, key);
	
	clock_gettime(CLOCK_REALTIME, &end);
	time_d = __calc_delta_time(start, end);
	__time_d(time_d);

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

	Node root = NULL;
	printf(WELCOME_MSG);

	while (scanf("%c", &input) == 1)
		handle_cmd(input, &root);
	
	return 0;
}
