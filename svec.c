// CREDIT:
/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */
// modified to implement split, remove, print

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "svec.h"

// similar to calc code 05 05 lecture credit nat tuck, but modified for svec
svec*
svec_split(svec* sv, int i0, int i1)
{
	// inclusive, exclusive
	assert(i0 >= 0 && i1 <= sv->size);
	svec* ys = make_svec();
	for (int ii = i0; ii < i1; ++ii) {
		svec_push_back(ys, svec_get(sv, ii));
	}
	return ys;
}

svec*
make_svec()
{
	svec* sv = calloc(1, sizeof(svec));	
	sv->capacity = 4;	
    sv->data = calloc(sv->capacity, sizeof(char*));
	sv->size = 0;
	return sv;
}

// we remove at index ii, and any elements at
// index > ii are moved up to fill the space
void
svec_remove(svec* sv, int ii)
{
	assert(ii >= 0 && ii < sv->size);
	free(sv->data[ii]);
	sv->size -= 1;
	for (int xx = ii; xx < sv->size; ++xx) {
		svec_put(sv, xx, sv->data[xx+1]);
	}
}

void
svec_print(svec* sv)
{
	for (int ii = 0; ii < sv->size+1; ++ii) {
		printf("[%s]\n", sv->data[ii]);
	}
}

void
free_svec(svec* sv)
{
	// free each char* in array
	for (long ii = 0; ii < sv->size; ++ii) {
		free(sv->data[ii]);
	}
	free(sv->data);
	free(sv);
	
}

char*
svec_get(svec* sv, int ii)
{
	assert(ii >= 0 && ii < sv->size);
    char* str = sv->data[ii];
	return str;
}

void
svec_put(svec* sv, int ii, char* item)
{
	assert(ii >= 0 && ii < sv->size);
	free(sv->data[ii]);
    sv->data[ii] = strdup(item);
}

void
svec_push_back(svec* sv, char* item)
{
    int ii = sv->size;
	int cc = sv->capacity;
	// double capacity and realloc if size is not large enough
	if (ii >= cc) {
		sv->capacity = cc * 2;
    	sv->data = realloc(sv->data, sv->capacity * sizeof(char*));
	}
    sv->size = ii + 1;
    svec_put(sv, ii, item);
}

void
svec_swap(svec* sv, int ii, int jj)
{
	char* temp = sv->data[jj];
	sv->data[jj] = sv->data[ii];
	sv->data[ii] = temp;
}
