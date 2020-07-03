#ifndef common_h
#define common_h
#define MIN(a,b) ((a) < (b) ? (a) : (b))

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

typedef struct KeyValue KeyValue;

struct KeyValue {
    Key* key;
    Value* value;
};

typedef struct Object Object;
struct Object {
    int size;
    char* bytes;
};

#endif