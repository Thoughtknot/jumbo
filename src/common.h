#ifndef common_h
#define common_h
typedef struct Key Key;
 
struct Key {
    int keySize;
    char* key;
};

typedef struct Value Value;
 
struct Value {
    int valueSize;
    char* value;
};

typedef struct Object Object;
 
struct Object {
    int size;
    char* bytes;
};
#endif