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
#include <sys/stat.h>

void compact_map(PersistedMap* map);
PersistedMap** build_maps(char* dir, int size);
void free_maps(PersistedMap** map, int size);
void persist_and_del(PersistedMap* map, int keyLen, char* keys);
void persist_and_put(PersistedMap* map, int keyLen, char* key, int valueLen, char* val);
void *handle_client_socket(void * args);
PersistedMap* getTable(char* directory, int table, PersistedMap** pm, int mapsize);
#endif