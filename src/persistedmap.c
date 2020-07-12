#include "persistedmap.h"

PersistedMap* getTable(int table, PersistedMap** pm, int mapsize) {
    if (pm[table] == NULL) {
        pm[table] = (PersistedMap*) malloc(sizeof(PersistedMap));
        pm[table]->mapsize = mapsize;
        pm[table]->map = create_map(mapsize);
        pm[table]->persist = NULL;

        char * filename = (char*) malloc(50);
        strcpy(filename, "db/");
        char prefix[6]; 
        sprintf(prefix, "%d", table);
        strcat(filename, prefix);
        strcat(filename, ".jmb");
        pm[table]->persist = create_persist(filename, true);
    }
    return pm[table];
}

void persist_and_put(PersistedMap* map, int keyLen, char* key, int valueLen, char* value) {
    persist(map->persist, keyLen, PUT, key);
    persist(map->persist, valueLen, PUT, value);
    put(map->map, keyLen, key, valueLen, value);
    if (map->map->size <= map->map->count) {
        printf("Resizing map.\n");
        map->map = resize(map->map);
        map->mapsize = map->map->size;
    }
}

void persist_and_del(PersistedMap* map, int keyLen, char* key) {
    persist(map->persist, keyLen, DEL, key);
    del(map->map, keyLen, key);
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

PersistedMap** build_maps(int size) {
    PersistedMap** persistedMaps = (PersistedMap**) calloc(size, sizeof(PersistedMap*));
    DIR *d;
    struct dirent *dir;
    d = opendir("db");
    const char * s = ".jmb";
    for (int i = 0; i < size; i++) {
        persistedMaps[i] = NULL;
    }
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char v[strlen(dir->d_name)];
            strcpy(v, "db/");
            strcat(v, dir->d_name);
            if (strstr(dir->d_name, ".jmb")) {
                char * seq = strtok(dir->d_name, s);
                if (seq != NULL) {
                    int mapNo = atoi(seq);
                    persistedMaps[mapNo] = (PersistedMap*) malloc(sizeof(PersistedMap));
                    persistedMaps[mapNo]->persist = create_persist(v, false);
                    persistedMaps[mapNo]->map = create_map(size);
                    persistedMaps[mapNo]->mapsize = size;
                    int loadedRecords = load_maps(persistedMaps[mapNo]);
                    printf("Loaded %d records as table %d from %s\n", loadedRecords, mapNo, v);
                }
            }
        }
        closedir(d);
    }
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