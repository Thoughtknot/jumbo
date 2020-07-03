#include "trie.h"

void free_node(Node* node) {
    if (node == NULL) {
        return;
    }
    if (node->value != NULL)
        free_value(node->value);
    for (int i = 0; i < CHARSIZE; i++) {
        if (node->children[i] != NULL) {
            free_node(node->children[i]);
        }
    }
    free(node);
}

void free_value(Value* value) {
    free(value->value);
    free(value);
}

Value* create_value(char* value, int valLen) {
    Value* ret = (Value*) malloc(sizeof(Value));
    ret->value = (char*) malloc(valLen);
    memcpy(ret->value, value, valLen);
    return ret;
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
        free_value(temp->value);
    }
    temp->value = create_value(val, valLen);
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
        free_value(temp->value);
        temp->value = NULL;
    }
}

Value* getVal(Node* trie, int keyLen, char* key) {
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