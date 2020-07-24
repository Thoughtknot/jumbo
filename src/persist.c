#include "persist.h"

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
        p->fp = fopen(path, "w+b");
    }
    else {
        p->fp = fopen(path, "a+b");
    }
    if(ferror(p->fp)){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
    p->name = (char*) malloc(strlen(path) + 1);
    strcpy(p->name, path);
    return p;
}

void persist(Persist* persist, int size, unsigned char operation, char* bytes) {
    fwrite(&operation, 1, 1, persist->fp);
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
    if (!fread(&val->operation, 1, 1, persist->fp)
            || !fread(&val->size, sizeof(int), 1, persist->fp)) {
        free(val);
        return NULL;
    }
    val->bytes = (char*) malloc(val->size);
    if (fread(val->bytes, 1, val->size, persist->fp) != val->size
            || ferror(persist->fp)){
        free_object(val);
        return NULL;
    }
    return val;
}