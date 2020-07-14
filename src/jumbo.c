#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "persistedmap.h"
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <pthread.h>
#include "client.h"

#define PORT 8080 
#define MAX_CONNECTIONS 50
#define SIZE 1024

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

int main(int argc, char *aa[]) 
{ 
    unsigned int sockfd, connfd, len; 
    sockaddr_in servaddr; 
  
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
  
    PersistedMap** pm = build_maps("db", SIZE);

    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed.\n"); 
        exit(0); 
    } 
    else
        printf("Server listening.\n"); 
    len = sizeof(sockaddr_in); 
  
    init_mutex(SIZE);
    pthread_t tid[MAX_CONNECTIONS];
    int i = 0;
    while (i < MAX_CONNECTIONS) {
        sockaddr_in cli;
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

        pthread_create(&tid[i], NULL, handle_client_socket, (void*) client);
        i++;
    }
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        pthread_join(tid[i], NULL);
    }
    deinit_mutex(SIZE);
    free_maps(pm, SIZE);
    close(sockfd); 
}