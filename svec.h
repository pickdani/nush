// CREDIT:
/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */
// (modified) to add print, remove, split
#ifndef SVEC_H
#define SVEC_H

typedef struct svec {
    char** data;
	int capacity;
	int size;
} svec;

svec* make_svec();
void free_svec(svec* sv);
void svec_remove(svec* sv, int ii);
void svec_print(svec* sv);

char* svec_get(svec* sv, int ii);
void  svec_put(svec* sv, int ii, char* item);

void svec_push_back(svec* sv, char* item);
void svec_swap(svec* sv, int ii, int jj);
svec* svec_split(svec* sv, int ii, int jj);

#endif
