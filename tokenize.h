#ifndef TOKENIZE_H
#define TOKENIZE_H

#include "svec.h"

svec* tokenize(const char* line);
void print_tokens(svec* sv);

#endif
