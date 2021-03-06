#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include "common.h"

void free_persist(Persist* p);
void free_object(Object* o);

Persist* create_persist(char * path, bool create);

void persist(Persist* persist, int size, unsigned char operation, char* bytes);

Object* load(Persist* persist);