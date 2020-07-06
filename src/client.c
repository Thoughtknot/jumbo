#include "client.h"

pthread_mutex_t lock; 

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
    int result = recv(sockfd, chars, 1, 0);
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

void init_mutex() {
    pthread_mutex_init(&lock, NULL);
}

void deinit_mutex() {
    pthread_mutex_destroy(&lock);
}
void *handle_client_socket(void * args) {
    int sockfd = ((client_args*) args)->socketfd;
    PersistedMap** pm = ((client_args*) args)->pm;
    int mapsize = ((client_args*) args)->mapsize;
    for (;;) {
        char operation = readChar(sockfd);
        if (operation == PUT) {
            int table = readInt(sockfd);

            pthread_mutex_lock(&lock); 
            PersistedMap* map = getTable(table, pm, mapsize);
            pthread_mutex_unlock(&lock); 
            
            int keyLen = readInt(sockfd);
            char* key = readBytes(sockfd, keyLen);
            int valueLen = readInt(sockfd);
            char* value = readBytes(sockfd, valueLen);
            printf("PUT - table: %d, key: %s, value: %s\n", table, key, value); 
            
            pthread_mutex_lock(&lock); 
            printf("a\n");
            persist(map->persist, keyLen, key);
            printf("b\n");
            persist(map->persist, valueLen, value);
            printf("c\n");
            put(map->map, keyLen, key, valueLen, value);
            printf("d\n");
            pthread_mutex_unlock(&lock); 
            printf("e\n");

            free(key);
            free(value);
            printf("f\n");
        }
        else if (operation == GET) {
            int table = readInt(sockfd);
            
            pthread_mutex_lock(&lock); 
            PersistedMap* map = getTable(table, pm, mapsize);
            pthread_mutex_unlock(&lock); 

            int keyLen = readInt(sockfd);
            char* key = readBytes(sockfd, keyLen);

            pthread_mutex_lock(&lock); 
            Value* val = get(map->map, keyLen, key);
            pthread_mutex_unlock(&lock); 
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
            pthread_mutex_lock(&lock); 
            PersistedMap* map = getTable(table, pm, mapsize);
            pthread_mutex_unlock(&lock); 

            int keyLen = readInt(sockfd);
            char* key = readBytes(sockfd, keyLen);

            pthread_mutex_lock(&lock); 
            del(map->map, keyLen, key);
            pthread_mutex_unlock(&lock); 

            printf("DEL: - table: %d, key: %s\n", table, key);
            free(key);
        }
        else if (operation == KEYS) {
            int table = readInt(sockfd);

            pthread_mutex_lock(&lock); 
            PersistedMap* map = getTable(table, pm, mapsize);
            pthread_mutex_unlock(&lock); 

            int limit = readInt(sockfd);

            pthread_mutex_lock(&lock); 
            Key** key = keys(map->map, limit);
            pthread_mutex_unlock(&lock); 
            
            int no_responses = MIN(map->mapsize, limit);
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