#ifndef TREEIO_H
#define TREEIO_H

#include <stdio.h>

#define CLR_RED     "\x1b[31m"
#define CLR_GREEN   "\x1b[32m"
#define CLR_YELLOW  "\x1b[33m"
#define CLR_BLUE    "\x1b[34m"
#define CLR_MAGENTA "\x1b[35m"
#define CLR_CYAN    "\x1b[36m"
#define CLR_RESET   "\x1b[0m"

#define UNKNOWN_CMD_ERR "Unknown command"
#define UNDEF_FUNC_T	"No defined function for the type"

#define EXIT_MSG		"Exiting...\n"
#define RET_TO_ROOT_MSG	"Returning to the root node\n"
#define NODE_FOUND_MSG	"Node found"
#define NODE_NFOUND_MSG	"Node not found"

#define ENOMEM "Insufficent memory, cannot allocate node"
#define ENONODE "Node is NULL, nothing to print"
#define EDUPNODE "Duplicate node insertion. Operation rejected"

#define WELCOME_MSG "-- Welcome to self-balancing bst interaction tool!\nCommands:\n\ti [...values] - insert keys\n\ts [value] - search for value\n\tp - print tree\n\tr - reset tree\n\tq - exit program\n--------------------------\n" 
	

#define EXIT_FUNC 		(printf(CLR_BLUE "--Exiting %s\n" CLR_RESET, __func__))
#define throw(error) 	(printf(CLR_RED "--Error: %s--" CLR_YELLOW " in %s\n" CLR_RESET, error, __func__))

#define msg(...)		(printf(__VA_ARGS__))

#define putws(n) {		\
	typeof(n) _n = n;	\
	while (_n-- > 0)	\
		putchar(' ');	\
}

#endif
