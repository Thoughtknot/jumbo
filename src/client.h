
#include <pthread.h>
#include <unistd.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include "common.h"
#include "persistedmap.h"

typedef union char_int char_int;
union char_int {
    char chars[4];
    int32_t num;
};

void init_mutex(int size);
void deinit_mutex(int size);