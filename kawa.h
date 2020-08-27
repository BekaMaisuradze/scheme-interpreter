#ifndef _kawa_
#define _kawa_

#include "functions.h"
#include "vector.h"
#include "hashset.h"
#include "bool.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define num 0
#define string 1
#define fnc 2
#define tof 3	// boolean
#define qlst 4	// quoted list


int StringHash(const void *elem, int numBuckets);
void StringFree(void *str);
void two_str_free(void *buff);
int StringCmp(const void *elemAddr1, const void *elemAddr2);
static int ProceduresCmp(const void *elemAddr1, const void *elemAddr2);
static int procHash(const void *elem, int numBuckets);
int comparator(const void *elemAddr1, const void *elemAddr2);
bool isNotEvaluableList(char *str);
bool isNumber(char *str);
bool isString(char *str);
bool isBool(char *str);
int count_front_spaces(char *command);
int commands_real_len(char *command);
int check_braces(char *str);
int getFuncName(char *str, char *res);
void save_macro(char *command, hashset *macros);
funcname searchFn(hashset *map, char *name);
int getAllTokens(char *src, vector *tokens);
int replace_macro(void *macro_addr, char *name, char *final_command, int *name_len);
void printList(node *root);
node * parse(char *list, int l);



#endif