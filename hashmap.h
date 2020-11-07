// file from hw04 submission
#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>

typedef struct hashmap_pair {
    char key[4]; // null terminated strings
    int  val;
    bool used; // indicates if this is used storing val or empty
    bool tomb; // indicates it is deleted
} hashmap_pair;

typedef struct hashmap {
   hashmap_pair* data; // pointer to array of hashmap_pair
   int capacity; // invariant: load factor 0.5
   int size; // size <= capacity / 2
} hashmap;

hashmap* make_hashmap();
void free_hashmap(hashmap* hh);
int hashmap_has(hashmap* hh, char* kk);
int hashmap_get(hashmap* hh, char* kk);
void hashmap_put(hashmap* hh, char* kk, int vv); 
void hashmap_del(hashmap* hh, char* kk);
hashmap_pair hashmap_get_pair(hashmap* hh, int ii);
void hashmap_dump(hashmap* hh);

#endif
