#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <bsd/string.h>
#include <string.h>

#include "hashmap.h"

hashmap_pair
make_hashmap_pair(char* key, int value)
{
	hashmap_pair hp;
	strlcpy(hp.key, key, sizeof(hp.key)); // size = 4
	hp.val = value;
	hp.used = 0;
	hp.tomb = 0;
	return hp;
}

int
hash(char* key, int capacity)
{
	// attribute prof nat lecture video 05 02 hash table
	// capacity must be power of two
	int hash = 0;
	for (int ii = 0; key[ii]; ++ii) {
		hash = hash * 67 + key[ii];
	}
	return hash & (capacity - 1);
}

hashmap*
make_hashmap_presize(int nn)
{
    hashmap* hh = calloc(1, sizeof(hashmap));
    hh->capacity = nn;
	hh->data = calloc(hh->capacity, sizeof(hashmap_pair));
	hh->size = 0;
	return hh;
}

hashmap*
make_hashmap()
{
    return make_hashmap_presize(4);
}

void
free_hashmap(hashmap* hh)
{
	free(hh->data);
	free(hh);
}

int
hashmap_has(hashmap* hh, char* kk)
{
    return hashmap_get(hh, kk) != -1;
}

int
hashmap_get(hashmap* hh, char* kk)
{
    // Note: return -1 for key not found.
	int index = hash(kk, hh->capacity);
    // for loop ensures we look at, at most hh->size elements
	for(int ii = 0; ii < hh->size; ++ii) {
		// getting to empty slot means no match
		if (hh->data[index].used == 0){
			return -1;
		}
		// found the correct key
		if (strcmp(hh->data[index].key, kk) == 0){
			return hh->data[index].val;
		}
		// check if at the end
		if (index + 1 >= hh->size) {
			index = 0;
		}
		else {
			index = index + 1;
		}
	}
	return -1;
}

void
hashmap_grow(hashmap* hh)
{
	// save old array and capacity
	int nn = hh->capacity;
	hashmap_pair* data = hh->data;

	// new in same place, lecture 05 02
	hh->capacity = 2 * nn;
	hh->data = calloc(hh->capacity, sizeof(hashmap_pair));
	hh->size = 0;

	// copy elements over
	for (int ii = 0; ii < nn; ++ii) {
		hashmap_pair curr = data[ii];
		if (strcmp(curr.key,"")){
			hashmap_put(hh, curr.key, curr.val);
		}
	}
	free(data);
}

void
hashmap_put(hashmap* hh, char* kk, int vv)
{
	// load factor 0.5
	//printf("%ld >= %ld\n", hh->size, hh->capacity/2);
	if (hh->size >= ((hh->capacity) / 2)){
		hashmap_grow(hh);
	}
	int hv = hash(kk, hh->capacity);
	while (1) {
		// if position is tomb or unused, simply insert
		if (hh->data[hv].tomb || !(hh->data[hv].used)) {
			hh->data[hv] = make_hashmap_pair(kk,vv);
			hh->data[hv].used = 1;
			hh->size = hh->size + 1;
			break;
		}
		// if key exists, just update value
		else if (strcmp(hh->data[hv].key, kk) == 0) {
			hh->data[hv].val = vv;
			break;
		}
		else {
			if (hv + 1 == hh->capacity) {
				hv = 0;
			}
			else {
				hv = hv + 1;
			}
		}
	}
}

void
hashmap_del(hashmap* hh, char* kk)
{
    // Remove any value associated with
    // this key in the map.
	int index = hash(kk, hh->capacity);
	
	// start searching at hash of key
	int hv = hash(kk, hh->size);
	for (int ii = 0; ii < hh->capacity;++ii){
		// finding empty slot exits
		if (!hh->data[hv].used) {
			break;
		}
		if (strcmp(hh->data[hv].key, kk) == 0) {
			hh->data[index].used = 1;
			hh->data[index].tomb = 1;
			hh->data[index].val = 0;
			break;
		}// go to next item
		else {
			if (hv + 1 == hh->capacity) {
				hv = 0;
			}
			else{
				hv = hv + 1;
			}
		}
	}	
}

hashmap_pair
hashmap_get_pair(hashmap* hh, int ii)
{
    // Get the {k,v} pair stored in index 'ii'.
	return hh->data[ii];
}

void
hashmap_dump(hashmap* hh)
{
    printf("== hashmap dump ==\n");
    printf("size=%d\tcapacity=%di\n", hh->size, hh->capacity);
	// Print out all the keys and values currently
    // in the map, in storage order. Useful for debugging.
	for (int ii=0; ii < hh->capacity; ++ii) {
		hashmap_pair hp = hashmap_get_pair(hh, ii);
		printf("value: %d\t\tkey: %s", hp.val, hp.key);
		if (hp.tomb == 1) {
			printf("\tTOMB");
		}
		if (hp.used == 1) {
			printf("\tUSED");
		}
		printf("\n");
	}
}

