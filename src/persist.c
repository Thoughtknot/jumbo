#include "persist.h"

void remove_free_persist(Persist* p) {
    remove(p->name);
    free_persist(p);
}

void free_persist(Persist* p) {
    fclose(p->fp);
    free(p->name);
    free(p);
}

void free_object(Object* o) {
    free(o->bytes);
    free(o);
}

Persist* create_persist(char * path, bool create) {
    Persist* p = (Persist*) malloc(sizeof(Persist));
    if (create) {
        printf("Opening a %s\n", path);
        p->fp = fopen(path, "w+b");
    }
    else {
        printf("Opening b %s\n", path);
        p->fp = fopen(path, "a+b");
    }
    if(ferror(p->fp)){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
    p->name = (char*) malloc(strlen(path));
    strcpy(p->name, path);
    return p;
}

void persist(Persist* persist, int size, char* bytes) {
    printf("persista, %d\n", persist == NULL);
    printf("persista, %d\n", persist->fp == NULL);
    fwrite(&size, sizeof(int), 1, persist->fp);
    printf("persistb\n");
    fwrite(bytes, 1, size, persist->fp);
    printf("persistc\n");
    fflush(persist->fp);
    printf("persistd\n");
    if(ferror(persist->fp)){
      perror(__func__);
      exit(EXIT_FAILURE);
    }
}

Object* load(Persist* persist) {
    Object * val = (Object *) malloc(sizeof(Object));
    int result = fread(&val->size, sizeof(int), 1, persist->fp);
    val->bytes = (char*) malloc(val->size);
    fread(val->bytes, 1, val->size, persist->fp);
    if(ferror(persist->fp)){
        free_object(val);
        return NULL;
    }
    return val;
}