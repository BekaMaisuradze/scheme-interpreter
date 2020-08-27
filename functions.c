#include "functions.h"
#include "kawa.h"



/************** MATH ******************/

node * plus(node *root, hashset *map, hashset *macros)
{
	double sum = 0;
	node *curr;
	while (root)
	{
		curr = evaluateNode((node *) root->valNode, map, macros);
		if (!curr || *curr->valNode != num)
		{
			printf("Invalid type argument to arithmetic expression.\n");
			return NULL;	// func returned NULL or not-appropriate type value
		}
		sum += *(double *) (curr->valNode + 1);
		deallocate_memory(curr);
		root = root->next;
	}
	return wrapper(&sum, num);
}

node * minus(node *root, hashset *map, hashset *macros)
{
	node *first = evaluateNode((node *) root->valNode, map, macros);
	if (!first || *first->valNode != num)
	{
		printf("Invalid type argument to arithmetic expression.\n");
		return NULL;	// func returned NULL or not-appropriate type value
	}
	node *sum = plus(root->next, map, macros);
	if (!sum || *sum->valNode != num) return NULL;

	double res = *(double *) (first->valNode + 1);
	res -= *(double *) (sum->valNode + 1);
	deallocate_memory(first);
	deallocate_memory(sum);
	
	return wrapper(&res, num);
}

node * multiply(node *root, hashset *map, hashset *macros)
{
	double mult = 1;
	node *curr;
	while (root)
	{
		curr = evaluateNode((node *) root->valNode, map, macros);
		if (!curr || *curr->valNode != num) 
		{
			printf("Invalid type argument to arithmetic expression.\n");
			return NULL;	// func returned NULL or not-appropriate type value
		}
		mult *= *(double *) (curr->valNode + 1);
		deallocate_memory(curr);
		root = root->next;
	}
	return wrapper(&mult, num);
}

node * divide(node *root, hashset *map, hashset *macros)
{
	//printf("divide\n");
	if (!root) return NULL;
	node *first = evaluateNode((node *) root->valNode, map, macros);
	if (!first || *first->valNode != num)
	{
		printf("Invalid type argument to arithmetic expression.\n");
		return NULL;	// func returned NULL or not-appropriate type value
	}
	node *mult = multiply(root->next, map, macros);
	if (!mult || *mult->valNode != num) return NULL;

	double res = *(double *) (first->valNode + 1);
	res /= *(double *) (mult->valNode + 1);
	deallocate_memory(first);
	deallocate_memory(mult);
	
	return wrapper(&res, num);
}

node * quotient(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next) { printf("call to 'quotient' must have 2 arguments.\n"); return NULL; }
	node *first = (node *) root->valNode, *second = (node *) root->next->valNode;
	if (root->next->next) {	printf("call to 'quotient' has too many arguments.\n");	return NULL; }

	node *f = evaluateNode(first, map, macros);
	node *s = evaluateNode(second, map, macros);
	if (!f || *f->valNode != num || !s || *s->valNode != num)
	{
		printf("Invalid type argument to 'quotient' function.\n");
		return NULL;
	}
	double x = *(double *) (f->valNode + 1);
	double y = *(double *) (s->valNode + 1);
	deallocate_memory(f);
	deallocate_memory(s);

	double res = floor(x/y);
	return wrapper(&res, num);
}

node * rest(node *root, hashset *map, hashset *macros)
{
	if (!root->next) { printf("call to 'remainder' must have 2 arguments.\n"); return NULL; }
	node *first = (node *) root->valNode, *second = (node *) root->next->valNode;
	if (root->next->next) {	printf("call to 'remainder' has too many arguments.\n"); return NULL; }

	node *f = evaluateNode(first, map, macros);
	node *s = evaluateNode(second, map, macros);
	if (!f || *f->valNode != num || !s || *s->valNode != num)
	{
		printf("Invalid type argument to 'remainder function.\n");
		return NULL;
	}
	int x = *(double *) (f->valNode + 1);
	int y = *(double *) (s->valNode + 1);
	deallocate_memory(f);
	deallocate_memory(s);

	double res = x%y;
	return wrapper(&res, num);
}

node * isPositive(node *root, hashset *map, hashset *macros)
{
	if (!root) return NULL;

	return operators_for_isfuncs(root, map, '>', macros);
}

node * isNegative(node *root, hashset *map, hashset *macros)
{
	if (!root) return NULL;

	return operators_for_isfuncs(root, map, '<', macros);
}

node * isZero(node *root, hashset *map, hashset *macros)
{
	if (!root) return NULL;

	return operators_for_isfuncs(root, map, '=', macros);
}

node * isOdd(node *root, hashset *map, hashset *macros)
{
	if (!root) return NULL;

	return operators_for_isfuncs(root, map, 'o', macros);
}

node * isEven(node *root, hashset *map, hashset *macros)
{
	if (!root) return NULL;

	return operators_for_isfuncs(root, map, 'e', macros);
}

node * square_root(node *root, hashset *map, hashset *macros)
{
	if (!root) { printf("call to 'sqrt' must have 1 argument.\n"); return NULL; }
	if (root->next) { printf("call to 'sqrt' has too many arguments.\n"); return NULL; }

	node *wrapped = evaluateNode((node *) root->valNode, map, macros);
	if (!wrapped || *wrapped->valNode != num)
	{
		printf("Invalid type argument to 'sqrt' function.\n");
		return NULL;
	}

	double x = *(double *) (wrapped->valNode + 1);
	deallocate_memory(wrapped);

	double res = sqrt(x);
	return wrapper(&res, num);
}

node * expt(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next) { printf("call to 'expt' must have 2 arguments.\n"); return NULL; }
	if (root->next->next) {	printf("call to 'expt' has too many arguments.\n"); return NULL; }

	node *f = evaluateNode((node *)root->valNode, map, macros);
	node *s = evaluateNode((node *)root->next->valNode, map, macros);
	if (!f || *f->valNode != num || !s || *s->valNode != num) {	printf("Invalid type argument to 'expt' function.\n"); return NULL; }
	double x = *(double *) (f->valNode + 1);
	double y = *(double *) (s->valNode + 1);
	deallocate_memory(f);
	deallocate_memory(s);

	double ex = pow(x, y);

	return wrapper(&ex, num);
}

node * neper(node *root, hashset *map, hashset *macros)
{
	if (!root) { printf("call to 'exp' must have 1 argument.\n"); return NULL; }
	if (root->next) { printf("call to 'exp' has too many arguments.\n"); return NULL; }

	node *wrapped = evaluateNode((node *) root->valNode, map, macros);
	if (!wrapped || *wrapped->valNode != num) { printf("Invalid type argument to 'exp' function.\n"); return NULL; }

	double nep = exp(*(double *) (wrapped->valNode + 1));
	deallocate_memory(wrapped);

	return wrapper(&nep, num);
}


/**** L I S T S *****/

node * car(node *root, hashset *map, hashset *macros)
{
	if (root->next) { printf("call to 'car' has too many arguments.\n"); return NULL; }
	if (!root->valNode || (*root->valNode != qlst && *root->valNode != fnc) || !((node *) root->valNode)->valNode)
		{ printf("list must not be empty.\n"); return NULL; }

	node *fst;
	if (*root->valNode == qlst) {
		fst = clone((node *) root->valNode);
	} else if (*root->valNode == fnc) {
		node *evaled = evaluateNode((node *) root->valNode, map, macros);
		if (!evaled || evaled->id != qlst) { printf("Passed wrong type argument to 'car'.\n"); return NULL; }
		deallocate_memory(evaled);
		fst = clone((node *) evaled->valNode);
	}
	
	return fst;
}

node * cdr(node *root, hashset *map, hashset *macros)
{
	if (root->next) { printf("call to 'cdr' has too many arguments.\n"); return NULL; }
	if (!root->valNode || (*root->valNode != qlst && *root->valNode != fnc))
		{ printf("Wrong type passed to 'cdr'.\n"); return NULL; }
	if (!((node *) root->valNode)->valNode) { printf("list must not be empty.\n"); return NULL; }

	node *res = NULL, *curr = NULL;
	
	node *iterable; bool del = false;
	if (*root->valNode == fnc) {
		//node *t = (node *)root->valNode;
		iterable = evaluateNode((node *) root->valNode, map, macros);
		if (!iterable || !iterable->valNode) { printf("Passed wrong type argument to 'cdr'.\n"); return NULL; }
		iterable = ((node *) iterable->valNode)->next;
		del = true;
	} else {
		iterable = ((node *)root->valNode)->next;
	}
	if (!iterable)	// i.e. empty list
		return make_empty_list();
	
	if (*(int *) iterable != qlst) { printf("Passed wrong type argument to 'cdr'.\n"); return NULL; }

	for (; iterable != NULL; iterable = iterable->next)
	{
		node *value;
		value = clone(iterable);
		if (!res) {
			res = value;
			curr = res;
		} else {
			curr->next = value;
			curr = curr->next;
		}
	}
	if (curr) curr->next = NULL;
	if (del) deallocate_memory(iterable);

	return wrapper(res, qlst);
}

node * cadr(node *root, hashset *map, hashset *macros)
{
	if (root->next) { printf("call to 'cadr' has too many arguments.\n"); return NULL; }
	if (!root->valNode || (*root->valNode != qlst && *root->valNode != fnc) ||  !((node *) root->valNode)->valNode)
		{ printf("list must not be empty."); return NULL; }
	if (!((node *) root->valNode)->next) { printf("call to 'cadr' must have 2 or more elements.\n"); return NULL; }

	node *sec;
	if (*root->valNode == qlst) {
		sec = clone(((node *) root->valNode)->next);
	} else if (*root->valNode == fnc) {
		node *evaled = evaluateNode((node *) root->valNode, map, macros);
		if (!evaled || evaled->id != qlst) { printf("Passed wrong type argument to 'cdr'.\n"); return NULL; }
		sec = clone(((node *) evaled->valNode)->next);
		deallocate_memory(evaled);
	}

	return sec;
}

node * cons(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || root->next->next) { printf("'cons' must have 2 arguments.\n"); return NULL; }
	if (!root->next->valNode || (*root->next->valNode != qlst && *root->next->valNode != fnc))
		{ printf("second argument must be a list.\n"); return NULL; }

	node *firstDup = evaluateNode((node *) root->valNode, map, macros);
	if (!firstDup) { printf("Invalid type(s) to 'cons'.\n"); return NULL; }

	node *curr = firstDup;

	if (((node *)root->next->valNode)->id != fnc)
	{
		node *listNode = (node *) root->next->valNode;
		for (; listNode != NULL; listNode = listNode->next)
		{
			curr->next = evaluateNode((node *) listNode->valNode, map, macros);
			if (!curr->next) break;
			curr = curr->next;
		}
	}
	else
	{
		node *evaled = evaluateNode((node *) root->next->valNode, map, macros);
		if (evaled)
			firstDup->next = (node *)evaled->valNode;
	}

	return wrapper(firstDup, qlst);
}

node * list(node *root, hashset *map, hashset *macros)
{
	node *res = NULL, *curr = NULL;

	while (root) {
		node *value;
		if (*root->valNode == fnc) {
			value = evaluateNode((node *)root->valNode, map, macros);
		} else if (*root->valNode == qlst) {
			if (!((node *) root->valNode)->valNode) value = make_empty_list();
			else value = (node *) list((node *) root->valNode, map, macros);
		} else {
			value = clone(root);
			value->id = qlst;
		}
		if (!value) { printf("Invalid value in 'list'.\n"); return NULL; }
		if (!res) {res = value; curr = res;}
		else {curr->next = value; curr = curr->next;}
		root = root->next;
	}
	curr->next = NULL;

	return wrapper(res, qlst);
}

node *append(node *root, hashset *map, hashset *macros)
{
	//node *first = evaluateNode((node *)root->valNode, map, macros), *curr;	// check if first node is 4-type list
	//if (!first || !first->valNode) return NULL;
	node *first = NULL, *curr = NULL;

	//int counter;	// to differ first iteration from others
	//bool check;		// to check if funct occured
	for (; root; root = root->next)
	{
		//counter = 0;
		bool check = false;
		node *curr_elem = (node *) root->valNode, *evaled;
	//	if (!curr_elem) continue;
		//printf("%d - %d\n", curr_elem->id, *curr_elem->valNode);
		if (curr_elem->id == fnc) { curr_elem = evaluateNode(curr_elem, map, macros); check = true; }
		if (!curr_elem) return NULL;
		if (!curr_elem->valNode) continue;
		if (*curr_elem->valNode == qlst && check) curr_elem = (node *) curr_elem->valNode;
		//printf("%d - %d\n", curr_elem->id, *curr_elem->valNode);
		for (; curr_elem; curr_elem = curr_elem->next)
		{
			//if (counter++ == 0 && curr_elem->id == fnc) {printf("sdc\n"); evaled = evaluateNode((node *)curr_elem, map, macros); check = true;}
			evaled = evaluateNode((node *)curr_elem->valNode, map, macros);
			if (!evaled) return NULL;

			// printf("appending : %d\n", *evaled->valNode);

			if (!first) {first = evaled; curr = first;}
			else {curr->next = evaled; curr = curr->next;}
			//if (check) break;
		}
	}
	return wrapper(first, qlst);
}



/**** S T R I N G S  and  B O O L E A N S ******/

node * stringequals(node *root, hashset *map, hashset *macros) { return string_cmp(root, map, macros, "="); }

node * stringless(node *root, hashset *map, hashset *macros) { return string_cmp(root, map, macros, "<"); }

node * stringgreater(node *root, hashset *map, hashset *macros) { return string_cmp(root, map, macros, ">"); }

node * stringgreater_equal(node *root, hashset *map, hashset *macros) { return string_cmp(root, map, macros, ">="); }

node * stringless_equal(node *root, hashset *map, hashset *macros) { return string_cmp(root, map, macros, "<="); }

node * string_case_cmp(node *root, hashset *map, hashset *macros) { return string_cmp(root, map, macros, "case_ins"); }

node * string_append(node *root, hashset *map, hashset *macros)
{

	int len = 0;	
	node *tmp;
	for (tmp = root; tmp != NULL; tmp = tmp->next) {
		node *evaled = evaluateNode((node *) tmp->valNode, map, macros);
		if (!evaled || *evaled->valNode != string) { printf("Invalid type to 'string-append'.\n"); return NULL; }
		len += strlen((char *) (evaled->valNode + 1));
	}
	++len; // for '\0'

	char res[len]; res[0] = '\0';

	while (root)
	{
		node *wrapped = evaluateNode((node *) root->valNode, map, macros);
		if (!wrapped || *wrapped->valNode != string) return NULL;
		strcat(res, (char *) (wrapped->valNode + 1));
		root = root->next;
		deallocate_memory(wrapped);
	}
	res[len-1] = '\0';
	return wrapper(res, string);
}

node * substr(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || !root->next->next) { printf("Call to 'substring' must have 3 arguments.\n"); return NULL; }
	if (root->next->next->next) { printf("call to 'substring' has too many arguments.\n"); return NULL; }

	node *str_wr = evaluateNode((node *) root->valNode, map, macros);
	node *start_wr = evaluateNode((node *) root->next->valNode, map, macros);
	node *end_wr = evaluateNode((node *) root->next->next->valNode, map, macros);
	if (!str_wr || *str_wr->valNode != string || !start_wr || *start_wr->valNode != num || !end_wr || *end_wr->valNode != num)
		{ printf("Invalid type argument(s) to 'substring'.\n"); return NULL; }

	char *str = (char *) (str_wr->valNode + 1);
	int start = *(double *) (start_wr->valNode + 1);
	int end = *(double *) (end_wr->valNode + 1);

	if (start > end || start < 0 || end > strlen(str) - 1)
		{ printf("CHECK arguments for 'substring' maaaan! -_-\n"); return NULL; }

	char res[end-start+1];
	int i;
	for (i = 0; i < end - start; ++i)
		res[i] = str[start+i];

	res[end-start] = '\0';
	deallocate_memory(str_wr);
	deallocate_memory(start_wr);
	deallocate_memory(end_wr);

	return wrapper(res, string);
}

node * is_greater(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || !root->valNode || !root->next->valNode)
		{ printf("Call to '>' compare operator must have 2 or more arguments.\n"); return NULL; }

	return operators_overload(root, map, ">", macros);
}

node * is_less(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || !root->valNode || !root->next->valNode)
		{ printf("Call to '<' compare operator must have 2 or more arguments.\n"); return NULL; }

	return operators_overload(root, map, "<", macros);
}

node * is_equal(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || !root->valNode || !root->next->valNode)
		{ printf("Call to '=' compare operator must have 2 or more arguments.\n"); return NULL; }

	return operators_overload(root, map, "=", macros);
}

node * is_less_or_equal(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || !root->valNode || !root->next->valNode)
		{ printf("Call to '<=' compare operator must have 2 or more arguments.\n"); return NULL; }

	return operators_overload(root, map, "<=", macros);
}

node * is_greater_or_equal(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || !root->valNode || !root->next->valNode)
		{ printf("Call to '>=' compare operator must have 2 or more arguments.\n"); return NULL; }

	return operators_overload(root, map, ">=", macros);
}

node * generic_equal(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || !root->valNode || !root->next->valNode)
		{ printf("Call to compare operator must have 2 arguments.\n"); return NULL; }

	node *fst = evaluateNode((node *) root->valNode, map, macros);
	if (!fst) return NULL;
	node *sec = evaluateNode((node *) root->next->valNode, map, macros);
	if (!sec) { deallocate_memory(fst); return NULL; }

	bool b = false;
	node *res;
	
	if (*fst->valNode == *sec->valNode)
	{
		int type = *fst->valNode;
		if (type == num)
			res = is_equal(root, map, macros);
		else if (type == string)
			res = stringequals(root, map, macros);
		else if (type == tof)
			b = *(root->valNode + 1) == *(root->next->valNode + 1);
		else if (type == qlst)
			res = list_equal((node *) fst->valNode, (node *) sec->valNode);
	
		if (type != tof && res && res->valNode && *res->valNode == tof)
			b = *(bool *) (res->valNode + 1);
	}

	deallocate_memory(fst);
	deallocate_memory(sec);

	return wrapper(&b, tof);
}

node * is_list(node *root, hashset *map, hashset *macros) { return is(root, map, 'l', macros); }

node * is_string(node *root, hashset *map, hashset *macros) { return is(root, map, 's', macros); }

node * is_char(node *root, hashset *map, hashset *macros) { return is(root, map, 'c', macros); }

node * is_integer(node *root, hashset *map, hashset *macros) { return is(root, map, 'i', macros); }

node * is_real(node *root, hashset *map, hashset *macros) { return is(root, map, 'r', macros); }

node * is_bool(node *root, hashset *map, hashset *macros) { return is(root, map, 'b', macros); }

node * is_procedure(node *root, hashset *map, hashset *macros) { return is(root, map, 'p', macros); }

node * and(node *root, hashset *map, hashset *macros) { return bool_funcs(root, map, 'a', macros); }

node * or(node *root, hashset *map, hashset *macros) { return bool_funcs(root, map, 'o', macros); }

node * not(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->valNode || root->next) { printf("There must be exactly one argument.\n"); return NULL; }

	node *e = evaluateNode((node *) root->valNode, map, macros);
	if (!e || *e->valNode != tof) { printf("Invalid type argumen.\n"); return NULL; }

	bool b = !(*(bool *) (e->valNode + 1));
	deallocate_memory(e);

	return wrapper(&b, tof);
}

node * is_null(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->valNode || root->next) { printf("'null?' function must have 1 argument.\n"); return NULL; }
	bool b = false;
	if (*root->valNode == fnc)
	{
		node *evaled = evaluateNode((node *) root->valNode, map, macros);
		if (!evaled || evaled->id != qlst) { printf("Invalid type to call 'null?'.\n"); return NULL; }
		if (!evaled->valNode) { deallocate_memory(evaled); b = true; return wrapper(&b, tof); }
	}
	if (!((node *)root->valNode)->valNode) b = true;
	return wrapper(&b, tof);
}

node * list_len(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->valNode || root->next) { printf("Call to 'length' must have one list-argument.\n"); return NULL; }

	double c = 0;
	if (*root->valNode == fnc)
	{
		node *evaled = evaluateNode((node *) root->valNode, map, macros);
		if (!evaled || evaled->id != qlst) { printf("Invalid type to call 'length'.\n"); return NULL; }
		for (evaled = (node *) evaled->valNode; evaled != NULL; evaled = evaled->next) ++c;
		deallocate_memory(evaled);
	}
	else  if (*root->valNode == qlst)
	{
		node *fst = (node *) root->valNode;
		if (fst->valNode)
			for (; fst != NULL; fst = fst->next) ++c;
	}
	else { printf("argument must be a list.\n"); return NULL; }

	return wrapper(&c, num);
}

node * scm_if(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || !root->next->next || root->next->next->next ||
		!root->valNode || !root->next->valNode || !root->next->next->valNode)
			{ printf("'if' condition must have 3 argument.\n"); return NULL; }

	node *e = evaluateNode((node *) root->valNode, map, macros);
	if (!e || *e->valNode != tof) { printf("first argument must be a boolean one.\n"); return NULL; }

	bool answ = *(bool *) (e->valNode + 1);
	deallocate_memory(e);

	if (!answ) return evaluateNode((node *) root->next->next->valNode, map, macros);
	else return evaluateNode((node *) root->next->valNode, map, macros);
}

node * cond(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->valNode || *root->valNode != fnc || !((node *) root->valNode)->next)
		{ printf("'cond' must have one cond-expressions list-argument.\n"); return NULL; }

	node *fst = (node *) ((node *) root->valNode)->valNode;
	node *sec = (node *) ((node *) root->valNode)->next->valNode;
	if (!fst || !sec) return NULL;
	
	if (fst->id == string) {
		if (strcmp((char *) fst + sizeof(int), "else") == 0) {
			return evaluateNode(sec, map, macros);
		} else {
			printf("Invalid word %s in 'cond'.", (char *) fst->valNode);
			return NULL;
		}
	}

	node *eval_expr = evaluateNode(fst, map, macros);
	if (!eval_expr) { printf("Invalid condition in 'cond'.\n"); return NULL; }
	bool expr = *(bool *) (eval_expr->valNode + 1);

	node *result;
	if (expr) result = evaluateNode(sec, map, macros);
	else result = cond(root->next, map, macros);
	deallocate_memory(eval_expr);

	return result;
}

node * let(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->valNode) { printf("First argument must be a name-expressions list.\n"); return NULL; }

	node *pair = (node *) root->valNode;

	hashset vars;
	HashSetNew(&vars, 2 * sizeof(void *), 3, StringHash, StringCmp, two_str_free);

	node *value_node;
	for (; pair != NULL; pair = pair->next)
	{
		if (!pair->valNode || !((node *) pair->valNode)->valNode) break;
		node *name_node = evaluateNode(((node *) ((node *) pair->valNode)->valNode), map, macros);
		if (!name_node) { printf("Error evaluating let's name.\n"); return NULL; }
		char *name = strdup((char *) (name_node->valNode + 1));
		// printf("name -> %s\n", name);

		value_node = evaluateNode(((node *) ((node *) pair->valNode)->next->valNode), map, macros);
		if (!value_node) { printf("Error evaluating let's value.\n"); return NULL; }
		char *value = un_parse(value_node, map, macros, fnc);
		// printf("value -> %s\n", value);

		char buffer[2*sizeof(void *)];
		memcpy(buffer, &name, sizeof(void *));
		memcpy(buffer + sizeof(void *), &value, sizeof(void *));
		HashSetEnter(&vars, buffer);

		deallocate_memory(name_node);
		deallocate_memory(value_node);
	}

	change(root->next, &vars);
	node *res = evaluateNode((node *) root->next->valNode, map, macros);
	HashSetDispose(&vars);
	return res;
}


/******** M A P P I N G    F U N C T I O N S *********/

node * apply(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->next || !root->next->valNode || root->next->next)
		{ printf("Call to 'apply' must have 2 arguments.\n"); return NULL; }
	node *saved = root->next;
	node *arg = evaluateNode((node *) root->next->valNode, map, macros);
	root->next = (node *) arg->valNode;
	node *res = evaluateNode(root, map, macros);
	root->next = saved;
	deallocate_memory(arg);

	return res;
}

node * eval(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->valNode || root->next) { printf("Call to 'eval' must have exactly one list-argument.\n"); return NULL; }
	if (*((node *) root->valNode)->valNode != string) { printf("First element of the list must be function name.\n"); return NULL; }

	node func_name;
	func_name.id = fnc;
	func_name.valNode = ((node *) root->valNode)->valNode;
	func_name.next = ((node *) root->valNode)->next;

	return evaluateNode(&func_name, map, macros);
}

node * generic_map(node *root, hashset *map, hashset *macros)
{
	if (!root || !root->valNode || !root->next || !root->next->valNode)
		{ printf("Call to 'map' must have 2 or more arguments.\n"); return NULL; }

	if (*root->valNode == 2 && ((node *)root->valNode)->valNode && *((node *)root->valNode)->valNode == string &&
		strcmp((char *) (((node *)root->valNode)->valNode + 1), "lambda") == 0)
			return lambda(root, map, macros);

	node *curr;
	int count = 0;
	for (curr = root->next; curr != NULL; curr = curr->next)
	{
		if (*curr->valNode != qlst && *curr->valNode != fnc) { printf("Arguments must be lists.\n"); return NULL; }
		++count;
	}
	//printf("%d\n", count);

	if (count == 1)
		return unary_map(root, map, macros);
	return nnary_map(root, map, count, macros);
}



// not in map

void loadFunctions(hashset *map) {
	storeFuncIntoMap(map, "car",			car);
	storeFuncIntoMap(map, "cdr",			cdr);
	storeFuncIntoMap(map, "cadr",			cadr);
	storeFuncIntoMap(map, "cons", 			cons);
	storeFuncIntoMap(map, "list", 			list);
	storeFuncIntoMap(map, "append", 		append);
	storeFuncIntoMap(map, "+", 				plus);
	storeFuncIntoMap(map, "-", 				minus);
	storeFuncIntoMap(map, "*", 				multiply);
	storeFuncIntoMap(map, "/", 				divide);
	storeFuncIntoMap(map, "quotient", 		quotient);
	storeFuncIntoMap(map, "remainder", 		rest);
	storeFuncIntoMap(map, "positive?", 		isPositive);
	storeFuncIntoMap(map, "negative?", 		isNegative);
	storeFuncIntoMap(map, "zero?", 			isZero);
	storeFuncIntoMap(map, "odd?", 			isOdd);
	storeFuncIntoMap(map, "even?", 			isEven);
	storeFuncIntoMap(map, "sqrt", 			square_root);
	storeFuncIntoMap(map, "expt", 			expt);
	storeFuncIntoMap(map, "exp", 			neper);

	storeFuncIntoMap(map, "string=?", 		stringequals);
	storeFuncIntoMap(map, "string<?", 		stringless);
	storeFuncIntoMap(map, "string>?", 		stringgreater);
	storeFuncIntoMap(map, "string>=?", 		stringgreater_equal);
	storeFuncIntoMap(map, "string<=?", 		stringless_equal);
	storeFuncIntoMap(map, "string-ci=?", 	string_case_cmp);
	storeFuncIntoMap(map, "string-append", 	string_append);
	storeFuncIntoMap(map, "substring", 		substr);
	storeFuncIntoMap(map, ">", 				is_greater);
	storeFuncIntoMap(map, "<", 				is_less);
	storeFuncIntoMap(map, "=", 				is_equal);
	storeFuncIntoMap(map, "<=", 			is_less_or_equal);
	storeFuncIntoMap(map, ">=", 			is_greater_or_equal);
	storeFuncIntoMap(map, "equal?", 		generic_equal);
	storeFuncIntoMap(map, "list?", 			is_list);
	storeFuncIntoMap(map, "string?", 		is_string);
	storeFuncIntoMap(map, "char?", 			is_char);
	storeFuncIntoMap(map, "integer?", 		is_integer);
	storeFuncIntoMap(map, "real?", 			is_real);
	storeFuncIntoMap(map, "boolean?", 		is_bool);
	storeFuncIntoMap(map, "procedure?", 	is_procedure);
	storeFuncIntoMap(map, "and", 			and);
	storeFuncIntoMap(map, "or", 			or);
	storeFuncIntoMap(map, "not", 			not);
	storeFuncIntoMap(map, "null?", 			is_null);
	storeFuncIntoMap(map, "empty?", 		is_null);
	storeFuncIntoMap(map, "length", 		list_len);
	storeFuncIntoMap(map, "if", 			scm_if);
	storeFuncIntoMap(map, "cond", 			cond);
	storeFuncIntoMap(map, "let", 			let);

	storeFuncIntoMap(map, "apply", 			apply);
	storeFuncIntoMap(map, "eval", 			eval);
	storeFuncIntoMap(map, "map", 			generic_map);
}

void storeFuncIntoMap(hashset *map, char *fname, funcname fn)
{
	char *carStr = strdup(fname);
	char buffer[sizeof(char *)+sizeof(funcname)];
	memcpy(buffer, &carStr, sizeof(char *));
	memcpy(buffer + sizeof(char *), &fn, sizeof(funcname));
	HashSetEnter(map, buffer);
}

node * evaluateNode(node *root, hashset *map, hashset *macros)
{
	if (!root) return NULL;
	int type = root->id;
	//printf("%d\n", type);

	if (type == num || type == string || type == tof) {
		return wrapper((char *) root + sizeof(int), *(int *) root);
	} else if(*(int *) root == qlst) {
		return wrapper(root, *(int *) root);
	} else {
		char *func = (char *) (root->valNode) + sizeof(int);
		//printf("eval node for --> %s\n", func);

		//printf("evaling %s\n", func);
		funcname fn = getfunction(root, func, map, macros);
		if (!fn) return NULL;

		node *result;
		//if (b) result = fn(list, map, macros); // aq root rcheba haershi...
		result = fn(root->next, map, macros);

		return result;
	}
}

char * un_parse(node *root, hashset *map, hashset *macros, int caller_type)
{
	//printList(root);
	//getchar();
	char new_comm[1024]; new_comm[0] = '\0';
	int iter_num = 0;
	for (; root != NULL; root = root->next)
	{
		iter_num++;
		char arg[256]; arg[0] = '\0';
		if (!root->valNode) return strdup("'()) "); // i.e. empty list
		int type = *root->valNode;
	//	printf("%d\n", type);
		if (type == num) {
			double res = *(double *)(root->valNode + 1);
			int tmp = res;
			//printf("%d\n", tmp);
			if (tmp == res) sprintf(arg, "%d ", tmp);
			else sprintf(arg, "%f ", res);
		} else if (type == string) {
			if (caller_type != fnc || iter_num != 1) strcpy(arg, "\"");
			strcat(arg, (char *) (root->valNode + 1));
			if (caller_type != fnc || iter_num != 1) strcat(arg, "\" ");
			else strcat(arg, " ");
		} else if (type == fnc) {
			node *evaled = evaluateNode((node *) root->valNode, map, macros);
			//arg[0] = '(';
			strcpy(arg, un_parse(evaled, map, macros, fnc));
			deallocate_memory(evaled);
			//printf("22222 -> %s\n", arg);
			//strcat(arg, ") ");
		} else if (type == tof) {
			//printf("333\n");
			if (*(bool *) (root->valNode + 1))
				strcpy(arg, "#t ");
			else
				strcpy(arg, "#f ");
		} else if (type == qlst) {
			strcpy(arg, "'(");
			strcat(arg, un_parse((node *) root->valNode, map, macros, qlst));
			arg[strlen(arg)-1] = ')';
			strcat(arg, " ");
		}
		strcat(new_comm, arg);
	}
	//printf("new macro -> %s\n", new_comm);
	return strdup(new_comm);
}


node * wrapper(void *val, int type)
{
	if (type == qlst && !((node *)val)->valNode) return make_empty_list();
	
	node *res = malloc(sizeof(node));
	void *valNode;

	if (type == num) {
		valNode = malloc(sizeof(int) + sizeof(double));
		memcpy((char *) valNode + sizeof(int), val, sizeof(double));
	} else if (type == string) {
		valNode = malloc(sizeof(int) + strlen((char *) val) + 1);
		strcpy((char *) valNode + sizeof(int), (char *) val);
	} else  if (type == tof) {
		valNode = malloc(sizeof(int) + sizeof(bool));
		memcpy((char *) valNode + sizeof(int), val, sizeof(bool));		
	} else  if (type == qlst || type == fnc) {
		valNode = malloc(sizeof(node));
		memcpy(valNode, val, sizeof(node));
	}

	*(int *) valNode = type;

	res->next = NULL;
	res->id = qlst;
	res->valNode = valNode;
	//printf("wrapper : \n");
	//printList(res);

	return res;
}

int count_bytes(int *valNode)
{
	int size = sizeof(int);
	switch (*valNode) {
		case num:
			size += sizeof(double);
			break;
		case string:
			size += strlen((char *) (valNode + 1)) + 1;
			break;
		case fnc: case qlst:
			size += sizeof(node);
			break;
		case tof:
			size += sizeof(bool);
	}

	return size;
}

node * clone(node *a)
{
	if (!a) return NULL;
	node *res = malloc(sizeof(node));
	res->id = a->id;
	int size = count_bytes(a->valNode);
	res->valNode = (int *) malloc(size);
	memcpy(res->valNode, a->valNode, size);
	res->next = NULL;

	return res;
}

node * make_empty_list()
{
	node *rr = malloc(sizeof(node));
	rr->id = qlst;
	rr->valNode = NULL;
	rr->next = NULL;
	return rr;
}

/* generic for functions isNegative, isPositive and isZero */
node * operators_for_isfuncs(node *root, hashset *map, char ope, hashset *macros)
{
	node *wrapped = evaluateNode((node *) root->valNode, map, macros);
	if (!wrapped || *wrapped->valNode != num)
	{
		printf("Invalid type argument to boolean function.\n");
		return NULL;
	}

	int x = *(double *) (wrapped->valNode + 1);
	deallocate_memory(wrapped);

	bool b = false;
	if (ope == '>' && x > 0 || ope == '<' && x < 0 || ope == '=' && x == 0 ||
		ope == 'o' && x % 2 == 1 || ope == 'e' && x % 2 == 0) b = true;

	return wrapper(&b, tof);
}

/* generic for functions is_greater, is_less and is_equal */
node * operators_overload(node *root, hashset *map, char *ope, hashset *macros)
{
	bool b = true;
	node *fst = evaluateNode((node *) root->valNode, map, macros), *sec;
	if (!fst || *fst->valNode != num) { printf("Invalid type argument to compare operator.\n"); return NULL; }

	root = root->next;
	while (root)
	{
		sec = evaluateNode((node *) root->valNode, map, macros);
		if (!sec || *sec->valNode != num) { printf("Invalid type argument to compare operator.\n"); return NULL; }
		double f = *(double *) (fst->valNode + 1);
		double s = *(double *) (sec->valNode + 1);

		if (!strcmp(ope, ">") && f <= s || !strcmp(ope, "<") && f >= s || !strcmp(ope, "=") && f != s ||
			!strcmp(ope, "<=") && f > s || !strcmp(ope, ">=") && f < s) { b = false; break; }

		root = root->next;
		deallocate_memory(sec);
	//printf("%f -> %f -> %d\n", f, s, b);
	}
	deallocate_memory(fst);

	return wrapper(&b, tof);
}

node * list_equal(node *fst, node *sec)
{
	bool b = true;
	while (true)
	{
		if (!fst && !sec) return wrapper(&b, tof);
		if (fst && !sec || !fst && sec) { b = false; return wrapper(&b, tof); }
		int *val1 = fst->valNode, *val2 = sec->valNode;
		if (*val1 != *val2) { b = false; return wrapper(&b, tof); }

		int type = *val1;
		if (type == qlst) { return list_equal((node *) ((node *) val1)->valNode, (node *) ((node *) val2)->valNode); }

		if (type == num && *(double *) (val1 + 1) != *(double *) (val2 + 1)) { b = false; return wrapper(&b, tof); }
		if (type == string && strcmp((char *) (val1 + 1), (char *) (val2 + 1)) != 0) { b = false; return wrapper(&b, tof); }
		if (type == tof && *(val1 + 1) != *(val2 + 1)) { b = false; return wrapper(&b, tof); }

		fst = fst->next, sec = sec->next;
	}
}

/* generic for functions is_list, is_string, is_char, is_integer, is_real and is_procedure */
node * is(node *root, hashset *map, char aux, hashset *macros)
{
	if (!root || !root->valNode || root->next) { printf("That boolean function must have 1 argument.\n"); return NULL; }

	node *val = evaluateNode((node *) root->valNode, map, macros);
	if (!val) { printf("Invalid type argument.\n"); return NULL; }

	bool b = false;
	if ((aux == 'l' && ((!val->valNode && val->id == qlst) || *val->valNode == qlst)) || (aux == 's' && *val->valNode == string) || 
		(aux == 'c' && *val->valNode == string && strlen((char *)(val->valNode+1)) == 1) ||
		(aux == 'i' && *val->valNode == num && *(double *)(val->valNode+1) == floor(*(double *)(val->valNode+1))) ||
		(aux == 'r' && *val->valNode == num) || (aux == 'b' && *val->valNode == tof)) b = true;

	if (aux == 'p' && *val->valNode == string)
	{
		int len = strlen((char *)(val->valNode+1));
		char arr[len+1];
		strcpy(arr, (char *)(val->valNode+1));
		char *a = arr;
		if (strcmp(a, "if") != 0 && strcmp(a, "cond") != 0 && strcmp(a, "not") != 0 && strcmp(a, "or") != 0 &&
			strcmp(a, "and") != 0 && HashSetLookup(map, &a) != NULL) b = true;
	}
	deallocate_memory(val);

	return wrapper(&b, tof);
}

/* generic for and, or. */
node * bool_funcs(node *root, hashset *map, char aux, hashset *macros)
{
	if (!root || !root->valNode || !root->next || !root->next->valNode)
		return NULL;

	bool b = false;
	node *f = evaluateNode((node *) root->valNode, map, macros);
	if (!f || *f->valNode != tof) { printf("Error, not boolean type argument to boolean condition.\n"); return NULL; }
	bool fres = *(bool *)(f->valNode+1);
	if (aux == 'o' && fres) b = true;
	else
	{
		node *s = evaluateNode((node *) root->next->valNode, map, macros);
		if (!s || *s->valNode != tof) { printf("Error, not boolean type argument to boolean condition.\n"); return NULL; }

		bool sres = *(bool *)(s->valNode+1);

		if ((aux == 'a' && fres && sres) || (aux == 'o' && sres))
			b = true;
		deallocate_memory(s);
	}
	deallocate_memory(f);

	return wrapper(&b, tof);
}

node * string_cmp(node *root, hashset *map, hashset *macros, char *ope)
{
	if (!root || !root->next || root->next->next) { printf("string compare function must have 2 arguments.\n"); return NULL; }

	node *str_1 = evaluateNode((node *) root->valNode, map, macros);
	node *str_2 = evaluateNode((node *) root->next->valNode, map, macros);
	if (!str_1 || *str_1->valNode != string || !str_2 || *str_2->valNode != string) { printf("Invalid values to string compare.\n"); return NULL; }
	char *f = (char *) (str_1->valNode + 1);
	char *s = (char *) (str_2->valNode + 1);

	bool b = false;
	if ((!strcmp(ope, "=") && strcmp(f, s) == 0) || (!strcmp(ope, "<") && strcmp(f, s) < 0) || (!strcmp(ope, ">") && strcmp(f, s) > 0)
		|| (!strcmp(ope, "<=") && strcmp(f, s) <= 0) || (!strcmp(ope, ">=") && strcmp(f, s) >= 0)
		|| (!strcmp(ope, "case_ins") && strcasecmp(f, s) == 0)) b = true;

	deallocate_memory(str_1);
	deallocate_memory(str_2);

	return wrapper(&b, tof);
}

node * unary_map(node *root, hashset *map, hashset *macros)
{
	node *res = NULL, *curr = NULL;

	node *arg;
	if (*root->next->valNode == qlst) {
		arg = (node *) root->next->valNode;
	} else {
		node *tmp = evaluateNode((node *) root->next->valNode, map, macros);
		if (!tmp || *tmp->valNode != qlst) { printf("Call to map must have a list or lists as arguments.\n"); return NULL; }
		arg = (node *) tmp->valNode;
		free(tmp);
	}


	for (; arg != NULL; arg = arg->next)
	{
		root->next = arg;
		node *tt = arg->next;
		root->next->next = NULL;
		node *tmp = evaluateNode(root, map, macros);
		if (!tmp) { printf("Invalid type element to 'map' list.\n"); return NULL; }
		arg->next = tt;

		if (!res) {res = tmp; curr = tmp;}
		else {curr->next = tmp; curr = curr->next;}
	}
	return list(res, map, macros);
}

node * lambda(node *root, hashset *map, hashset *macros)
{
	if (root->next->next) { printf("call to 'lambda' has too many arguments.\n"); return NULL; }
	if (!((node *) root->valNode)->next || !((node *) root->valNode)->next->valNode ||
		!((node *) (((node *) root->valNode)->next->valNode))->valNode)
			{ printf("call to 'lambda' hasn't got an argument.\n"); return NULL; }

	if (!((node *)root->valNode)->next->next || !((node *)root->valNode)->next->next->valNode)
		{ printf("Invalid type argument to 'lambda'.\n"); return NULL; }

	char *arg_name = strdup((char *) (((node *) (((node *) root->valNode)->next->valNode))->valNode + 1));

/*** store pointers of nodes where will be written argument values ***/
	int **arr[36], idx = 0;
	node *key = (node*) ((node *)root->valNode)->next->next->valNode;
	for (; key != NULL; key = key->next)
	{
		if (!key->valNode) return NULL;
		if (*key->valNode == string && strcmp((char *)(key->valNode+1), arg_name) == 0)
			arr[idx++] = (int **) ((char *) key + sizeof(int));
	}
	free(arg_name);

	if (*root->next->valNode == fnc) {
		// full free of (node *) root->next->valNode
		node *tmp = evaluateNode((node *) root->next->valNode, map, macros);
		if (!tmp) { printf("Invalid type argument to 'map'.\n"); return NULL; }
		root->next->valNode = (int *) tmp->valNode;
		free(tmp);
	}
	
	if (!root->next->valNode || *root->next->valNode != qlst)
		{ printf("map argument must be a list.\n"); return NULL; }
	
/*** change argument nodes with argument values ***/
	node *curr_arg_root, *res = NULL, *curr_res = NULL;;
	for (curr_arg_root = (node *) root->next->valNode; curr_arg_root != NULL; curr_arg_root = curr_arg_root->next)
	{
		node *curr_arg = (node *) curr_arg_root->valNode;
		int i;
		for (i = 0; i < idx; ++i) {
			free(*arr[i]);
			*arr[i] = (int *) curr_arg;
		}
		// printList(root);
		// getchar();

		node *elem = evaluateNode((node *) ((node *)root->valNode)->next->next->valNode, map, macros);

		if (!res) {res = elem; curr_res = res;}
		else {curr_res->next = elem; curr_res = curr_res->next;}
	}

	return list(res, map, macros);
}

node * nnary_map(node *root, hashset *map, int numlists, hashset *macros)
{
	int i, j, k, s;
	int minlen = get_minlen_of_lists(root->next, map, macros);
	if (minlen == -1) return NULL;

	node * arr[numlists];

	for (k = 0; k < numlists; ++k)
	{
		arr[k] = root->next;
		for (s = 0; s < k; ++s)
			arr[k] = (node *) arr[k]->next;
		arr[k] = (node *) arr[k]->valNode;
	}

	node *final = NULL, *currAll = NULL;
	for (i = 0; i < minlen; ++i)
	{
		node *all = NULL, *curr = NULL;
		for (j = 0; j < numlists; ++j)
		{
			node *tmp = clone(arr[j]);

			if (!all) {all = tmp; curr = tmp;}
			else {curr->next = tmp; curr = curr->next;}
			arr[j] = arr[j]->next;
		}

		root->next = list(all, map, macros);
		if (!final) {final = apply(root, map, macros); currAll = final;}
		else {currAll->next = apply(root, map, macros); currAll = currAll->next;}
	}
	currAll->next = NULL;
	return list(final, map, macros);
}

funcname getfunction(node *root, char *func, hashset *map, hashset *macros)
{
	bool b = false;
	node *list;
	while (true) {
		void *macro_addr = HashSetLookup(macros, &func);
		if (macro_addr) {
			b = true;
			char *final_command = un_parse(root, map, macros, fnc);
			//printf("new macro command -> %s\n", final_command);
			final_command[strlen(final_command)-1] = '\0';			// removes extra space
			char command[1024]; command[0] = '('; command[1] = '\0';
			strcat(command, final_command);
			free(final_command);
			char name[128];
			int name_len = getFuncName(command, name);
			int res = replace_macro(macro_addr, name, command, &name_len);
			command[strlen(command)-1] = '\0';
			//printf("replaced macro -> %s\n", command);
			//getchar();
			if (res == -1) return NULL;
			func = name;
			list = parse(command + name_len + 2, fnc);
			//printList(list);
			//getchar();
		} else break;
	}
	if (b) { free(root->next); root->next = list; }
	//cprintf("%s\n", func);
	char *searchRes = (char *) HashSetLookup(map, &func);
	if (!searchRes) {printf("Function '%s' doesn't exists.\n", func); return NULL;}
	funcname fn = *(funcname *) (searchRes + sizeof(char *));

	return fn;
}

int get_minlen_of_lists(node *root, hashset *map, hashset *macros)
{
	int minlen = 0;

	for (; root != NULL; root = root->next)
	{
		if (*root->valNode == fnc)
		{
			node *tmp = evaluateNode((node *) root->valNode, map, macros);
			if (!tmp || *tmp->valNode != qlst) { printf("Invalid type argument to 'map'.\n"); return -1; }
			deallocate_memory((node *) root->valNode);
			root->valNode = tmp->valNode;
			free(tmp);
		}

		node *curr_sub_list = (node *) root->valNode;
		int currlen = 0;
		while (curr_sub_list)
		{
			++currlen;
			curr_sub_list = curr_sub_list->next;
		}
		if (currlen < minlen || minlen == 0) minlen = currlen;
	}
	return minlen;
}

void change(node *root, hashset *vars)
{
	//printf("sss %d\n", HashSetCount(vars));
	for (; root != NULL; root = root->next)
	{
		if (*root->valNode == string) {
			//printf("111\n");
			//printf("%s\n", (char *)(root->valNode+1));
			char *ptr = (char *)(root->valNode+1);
			void *res = HashSetLookup(vars, &ptr);
			if (res) {
				//printf("%s\n", ptr);
				//printf("%s\n", *((char **)res+1));
				int *valNode;
				if (isNumber(*((char **)res+1))) {
					//printf("%f\n", atof(*((char **)res+1)));
					valNode = malloc(sizeof(int) + sizeof(double));
					*valNode = num;
					double d = atof(*((char **)res+1));
					memcpy(valNode+1, &d, sizeof(double));
				} else if (isString(*((char **)res+1))) {
					valNode = malloc(sizeof(int) + strlen(*((char **)res+1)) + 1);
					*valNode = string;
					strcpy((char *) (valNode+1), *((char **)res+1));
				} else if (isBool(*((char **)res+1))) {
					valNode = malloc(sizeof(int) + sizeof(bool));
					*valNode = tof;
					if (strcmp(*((char **)res+1), "#t") == 0) *(bool *) (valNode + 1) = true;
					else *(bool *) (valNode + 1) = false;
				}
				deallocate_memory((node*)root->valNode);
				root->valNode = valNode;
			}
		} else if (*root->valNode == fnc) {
			change((node *) root->valNode, vars);
		}
	}
}

void deallocate_memory(node *root)
{
	while (root)
	{
		//printf("deallocating...\n");
		int type;
		if (root->id != qlst || root->id != fnc || !root->valNode) type = -1;	// empty list
		else type = *root->valNode;
		if (type == fnc || type == qlst)
			deallocate_memory((node *) root->valNode);
		else if (type != -1)
			free(root->valNode);

		node *to_delete = root;
		root = root->next;
		free(to_delete);
	}
}




/*********** J U N K ********************/


/*char * eval_node_for_string(node *root, hashset *map)
{
	if (*(int *) root == 1) {
		return (char *) root + sizeof(int);
	} else {
		char *func = (char *) (root->valNode) + sizeof(int);
		char *searchRes = (char *) HashSetLookup(map, &func);
		funcname fn = *(funcname *) (searchRes + sizeof(char *));
		node *result = fn(root->next, map);

		return (char *) (result->valNode + 1);
	}
}

node * evaluateNode(node *root, hashset *map)
{
	char *func = (char *) (root->valNode) + sizeof(int);
	printf("eval for quote --> %s\n", func);
	char *searchRes = (char *) HashSetLookup(map, &func);
	funcname fn = *(funcname *) (searchRes + sizeof(char *));
	node *result = fn(root->next, map);
	printf("end\n");

	return result;
}*/

/*double stringToInt(char *arr)
{
	int lc_count;
    int tmp=0;
    int array_mult = 1;
    int i = strlen(arr)-1;
     
    for(lc_count = 0; lc_count < strlen(arr); lc_count++) {
        tmp = tmp + (arr[i]-'0') * array_mult;
        array_mult *= 10;
        --i;
    }
    return tmp;

    void unparse(node *root, char *res)
{
	printf("11xsaxsa\n");
	if (!root) return;
	if (!root->valNode) {
		strcat(res, "()");
	} else {
		int type = *(root->valNode);
		if (type == 0) {
			double val = *(double *)(root->valNode + 1);
			int tmp = val;
			if (tmp == val) {printf("ddd\n"); strcat(res, " "+tmp); printf("%s\n", res);}
			else
			{
				char arr[80];
				sprintf(arr, "%f", val);
				strcat(res, arr);
			}
			printf("eeend\n");
		} else if (type == 1) {
			printf("sssttt\n");
			strcat(res, (char *)(root->valNode + 1));
		} else if (type == 2 || type == 4) {
			printf("lissss\n");
			strcat(res, "(");
		 	unparse((node *) (root->valNode), res);
		 	strcat(res, "");
		} else if (type == 3) {
			if (*(bool *)(root->valNode + 1) == true) strcat(res, "#t");
			else strcat(res, "#f");
		} //else if (type = 4) printList((node *) (root->valNode));	// should be printed as one sequance
	}
	if (root->next) strcat(res, " ");	// if not last elem from list
	unparse(root->next, res);
}


}*/