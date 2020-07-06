#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <dirent.h>
#include <pthread.h>
#include <unistd.h> 
#include "map.h"
#include "persist.h"
#include "client.h"

#define PORT 8080 
#define MAX_CONNECTIONS 50
#define SIZE 1024

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

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
    for (int i = 0; i < SIZE; i++) {
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
                    persistedMaps[mapNo]->persist = create(v, false);
                    persistedMaps[mapNo]->map = create_map(SIZE);
                    persistedMaps[mapNo]->mapsize = SIZE;
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
    sockaddr_in servaddr, cli; 
  
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
  
    if ((bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
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
  
    init_mutex();
    pthread_t tid[MAX_CONNECTIONS];
    int i = 0;
    while (i < MAX_CONNECTIONS) {
        connfd = accept(sockfd, (sockaddr*)&cli, &len); 
        if (connfd < 0) { 
            printf("Server accept failed.\n"); 
            exit(0); 
        } 
        else
            printf("Server accepted client %d.\n", i);
        client_args* client = (client_args*) malloc(sizeof(client_args));
        client->socketfd = connfd;
        client->mapsize = SIZE;
        client->pm = pm;

        //pthread_create(&tid[i], NULL, handle_client_socket, (void *)client);
        handle_client_socket((void *) client);
        i++;
    }
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        pthread_join(tid[i], NULL);
    }
    deinit_mutex();
    free_maps(pm);
    close(sockfd); 
}