// functions modified for shell, but much of this was written following
// all to calc lecture video: credit Nat Tuck
// URL: https://github.com/NatTuck/scratch-2020-09/blob/682c8a64774068b0f8938d46b54300b2724b3a90/3650/v05/calc/ast.c

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "svec.h"

// returns index of any of the operators or -1 if not found
int
find_op(svec* sv)
{	
	for (int ii = 0; ii < sv->size; ++ii){
		if (strcmp(svec_get(sv, ii), ";") == 0) {
			return ii;
		}
		else if (strcmp(svec_get(sv, ii), "&&") == 0) {
			return ii;
		}
		else if (strcmp(svec_get(sv, ii), "||") == 0) {
			return ii;
		}
		else if (strcmp(svec_get(sv, ii), ">") == 0) {
			return ii;
		}
		else if (strcmp(svec_get(sv, ii), "<") == 0) {
			return ii;
		}
		else if (strcmp(svec_get(sv, ii), "|") == 0) {
			return ii;
		}
		else {
			continue;
		}
	}
	return -1;
}

// credit for simliar parsing strategy (modified): Nat Tuck
// URL: https://github.com/NatTuck/scratch-2020-09/blob/682c8a64774068b0f8938d46b54300b2724b3a90/3650/v05/calc/parse.c
ast*
parse(svec* sv)
{
	int op_index = find_op(sv);
	// just a command, can't find operator in token list
	// base case:
	if (sv->size == 1 || op_index == -1) {
		return make_ast_cmd(sv);
	}
	else if (op_index != -1){
		svec* l = svec_split(sv, 0, op_index);
		svec* r = svec_split(sv, op_index + 1, sv->size);
		ast* at = make_ast_op(sv->data[op_index], parse(l), parse(r));
		return at;
	}
	else {
		fprintf(stderr, "invalid token stream");
		exit(1);
	}
}

// construct leaf
// credit (modified): Nat Tuck
// ast.c URL top of file
ast*
make_ast_cmd(svec* c)
{
    ast* at = malloc(sizeof(ast));
    at->op = "="; // if operation is = this is a single expression
    at->cmd = c;
    at->left = 0;
    at->right = 0;
    return at;
}

// construct node
// credit (modified): Nat Tuck
// ast.c URL top of file
ast*
make_ast_op(char* op, ast* a0, ast* a1)
{
    ast* at = malloc(sizeof(ast));
    at->op = strdup(op);
	at->cmd = 0;
	at->left = a0;
    at->right = a1;
    return at;
}

// credit (modified): Nat Tuck
// ast.c URL top of file
void
free_ast(ast* at)
{
	if (at) {
		// will not call strdup in base case
		// so nothing to free
		if (strcmp(at->op, "=") != 0) {
			free(at->op);
		}
		if (at->cmd){
			free_svec(at->cmd);
		}
		if (at->left){
			free_ast(at->left);
		}
		if (at->right){
			free_ast(at->right);
		}
		free(at);
    }
}

// Used only for debugging
// takes up a lot of vertical space
void
ast_print(ast* at)
{
    if (strcmp(at->op, "=") == 0) {
        printf("\ncmd=\"");
		svec_print(at->cmd);
		printf("\"\n");
		printf("ast->left=%d\n", at->left);
		printf("ast->right=%d\n\n", at->right);
    }
    else {
		printf("left\n{\n");
		ast_print(at->left);
		printf("}\n");
		printf("\noperator=%s\n\n", at->op);
		printf("right\n{\n");
		ast_print(at->right);
   		printf("}\n"); 
	}
}

