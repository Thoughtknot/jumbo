#include "persistedmap.h"
#include "list_files.h"

void compact_map(PersistedMap* map) {
    char * filename = (char*) malloc(strlen(map->persist->name + 6));
    strcpy(filename, map->persist->name);
    strcat(filename, "-1");

    char * original_filename = (char*) malloc(strlen(map->persist->name));
    strcpy(original_filename, map->persist->name);

    Persist* new_persist = create_persist(filename, true);
    printf("%s <-> %s\n", original_filename, filename);
    
    Key** ks = keys(map->map, map->mapsize);
    int no_keys = MIN(map->mapsize, map->map->count);
    for (int i = 0; i < no_keys; i++) {
        Value* val = get(map->map, ks[i]->keySize, ks[i]->key);
        persist(new_persist, val->valueSize, PUT, val->value);
    }
    free(ks);

    remove(original_filename);
    fflush(map->persist->fp);
    free_persist(map->persist);
    map->persist = new_persist;
    fflush(map->persist->fp);
    fclose(map->persist->fp);
        
    rename(filename, original_filename);

    map->persist->fp = fopen(original_filename, "a+b");
    map->persist->name = original_filename;
    free(filename);
}

PersistedMap* getTable(char* directory, int table, PersistedMap** pm, int mapsize) {
    if (pm[table] == NULL) {
        pm[table] = (PersistedMap*) malloc(sizeof(PersistedMap));
        pm[table]->mapsize = mapsize;
        pm[table]->map = create_map(mapsize);
        pm[table]->persist = NULL;

        char * filename = (char*) malloc(50);
        strcpy(filename, directory);
        strcat(filename, "/");
        char prefix[6]; 
        sprintf(prefix, "%d", table);
        strcat(filename, prefix);
        strcat(filename, ".jmb");
        printf("File: %s\n", filename);
        pm[table]->persist = create_persist(filename, true);
    }
    return pm[table];
}

void persist_and_put(PersistedMap* map, int keyLen, char* key, int valueLen, char* value) {
    persist(map->persist, keyLen, PUT, key);
    persist(map->persist, valueLen, PUT, value);
    put(map->map, keyLen, key, valueLen, value);
    ++map->stateSeqNo;
    if (map->map->size <= map->map->count) {
        printf("Resizing map.\n");
        map->map = resize(map->map);
        map->mapsize = map->map->size;
    }
}

void persist_and_del(PersistedMap* map, int keyLen, char* key) {
    persist(map->persist, keyLen, DEL, key);
    del(map->map, keyLen, key);
    ++map->stateSeqNo;
}

int load_maps(PersistedMap* pm) {
    int i = 0;
    while (true) {
        Object* key = load(pm->persist);
        if (!key)
            break;
        if (key->operation == DEL) {
            del(pm->map, key->size, key->bytes);
        }
        else {
            Object* value = load(pm->persist);
            put(pm->map, key->size, key->bytes, value->size, value->bytes);
            free_object(value);
        }
        free_object(key);
        i++;
    }
    return i;
}

PersistedMap** build_maps(char* directory, int size) {
    printf("Building maps from directory [%s].\n", directory);
    PersistedMap** persistedMaps = (PersistedMap**) calloc(size, sizeof(PersistedMap*));
    DirectoryContents* contents = list_directory_contents(directory);
    printf("Found %d files in [%s].\n", contents->numFiles, directory);
    for (int i = 0; i < contents->numFiles; i++) {
        char* buffer = (char*) malloc(strlen(directory) + strlen(contents->fileNames[i]) + 2);
        sprintf(buffer, "%s/%s", directory, contents->fileNames[i]);
        printf("File: %s\n", contents->fileNames[i]);
        char * seq = strtok(contents->fileNames[i], ".jmb");
        if (seq != NULL) {
            int mapNo = atoi(seq);
            persistedMaps[mapNo] = (PersistedMap*) malloc(sizeof(PersistedMap));
            persistedMaps[mapNo]->persist = create_persist(buffer, false);
            persistedMaps[mapNo]->map = create_map(size);
            persistedMaps[mapNo]->mapsize = size;
            int loadedRecords = load_maps(persistedMaps[mapNo]);
            printf("Loaded %d records as table %d from %s\n", loadedRecords, mapNo, buffer);
        }
        free(buffer);
    }
    free_contents(contents);
    return persistedMaps;
}

void free_maps(PersistedMap** map, int size) {
    for (int i = 0; i < size; i++) {
        if (!map[i])
            continue;
        free_map(map[i]->map);
        free_persist(map[i]->persist);
    }
    free(map);
}