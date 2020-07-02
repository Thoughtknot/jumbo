#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

#define CHARSIZE 256
typedef struct Node Node;

struct Node { 
    int valLen;
	char* value;
    Node* children[CHARSIZE];
    //Node * parent;
};

Node* createNode();
void free_node(Node* node);
void putVal(Node* trie, int keyLen, char* key, int valLen, char* val);
void delVal(Node* trie, int keyLen, char* key);
char* getVal(Node* trie, int keyLen, char* key);
