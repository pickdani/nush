#ifndef NUSH_H
#define NUSH_H

#include "nush.h"

int execute(svec* cmd);
int redir_in(ast* at);
int redir_out(ast* at);
int exec_semi(ast* at);
int exec_and(ast* at);
int exec_or(ast* at);
int exec_pipe(ast* at);

#endif
