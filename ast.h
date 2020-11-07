// credit for tree struct (modified): Nat Tuck
// URL: https://github.com/NatTuck/scratch-2020-09/blob/682c8a64774068b0f8938d46b54300b2724b3a90/3650/v05/calc/ast.h

#ifndef AST_H
#define AST_H

#include "svec.h"

// tree has 7 operators, to left and right of operators are commands
// therefore evaluating command is a leaf to ast
typedef struct ast {
    char* op; // one of: {"&&", "||", "&", "|", "<", ">", ";"}
			  //     or: "=" for single command
    svec* cmd; // expression for evaulting
	// rest of tree or null
	struct ast* left;
    struct ast* right;
} ast;

ast* parse(svec* tok);
ast* make_ast_cmd(svec* cmd);
ast* make_ast_op(char* op, ast* a0, ast* a1);
void free_ast(ast* ast);

int ast_eval(ast* ast);
void ast_print(ast* ast);

#endif
