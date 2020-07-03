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

Persist* create(char * path, bool create) {
    Persist* p = (Persist*) malloc(sizeof(Persist));
    if (create) {
        p->fp = fopen(path, "w+b");
    }
    else {
        p->fp = fopen(path, "a+b");
    }
    p->name = (char*) malloc(strlen(path));
    strcpy(p->name, path);
    return p;
}

void persist(Persist* persist, int size, char* bytes) {
    fwrite(&size, sizeof(int), 1, persist->fp);
    fwrite(bytes, 1, size, persist->fp);
    fflush(persist->fp);
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