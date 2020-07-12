#ifndef persistedmapheader
#define persistedmapheader
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "common.h"
#include "map.h"
#include "persist.h"
#include <dirent.h>
#include <unistd.h> 

PersistedMap** build_maps(int size);
void free_maps(PersistedMap** map, int size);
void *handle_client_socket(void * args);
PersistedMap* getTable(int table, PersistedMap** pm, int mapsize);
void persist_and_put(PersistedMap* map, int keyLen, char* key, int valueLen, char* val);
void persist_and_del(PersistedMap* map, int keyLen, char* keys);
#endif