#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "map.h"
#ifndef common_h
#define common_h
#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct Persist Persist;
struct Persist {
    FILE* fp;
    char* name;
};

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

typedef struct Object Object;
struct Object {
    int size;
    char* bytes;
};

typedef struct HashMap HashMap;
struct HashMap {
    LinkedEntries** entries;
    int size;
    int count;
};

typedef struct PersistedMap PersistedMap;
struct PersistedMap {
    Persist* persist;
    HashMap* map;
    int mapsize;
};

typedef struct client_args client_args;
struct client_args {
    
    PersistedMap** pm;
    int socketfd;
    int mapsize;
};


#endif