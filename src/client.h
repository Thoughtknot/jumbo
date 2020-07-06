
#include <pthread.h>
#include <unistd.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include "common.h"
#include "persist.h"
#include "map.h"
#define PUT 11
#define GET 21
#define DEL 31
#define KEYS 41

typedef union char_int char_int;
union char_int {
    char chars[4];
    int32_t num;
};

void init_mutex();
void deinit_mutex();
void *handle_client_socket(void * args);