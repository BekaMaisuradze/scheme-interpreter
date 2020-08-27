#ifndef _functions_
#define _functions_

#include "bool.h"
#include "hashset.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

typedef struct node_t {
	int id;
	int *valNode;
	struct node_t *next;
} node;

typedef node* (*funcname)(node *, hashset *, hashset *);


node * car(node *root, hashset *map, hashset *macros);

node * cdr(node *root, hashset *map, hashset *macros);

node * cons(node *root, hashset *map, hashset *macros);

node * list(node *root, hashset *map, hashset *macros);

node *append(node *root, hashset *map, hashset *macros);

///// MATH /////

node * plus(node *root, hashset *map, hashset *macros);	// double

node * minus(node *root, hashset *map, hashset *macros);	// double

node * multiply(node *root, hashset *map, hashset *macros);	// double

node * divide(node *root, hashset *map, hashset *macros);	// double

node * quotient(node *root, hashset *map, hashset *macros); 	// int

node * rest(node *root, hashset *map, hashset *macros);	// int

node * isPositive(node *root, hashset *map, hashset *macros);	// char

node * isNegative(node *root, hashset *map, hashset *macros);	// char

node * isZero(node *root, hashset *map, hashset *macros);	// char

node * isOdd(node *root, hashset *map, hashset *macros);	// char

node * isEven(node *root, hashset *map, hashset *macros);	// char

node * square_root(node *root, hashset *map, hashset *macros);	// double

node * expt(node *root, hashset *map, hashset *macros);	// double

node * neper(node *root, hashset *map, hashset *macros);	// double

// strings ///

node * stringequals(node *root, hashset *map, hashset *macros);

node * stringless(node *root, hashset *map, hashset *macros);

node * stringgreater(node *root, hashset *map, hashset *macros);

node * stringgreater_equal(node *root, hashset *map, hashset *macros);

node * stringless_equal(node *root, hashset *map, hashset *macros);

node * string_case_cmp(node *root, hashset *map, hashset *macros);

node * string_append(node *root, hashset *map, hashset *macros);

node * substr(node *root, hashset *map, hashset *macros);

node * is_greater(node *root, hashset *map, hashset *macros);

node * is_less(node *root, hashset *map, hashset *macros);

node * is_equal(node *root, hashset *map, hashset *macros);

node * generic_equal(node *root, hashset *map, hashset *macros);

node * is_list(node *root, hashset *map, hashset *macros);

node * is_null(node *root, hashset *map, hashset *macros);

node * list_len(node *root, hashset *map, hashset *macros);

node * is_string(node *root, hashset *map, hashset *macros);

node * is_char(node *root, hashset *map, hashset *macros);

node * is_integer(node *root, hashset *map, hashset *macros);

node * is_real(node *root, hashset *map, hashset *macros);

node * is_bool(node *root, hashset *map, hashset *macros);

node * is_procedure(node *root, hashset *map, hashset *macros);

node * and(node *root, hashset *map, hashset *macros);

node * or(node *root, hashset *map, hashset *macros);

node * not(node *root, hashset *map, hashset *macros);

node * scm_if(node *root, hashset *map, hashset *macros);

node * cond(node *root, hashset *map, hashset *macros);

node * let(node *root, hashset *map, hashset *macros);

// mapping functions //

node * apply(node *root, hashset *map, hashset *macros);

node * eval(node *root, hashset *map, hashset *macros);

node * generic_map(node *root, hashset *map, hashset *macros);	// combines two mapping functions declared below



// not in map

void loadFunctions(hashset *map);

void storeFuncIntoMap(hashset *map, char *fname, funcname fn);

node * evaluateNode(node *node, hashset *map, hashset *macros);

node * wrapper(void *val, int type);

int count_bytes(int *valNode);

node * clone(node *a);

node * make_empty_list();

node * operators_overload(node *root, hashset *map, char *ope, hashset *macros);

node * list_equal(node *fst, node *sec);

node * operators_for_isfuncs(node *root, hashset *map, char ope, hashset *macros);

node * is(node *root, hashset *map, char aux, hashset *macros);

node * bool_funcs(node *root, hashset *map, char aux, hashset *macros);

node * string_cmp(node *root, hashset *map, hashset *macros, char *ope);

node * unary_map(node *root, hashset *map, hashset *macros);		// takes a list

node * nnary_map(node *root, hashset *map, int numlists, hashset *macros);		// takes lists

int get_minlen_of_lists(node *root, hashset *map, hashset *macros);

funcname getfunction(node *root, char *func, hashset *map, hashset *macros);

char * un_parse(node *root, hashset *map, hashset *macros, int caller_type);

void change(node *root, hashset *vars); 		// changes variables with appropriate values

node * lambda(node *root, hashset *map, hashset *macros);

void deallocate_memory(node *root);




//// J U N K /////



void unparse(node *root, char *res);
char * eval_node_for_string(node *root, hashset *map);
node * eval_node_for_quote(node *root, hashset *map);



#endif