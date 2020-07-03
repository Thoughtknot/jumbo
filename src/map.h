#ifndef mapheader
#define mapheader
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "common.h"

typedef struct LinkedEntries LinkedEntries;

struct LinkedEntries {
    KeyValue* entry; 
    LinkedEntries* next;
};

typedef struct HashMap HashMap;
 
// Define the Hash Table here
struct HashMap {
    LinkedEntries** entries;
    int size;
    int count;
};

void put(HashMap* table, int keyLen, char* key, int valLen, char* value);
void del(HashMap* table, int keyLen, char* key);
Value* get(HashMap* table, int keyLen, char* key);
Key** keys(HashMap* map, int limit);

HashMap* create_map(int size);
void free_map(HashMap* map);
#endif