#include "trie.h"

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