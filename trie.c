#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

#define CHARSIZE 256
typedef struct Node Node;
typedef struct List List;

struct Node { 
    int valLen;
	char* value;
    Node* children[CHARSIZE];
    //Node * parent;
}; 

void free_node(Node* node) {
    if (node == NULL) {
        return;
    }
    if (node->value != NULL)
        free(node->value);
    for (int i = 0; i < CHARSIZE; i++) {
        if (node->children[i] != NULL) {
            free_node(node->children[i]);
        }
    }
    free(node);
}

Node* createNode() {
    Node* trie = (Node*) malloc(sizeof(Node));
    //trie->parent = parent;
    return trie;
}

void putVal(Node* trie, int keyLen, char* key, int valLen, char* val) {
    Node* temp = trie;
    for (int i = 0; i < keyLen; i++) {
        unsigned char c = key[i];
        if (temp->children[c] == NULL) {
            temp->children[c] = createNode();
        }
        temp = temp->children[c];
    }
    if (temp->value != NULL) {
        free(temp->value);
    }
    temp->value = (char*) malloc(valLen);
    memcpy(temp->value, val, valLen);
}

void delVal(Node* trie, int keyLen, char* key) {
    Node* temp = trie;
    for (int i = 0; i < keyLen; i++) {
        unsigned char c = key[i];
        temp = temp->children[c];
        if (temp == NULL) {
            return;
        }
    }
    if (temp->value != NULL) {
        free(temp->value);
        temp->value = NULL;
    }
}

char* getVal(Node* trie, int keyLen, char* key) {
    Node* temp = trie;
    for (int i = 0; i < keyLen; i++) {
        unsigned char c = key[i];
        temp = temp->children[c];
        if (temp == NULL) {
            return NULL;
        }
    }
    return temp->value;
}
/*
int main() {
    Node* root = createNode();
    put(root, 4, "test", 2, "-1-");
    put(root, 4, "team", 5, "-heal");
    put(root, 4, "test", 2, "99");
    put(root, 5, "roast", 2, "RsR");
    delete(root, 4, "team");
    char * val1 = get(root, 3, "tea");
    char * val2 = get(root, 4, "team");
    char * val3 = get(root, 4, "test");
    char * valr = get(root, 5, "roast");
    printf("Val: %s\n", val1);
    printf("Val: %s\n", val2);
    printf("Val: %s\n", val3);
    printf("Val: %s\n", valr);
    
    put(root, 3, "tea", 2, "44");
    char * val4 = get(root, 3, "tea");
    printf("Val: %s\n", val4);

    free_node(root);
}
*/