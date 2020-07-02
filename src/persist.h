#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
typedef struct Object Object;
 
struct Object {
    int size;
    char* bytes;
};

typedef struct Persist Persist;
struct Persist {
    FILE *fp;
};

void free_persist(Persist* p);
void free_object(Object* o);

Persist* create(char * path, bool create);

void persist(Persist* persist, int size, char* bytes);

Object* load(Persist* persist);