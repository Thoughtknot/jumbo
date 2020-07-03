#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "common.h"

#define CHARSIZE 256

typedef struct Node Node;
struct Node {
    Value* value;
    Node* children[CHARSIZE];
};

typedef struct Trie Trie;
struct Trie {
    Value* value;
    Node* root;
    int size;
};

Trie* createTrie();
void free_trie(Trie* node);

void putVal(Trie* trie, int keyLen, char* key, int valLen, char* val);
void delVal(Trie* trie, int keyLen, char* key);
Value* getVal(Trie* trie, int keyLen, char* key);
Key** getKeys(Trie* trie, int limit);