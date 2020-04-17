#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "include/treeio.h"
#include "include/2_3tree.h"

#define DATA_PATH "data/randnumbers"
#define OUT_PATH "data/timestamps"

#define BILLION 1000000000.0

#define __INIT_DELTA_TIME__ 		\	
	double time_d;			\
	struct timespec start, end;	\

#define __START_DELTA_TIME__			\
	time_d = 0.0;				\
	clock_gettime(CLOCK_REALTIME, &start);	\

#define __CALC_DELTA_TIME__				\
	clock_gettime(CLOCK_REALTIME, &end);		\
	time_d = ((end).tv_sec - (start).tv_sec) + 	\
		((end).tv_nsec - (start).tv_nsec) 	\
			/ BILLION

#define DELTA_TIME_FORMAT ">> %s: %lf seconds --\n"

#define __print_delta_time(stream)				\
	fprintf((stream), DELTA_TIME_FORMAT, __func__, time_d);	\


static inline void exit_program(int code)
{
	msg(CLR_YELLOW EXIT_MSG CLR_RESET);
	exit(code);
}

static inline void run_file_test(Node *root)
{
	FILE *input = fopen(DATA_PATH, "r");
	FILE *output = fopen(OUT_PATH, "w");

	if (!input || !output)
		return;

	int n;
	int counter = 0;
	double avg_time = 0.0; 

	__INIT_DELTA_TIME__;

	fprintf(output, "---- Inserting nodes ----\n\n");

	while (fscanf(input, "%d", &n) == 1) {
		__START_DELTA_TIME__;

		*root = insert(*root, n);

		__CALC_DELTA_TIME__;
		__print_delta_time(output);

		avg_time += time_d;
		counter++;
	}

	avg_time /= (double) counter;

	fprintf(output, "\n\n==== Average insertion time: %lf seconds\n\n", avg_time);
	fprintf(output, "\n\n---- Searching nodes ----\n\n");

	rewind(input);

	avg_time = 0.0;
	counter = 0;

	while (fscanf(input, "%d", &n) == 1) {
		__START_DELTA_TIME__;

		search(*root, n);

		__CALC_DELTA_TIME__;
		__print_delta_time(output);

		avg_time += time_d;
		counter++;
	}

	avg_time /= (double) counter;
	fprintf(output, "\n\n==== Average search time: %lf seconds\n\n", avg_time);

	fclose(input);
	fclose(output);
	
	msg(TESTS_COMPLETE_MSG);
}

static inline void insert_nodes(Node *root)
{
	int key;
	
	__INIT_DELTA_TIME__;

	while (scanf("%d", &key) == 1) {
		__START_DELTA_TIME__;
		
		*root = insert(*root, key);

		__CALC_DELTA_TIME__;
		__print_delta_time(stdout);
	}
}

static inline void search_node(Node root)
{ 
	int key;

	__INIT_DELTA_TIME__;

	if (scanf("%d", &key) != 1)
		return;
	getchar();
	
	__START_DELTA_TIME__;

	Node node = search(root, key);
	
	__CALC_DELTA_TIME__;
	__print_delta_time(stdout);

	if (node) {
		print_node(node);
	} else {
		msg(NODE_NFOUND_MSG "\n");
	}
}

static inline void handle_cmd(char cmd, Node *root)
{
	switch (cmd) {
		case 'h'	: msg(COMMANDS);				break;
		case 'i'	: insert_nodes(root); 			break;
		case 'p'	: print(*root, 0); 				break;
		case 's'	: search_node(*root);			break;
		case 'q'	: exit_program(EXIT_SUCCESS);	break;
		case 'r'	: clear(*root);					break;
		case 't'	: run_file_test(root);			break;
		case '\n'	: 								break;
		default		: throw(UNKNOWN_CMD_ERR); 		break;
	}
}

int main(void)
{
	char input;

	Node root = NULL;
	msg(WELCOME_MSG);

	while (scanf("%c", &input) == 1)
		handle_cmd(input, &root);
	
	return 0;
}
