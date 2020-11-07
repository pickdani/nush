// my tokenizer from hw5 
// only modified to seperate on more symbols

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenize.h"
#include "svec.h"

void
print_tokens(svec* sv)
{
	printf("tokens: ");
	for (int ii = 0; ii < sv->size+1; ++ii) {
		printf("[%s]", sv->data[ii]);
	}
	printf("\n");
}
	
char*
read_token(const char* text, long ii)
{
	int nn = 0;
	// stop at space, |, & these indicate start of a new token
	if (text[ii + nn]){
		while(!isspace(text[ii + nn])
				&& text[ii + nn] != ';'
				&& text[ii + nn] != '|'
				&& text[ii + nn] != '&'
				&& text[ii + nn] != '<'
				&& text[ii + nn] != '>'
				&& text[ii + nn] != ')'
				&& text[ii + nn] != '('
				&& text[ii + nn] != '$'
				&& text[ii + nn] != '\"') {
			++nn;
		}
	}
	char* token = malloc(nn + 1);
	memcpy(token, text + ii, nn);
	token[nn] = 0;
	return token;
}

svec*
tokenize(const char* line) 
{
	svec* tokens = make_svec(); // empty linked list to store tokens
	int nn = strlen(line);
	for (int ii = 0; ii < nn;) {
		// ignore spaces, (and groups of spaces)
		if (isspace(line[ii])) {
			++ii;
			continue;
		}
		// check for these single char tokens
		else if (line[ii] == ';' || line[ii] == '&' || line[ii] == '|'
				|| line[ii] == '\"' || line[ii] == '<' || line[ii] == '>'
				|| line[ii] == '(' || line[ii] == ')' || line[ii] == '$') {
			// check for double operators ||, &&
			if (ii + 1 < nn) {
				if (line[ii + 1] == line[ii]) {
					char token[4] = "xx";
					token[0] = line[ii];
					token[1] = line[ii];
					svec_push_back(tokens,token);
					ii += 2;
					continue;
				}
			}
			// must be single char
			char token[4] = "x";
			token[0] = line[ii];
			svec_push_back(tokens, token);
			++ii;
			continue;
		}
		else {
			char* token = read_token(line, ii);
			svec_push_back(tokens, token);
			ii += strlen(token);
			free(token);
			continue;
		}
	}
	return tokens;
}
