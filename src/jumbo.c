#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <dirent.h>
#include "map.h"
#include "persist.h"
#define PORT 8080 
#define SA struct sockaddr 

#define PUT 11
#define GET 21
#define DEL 31

#define SIZE 1024

typedef struct PersistedMap PersistedMap;
struct PersistedMap {
    Persist* persist;
    HashMap* map;
};

union char_int {
    char chars[4];
    int32_t num;
};

int sendBytes(int sockfd, char * val, int len) {
    int result = send(sockfd, val, len, 0);
    return result;
}

char * readBytes(int sockfd, int len) {
    char* buff = (char*)malloc(sizeof(char)*len);
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
    union char_int buff;
    buff.num = val;
    int result = send(sockfd, buff.chars, 4, 0);
    return result;
}

int readInt(int sockfd) {
    union char_int buff;
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

HashMap* getTable(int table, PersistedMap** pm) {
    if (!pm[table]) {
        pm[table] = (PersistedMap*) malloc(sizeof(PersistedMap));
        pm[table]->map = create_map(SIZE);

        char filename[50];
        strcpy(filename, "db/");
        char prefix[6]; 
        sprintf(prefix, "%d", table);
        strcat(filename, prefix);
        strcat(filename, ".jmb");
        pm[table]->persist = create(filename, true);
    }
    return pm[table];
}

void handleClientSocket(int sockfd, PersistedMap** pm) 
{ 
    for (;;) { 
        char operation = readChar(sockfd);
        if (operation == PUT) {
            int table = readInt(sockfd);
            PersistedMap* map = getTable(table, pm);
            int keyLen = readInt(sockfd);
            char* key = readBytes(sockfd, keyLen);
            int valueLen = readInt(sockfd);
            char* value = readBytes(sockfd, valueLen);
            printf("PUT - table: %d, key: %s, value: %s\n", table, key, value); 
            persist(map->persist, keyLen, key);
            persist(map->persist, valueLen, value);
            put(map->map, keyLen, key, valueLen, value);
            free(key);
            free(value);
        }
        else if (operation == GET) {
            int table = readInt(sockfd);
            PersistedMap* map = getTable(table, pm);
            int keyLen = readInt(sockfd);
            char* key = readBytes(sockfd, keyLen);
            Value* val = get(map->map, keyLen, key);
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
            PersistedMap* map = getTable(table, pm);
            int keyLen = readInt(sockfd);
            char* key = readBytes(sockfd, keyLen);
            del(map->map, keyLen, key);
            printf("DEL: - table: %d, key: %s\n", table, key);
            free(key);
        }
    } 
}

int load_maps(PersistedMap* pm) {
    int i = 0;
    while (true) {
        Object* key = load(pm->persist);
        Object* value = load(pm->persist);
        if (!key || !value) {
            if (value)
                free_object(value);
            break;
        }
        put(pm->map, key->size, key->bytes, value->size, value->bytes);
        free_object(key);
        free_object(value);
        i++;
    }
    return i;
}

PersistedMap** build_maps() {
    PersistedMap** persistedMaps = (PersistedMap**) calloc(SIZE, sizeof(PersistedMap*));
    DIR *d;
    struct dirent *dir;
    d = opendir("db");
    const char * s = ".jmb";
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
                    persistedMaps[mapNo]->persist = create(v, false);
                    persistedMaps[mapNo]->map = create_map(SIZE);
                    int loadedRecords = load_maps(persistedMaps[mapNo]);
                    printf("Loaded %d records as table %d from %s\n", loadedRecords, mapNo, v);
                }
            }
        }
        closedir(d);
    }
    return persistedMaps;
}

void free_maps(PersistedMap** map) {
    for (int i = 0; i < SIZE; i++) {
        if (!map[i])
            continue;
        free_map(map[i]->map);
        free_persist(map[i]->persist);
    }
    free(map);
}

// Driver function 
int main(int argc,char *aa[]) 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Socket creation failed.\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created.\n"); 
    memset(&servaddr, 0, sizeof(servaddr)); 
  
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("Sock bind failed.\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully bound.\n"); 
  
    PersistedMap** pm = build_maps();

    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed.\n"); 
        exit(0); 
    } 
    else
        printf("Server listening.\n"); 
    len = sizeof(cli); 
  
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("Server accept failed.\n"); 
        exit(0); 
    } 
    else
        printf("Server accepted the client.\n"); 
  
    handleClientSocket(connfd, pm);

    free_maps(pm);
    close(sockfd); 
}