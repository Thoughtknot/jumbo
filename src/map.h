#ifndef mapheader
#define mapheader
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

typedef struct Key Key;
 
struct Key {
    int keySize;
    char* key;
};

typedef struct Value Value;
 
struct Value {
    int valueSize;
    char* value;
};

typedef struct KeyValue KeyValue;
 
struct KeyValue {
    Key* key;
    Value* value;
};

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

HashMap* create_map(int size);
void free_map(HashMap* map);
#endif