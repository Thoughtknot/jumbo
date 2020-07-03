#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include "common.h"

typedef struct Persist Persist;
struct Persist {
    FILE* fp;
    char* name;
};

void remove_free_persist(Persist* p);
void free_persist(Persist* p);
void free_object(Object* o);

Persist* create(char * path, bool create);

void persist(Persist* persist, int size, char* bytes);

Object* load(Persist* persist);