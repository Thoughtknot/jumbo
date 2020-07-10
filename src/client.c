#include "client.h"

pthread_mutex_t * lock = NULL; 

int sendBytes(int sockfd, char * val, int len) {
    int result = send(sockfd, val, len, 0);
    return result;
}

char * readBytes(int sockfd, int len) {
    char* buff = (char*) malloc(sizeof(char)*len);
    if (!buff)
        return NULL;

    int result = recv(sockfd, buff, len, 0);
    if (result < 1 )
    {
        printf("%s\n", strerror(result));
    }
    return buff;
}

int sendInt(int sockfd, int val) {
    char_int buff;
    buff.num = val;
    int result = send(sockfd, buff.chars, 4, 0);
    return result;
}

int readInt(int sockfd) {
    char_int buff;
    int result = recv(sockfd, buff.chars, 4, 0);
    if (result == -1)
        return -1;
    return buff.num;
}

char readChar(int sockfd) {
    char chars[1];
    recv(sockfd, chars, 1, 0);
    return chars[0];
}

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
    persist(map->persist, keyLen, key);
    persist(map->persist, valueLen, value);
    put(map->map, keyLen, key, valueLen, value);
    printf("Resizing map.\n");
    if (map->map->size <= map->map->count) {
        map->map = resize(map->map);
        map->mapsize = map->map->size;
    }
}

void init_mutex(int size) {
    lock = (pthread_mutex_t *) malloc(size * sizeof(pthread_mutex_t));
    for (int i = 0; i < size; i++) {
        pthread_mutex_init(&lock[i], NULL);
    }
}

void deinit_mutex(int size) {
    for (int i = 0; i < size; i++) {
        pthread_mutex_destroy(&lock[i]);
    }
}

void *handle_client_socket(void * args) {
    int sockfd = ((client_args*) args)->socketfd;
    PersistedMap** pm = ((client_args*) args)->pm;
    int mapsize = ((client_args*) args)->mapsize;
    for (;;) {
        char operation = readChar(sockfd);
        if (operation == PUT) {
            int table = readInt(sockfd);

            pthread_mutex_lock(&lock[table]); 
            PersistedMap* map = getTable(table, pm, mapsize);
            
            int keyLen = readInt(sockfd);
            char* key = readBytes(sockfd, keyLen);
            int valueLen = readInt(sockfd);
            char* value = readBytes(sockfd, valueLen);
            printf("PUT - table: %d, key: %s, value: %s\n", table, key, value); 
            persist_and_put(map, keyLen, key, valueLen, value);
            
            pthread_mutex_unlock(&lock[table]); 

            free(key);
            free(value);
        }
        else if (operation == GET) {
            int table = readInt(sockfd);
            
            pthread_mutex_lock(&lock[table]); 
            PersistedMap* map = getTable(table, pm, mapsize);

            int keyLen = readInt(sockfd);
            char* key = readBytes(sockfd, keyLen);

            Value* val = get(map->map, keyLen, key);
            pthread_mutex_unlock(&lock[table]); 
            if (val == NULL) {
                printf("GET: - table: %d, key: %s, value: (null)\n", table, key);
                sendInt(sockfd, -1); 
            }
            else {
                printf("GET - table: %d, key: %s, value: %s\n", table, key, val->value);
                sendInt(sockfd, val->valueSize);
                sendBytes(sockfd, val->value, val->valueSize);
            }
            free(key);
        }
        else if (operation == DEL) {
            int table = readInt(sockfd);
            pthread_mutex_lock(&lock[table]); 
            PersistedMap* map = getTable(table, pm, mapsize);

            int keyLen = readInt(sockfd);
            char* key = readBytes(sockfd, keyLen);

            del(map->map, keyLen, key);
            pthread_mutex_unlock(&lock[table]); 

            printf("DEL: - table: %d, key: %s\n", table, key);
            free(key);
        }
        else if (operation == KEYS) {
            int table = readInt(sockfd);

            pthread_mutex_lock(&lock[table]); 
            PersistedMap* map = getTable(table, pm, mapsize);

            int limit = readInt(sockfd);

            Key** key = keys(map->map, limit);
            pthread_mutex_unlock(&lock[table]); 
            
            int no_responses = MIN(map->map->count, limit);
            printf("KEYS: - table: %d, no_responses: %d\n", table, no_responses);
            sendInt(sockfd, no_responses);
            for (int i = 0; i < no_responses; i++) {
                sendInt(sockfd, key[i]->keySize);
                sendBytes(sockfd, key[i]->key, key[i]->keySize);
            }
            free(key);
        }
    } 
}