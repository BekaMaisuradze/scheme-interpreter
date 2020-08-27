#include "kawa.h"


static const signed long kHashMultiplier = -1664117991L;
int StringHash(const void *elem, int numBuckets)
{
	char *s = *(char **) elem;
	unsigned long hashcode = 0;
	int i;
	for (i = 0; i < strlen(s); i++)  
    	hashcode = hashcode * kHashMultiplier + tolower(s[i]);  
	return hashcode % numBuckets;                                  
}

/* Dealocates dinamically created C string */
void StringFree(void *str)
{
	free(*(char **)str);
}

void two_str_free(void *buff)
{
	free(*(char **) buff);
	free(*((char **) buff + 1));
}

/* Compare function for hashset, compares keys as c-strings [uses strcasecmp] */
int StringCmp(const void *elemAddr1, const void *elemAddr2)
{
	return strcasecmp(*(char **) elemAddr1, *(char **) elemAddr2);
}

static int ProceduresCmp(const void *elemAddr1, const void *elemAddr2)
{
	char * macro = *(char **) elemAddr1, *sec = *(char **) elemAddr2;

	int i;
	for (i = 0; i < strlen(macro) && i < strlen(sec); ++i)
	{
		if (macro[i] == ' ') break;
		if (macro[i] != sec[i]) return -1;
	}
	return 0;
}

static int procHash(const void *elem, int numBuckets)
{
	char *s = *(char **) elem;
	unsigned long hashcode = 0;
	int i;
	for (i = 0; i < strlen(s); i++) {
		if (s[i] == ' ') break;
    	hashcode = hashcode * kHashMultiplier + tolower(s[i]);  
	}
	return hashcode % numBuckets;                                  
}

int comparator(const void *elemAddr1, const void *elemAddr2)
{
	return strcmp((char *) elemAddr1, (char *) elemAddr2);
}

bool isNotEvaluableList(char *str)
{
	return str[0] == '\'';
}

bool isNumber(char *str)
{
	int i = 0;
	if (str[0] == '-') {
		if (strlen(str) == 1 || str[1] == ' ') return false;
		else ++i;
	}
	for (; i<strlen(str); ++i)
		if (!isdigit(str[i]) && str[i] != '.' && str[i] != ' ') return false;

	return true;
}

bool isString(char *str)
{
	if (str[0] == '"' && str[strlen(str)-1] == '"') return true;

	return false;
}

bool isBool(char *str)
{
	return str[0] == '#' && (str[1] == 't' || str[1] == 'f');
}

void print_double_nontrailing_zeroes(double d)
{
	char n[50];
	sprintf(n, "%f", d);
	int i = strlen(n)-1;
	while (i) {
		if (n[i] == '0')
			--i;
		else break;
	}
	int k;
	for (k = 0; k <= i; k++)
		printf("%c", n[k]);
}

void cut_comments(char *line)
{
	int k;
	for (k = 0; k < strlen(line); ++k)
	{
		if (line[k] == ';')
		{
			line[k] = '\0';
			break;
		}
	}
}

int count_front_spaces(char *command)
{
	int i;
	for (i = 0; i < strlen(command); i++)
		if (command[i] != ' ') break;

	return i;
}

int commands_real_len(char *command)
{
	int i;
	for (i = strlen(command) - 1; i >= 0; i--)
		if (command[i] != ' ') break;

	return i;
}

int check_braces(char *str)
{
	int i, count = 0;

	for (i = 0; i < strlen(str); ++i)
	{
		if (str[i] == '(') ++count;
		else if (str[i] == ')') --count;
	}
	return count;
}

int getFuncName(char *str, char *res)
{
	int i = 1, mem = 0;
	bool found = false;
	for (; i < strlen(str)-1; ++i)
	{
		if (str[i] == ' ' && found)	break;

		else if (str[i] != ' ')
		{
			res[mem++] = str[i];
			if (!found) found = true;
		}
	}
	res[mem] = '\0';
	return i-1;
}

void save_macro(char *command, hashset *macros)
{
	//printf("%s\n", command);

	char tmp[256];
	int i;
	for (i = 1; i < strlen(command); i++)
	{
		if (command[i] == ')') break;
		tmp[i-1] = command[i];
	}
	tmp[i-1] = '\0';
	//printf("%s\n", strdup(tmp));

	char *name_and_arg = strdup(tmp);
	char *procedure_body = strdup(command + strlen(name_and_arg) + 3);	// 2 for (), and 1 for ' '
	//printf("%s  --  %s\n", name_and_arg, procedure_body);

	char buffer[2 * sizeof(char **)];
	memcpy(buffer, &name_and_arg, sizeof(char **));
	memcpy(buffer + sizeof(char **), &procedure_body, sizeof(char **));
	HashSetEnter(macros, buffer);
}

funcname searchFn(hashset *map, char *name)
{
	void *elem = HashSetLookup(map, &name);
	if (!elem) return elem;	// i.e. NULL
	//printf("%s\n", *(char **) elem);

	funcname fn = *(funcname *) ((char *) elem + sizeof(char **));
	//printf("%d\n", *((int *) elem + 1));
	return fn;
}

int getAllTokens(char *src, vector *tokens)
{
	int i = 0;
	while (i < strlen(src)) {
		char res[1024];
		int tmp = 0;
		//if (src[i] == '\'') ++i;
		if (src[i] == '(') {
			int count = 0;
			for (; i<strlen(src); ++i)
			{
				if (src[i] == '(') count++;
				else if (src[i] == ')') --count;
				res[tmp++] = src[i];
				if (count == 0) {i += 2; break;}
			}
		} else if (src[i] == '\'') {
			if (src[i+1] != '(') { ++i; continue; }
			res[tmp++] = src[i++];
			//res[tmp++] = src[i++];
			int count = 0;
			for (; i<strlen(src); ++i)
			{
				if (src[i] == '(') {count++; if (src[i-1] != '\'') res[tmp++] = '\'';}
				else if (src[i] == ')') --count;
				res[tmp++] = src[i];
				if (count == 0) {i += 2; break;}
			}
		} else if (src[i] == '"') {
			res[tmp++] = '"';
			++i;
			for (; i<strlen(src); ++i)
			{
				if (src[i] == '"') {res[tmp++] = '"'; break;}
				res[tmp++] = src[i];
			}
			if (src[++i] != ' ' && i != strlen(src)) return -1;
			++i; // to ignore comming space
		} else {
			for (; i<strlen(src); ++i)
			{
				if (src[i] == ' ') {++i; break;}
				res[tmp++] = src[i];
			}
		}

		if (tmp != 0) {
			res[tmp] = '\0';
			char *r = strdup(res);
			// printf("token -> %s\n", r);
			VectorAppend(tokens, &r);
		}
	}
	return 1;	// success
}

int replace_macro(void *macro_addr, char *name, char *final_command, int *name_len)
{
	char *head = *(char **) macro_addr;
	char *body = *((char **) macro_addr + 1);
	//if (check_braces(final_command + *name_len + 2) < 0)	// tu vailma gamoidzaxa meored an metjer zedized
	//	final_command[strlen(final_command)-1] = '\0';

	vector args, called_args;
	VectorNew(&args, sizeof(char *), NULL, 50);
	VectorNew(&called_args, sizeof(char *), NULL, 50);
	int res = getAllTokens(head, &args);
	int res2 = getAllTokens(final_command + *name_len + 2, &called_args);
	if (res < 0 || res2 < 0) {printf("parsing error\n"); return -1;}

	int args_num = VectorLength(&args);
	int args_num_2 = VectorLength(&called_args);
	//printf("args numm: %d -> %d\n", args_num, args_num_2);
	if (args_num != args_num_2 + 1) {printf("wrong call...\n"); return -1;}


	char new_command[2048]; new_command[0] = '\0';

	char token[1024]; token[0] = '\0';
	char *ptr = token;
	int i;
	for (i = 0; i < strlen(body); ++i)
	{
		char c = body[i];
		if (c != ' ' && c != '(' && c != ')') {
			char cc[2];
			cc[0] = body[i];
			cc[1] = '\0';
			strcat(token, cc);
		} else {
			int idx = VectorSearch(&args, &ptr, StringCmp, 1, false); // ignore func name
			if (idx != -1)
				strcat(new_command, *(char **)(VectorNth(&called_args, idx-1)));
			else
				strcat(new_command, token);

			token[0] = body[i];
			token[1] = '\0';
			strcat(new_command, token);
			token[0] = '\0';
		}
	}
	*name_len = getFuncName(new_command, name);
	strcpy(final_command, new_command);
	//printf("%s -->> %s\n", name, final_command);
//	getchar();
	return 1;
}

void printList(node *root)
{
	if (!root) return;
	if (!root->valNode) {
		printf("()");
	} else {
		int type = *(root->valNode);
		if (type == num) {
			double res = *(double *)(root->valNode + 1);
			int tmp = res;
			if (tmp == res) printf("%d", tmp);
			else print_double_nontrailing_zeroes(res);
		} else if (type == string) {
			printf("%s", (char *)(root->valNode + 1));
		} else if (type == fnc || type == qlst) {
			printf("(");
		 	printList((node *) (root->valNode));
		 	printf(")");
		} else if (type == tof) {
			if (*(bool *)(root->valNode + 1) == true) printf("#t");
			else printf("#f");
		}
	}
	if (root->next) printf(" ");	// if not last elem from list
	printList(root->next);
}

node * parse(char *list, int l)
{
	//printf("parsing -> %s\n", list);
	vector tokens;
	VectorNew(&tokens, sizeof(char *), NULL, 50);
	int result = getAllTokens(list, &tokens);
	if (result < 0) {printf("parsing error\n"); return NULL;}
	node *root = (node *) malloc(sizeof(node));
	node *curr = root;
	//char delim[3] = " '";
	//char *token = nextToken(list);

	int i, len = VectorLength(&tokens);
	for (i = 0; i < len; ++i)
	{
		//if (token[strlen(token)-1] == ')') token[strlen(token)-1] = '\0';
		char *token = *((char **)(VectorNth(&tokens, i)));
		void * val;

		if (isNumber(token)) {
			val = malloc(sizeof(int) + sizeof(double));
			*(int *)val = num;
			*(double *)((char *) val + sizeof(int)) = atof(token);
		} else if (token[0] == '(') {
			token[strlen(token)-1] = '\0';
			val = malloc(sizeof(node *));
			val = parse(token+1, l);
		} else if (isBool(token)) {
			val = malloc(sizeof(int) + sizeof(bool));
			*(int *)val = tof;
			if (strcmp(token, "#t") == 0) *(bool *) ((int *) val + 1) = true;
			else *(bool *) ((int *) val + 1) = false;
			//strcpy((char *) val + sizeof(int), token);
		} else if(token[0] == '\'') {
			if (strlen(token) <= 3) {			// i.e. empty '()
				val = make_empty_list();
			} else {
				token[strlen(token)-1] = '\0';
				val = malloc(sizeof(node *));
				val = parse(token+2, qlst);
			}
		} else {
			//if (token[0] == '"') token = token + 1;
			val = malloc(sizeof(int) + strlen(token) + 1); // extra " will go for '\0'
			*(int *)val = string;
			if (token[strlen(token)-1] == '"') { token[strlen(token)-1] = '\0'; strcpy((char *) val + sizeof(int), token+1); }
			else strcpy((char *) val + sizeof(int), token);
		}


		curr->id = l;
		curr->valNode = (int *) val;

		if (i == VectorLength(&tokens) - 1) curr->next = NULL;
		else { curr->next = (node *) malloc(sizeof(node)); curr = curr->next; }
	}
	return root;
}


void EVAL(node *list, funcname fn, hashset *map, hashset *macros);
int main()
{
	hashset map, macros;
	HashSetNew(&map, sizeof(char **) + sizeof(funcname), 97, StringHash, StringCmp, StringFree);
	HashSetNew(&macros, 2 * sizeof(char **), 13, procHash, ProceduresCmp, two_str_free);
	loadFunctions(&map);
	int lines_num = 0;

	while (true)
	{
		char command[2048];
		command[0] = '\0';	// for every new iteration
		int braces = 0;
		while (true)	// get final command
		{
			char line[256]; line[0] = '\0';
			printf("#|kawa:%d|# ", ++lines_num);
			fgets(line, 256, stdin);
			line[strlen(line)-1] = ' '; 	// space instead of extra symbol ('\n') put by 'fgets' at the very end of the command
			cut_comments(line);

			braces += check_braces(line);
			if (braces < 0) {
				printf("Error, check braces correction.\n");
				break;
			}
			strcat(command, line);
			if (braces == 0) break;
		}
		if (braces == -1) continue;

		/*** start corrections ***/
		int front_spaces = count_front_spaces(command);
		char *final_command = command + front_spaces;	// cut front spaces of command

		int command_len = commands_real_len(final_command);
		final_command[command_len+1] = '\0';	// cut end spaces of command
		/*** end corrections ***/

		if (!strcmp(final_command, "(exit)")) break;
		if (isNumber(final_command) || isBool(final_command)) {
			printf("%s\n", final_command);

		} else if (isString(final_command)) {
			final_command[strlen(final_command)-1] = '\0';
			printf("%s\n", final_command + 1);

		} else if (isNotEvaluableList(final_command)) {
			printf("%s\n", final_command + 1);
			
		} else if (final_command[0] == '(') {
			int index;
			char name[128]; name[0] = '\0';
			int name_len = getFuncName(final_command, name);
			//	printf("%d\n", name_len);
			final_command[strlen(final_command)-1] = '\0';	// for last )


			if (!strcmp(name, "define")) {
				save_macro(final_command + name_len + 2, &macros);

			} else {
				bool replace_macro_succes = true;
				while (true) {
					char *ptr = name;
					void *macro_addr = HashSetLookup(&macros, &ptr);
					if (macro_addr) {
						int res = replace_macro(macro_addr, name, final_command, &name_len);
						final_command[strlen(final_command)-1] = '\0';
						if (res == -1) { replace_macro_succes = false; break; }
					} else break;
				}
				if (!replace_macro_succes) continue;

				funcname fn = searchFn(&map, name);
				if (fn)
				{
					node *list = parse(final_command + name_len + 2, fnc);
					if (list != NULL) EVAL(list, fn, &map, &macros);
					deallocate_memory(list);
				} else printf("%s function not found\n", name);
			}
		}
	}

	HashSetDispose(&map);
	HashSetDispose(&macros);

	return 0;
}

void EVAL(node *list, funcname fn, hashset *map, hashset *macros)
{
	node *result = fn(list, map, macros);
	printList(result);
	printf("\n");
	deallocate_memory(result);
}