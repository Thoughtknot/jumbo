#include "persist.h"

void free_persist(Persist* p) {
    fclose(p->fp);
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
/*
int main() {
    Persist* p = create("tst.file", true);
    persist(p, 2, "Hi");
    persist(p, 3, "Boo");
    persist(p, 3, "Cow");
    destroy(p);
    
    Persist* q = create("tst.file", false);
    Object* val1 = load(q);
    Object* val2 = load(q);
    Object* val3 = load(q);
    printf("Val: %d, %s\n", val1->size, val1->bytes);
    printf("Val: %d, %s\n", val2->size, val2->bytes);
    printf("Val: %d, %s\n", val3->size, val3->bytes);
    free(val1->bytes);
    free(val2->bytes);
    free(val3->bytes);
    free(val1);
    free(val2);
    free(val3);
    destroy(q);
}
*/