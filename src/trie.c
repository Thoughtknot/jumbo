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
    Node* node = (Node*) malloc(sizeof(Node));
    return node;
}

void free_trie(Trie* trie) {
    free_node(trie->root);
    free(trie);
}

Trie* createTrie() {
    Trie* trie = (Trie*) malloc(sizeof(Trie));
    trie->size = 0;
    trie->root = createNode();
    return trie;
}

void putVal(Trie* trie, int keyLen, char* key, int valLen, char* val) {
    Node* temp = trie->root;
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
    else {
        ++trie->size;
    }
    temp->value = create_value(val, valLen);
}

void delVal(Trie* trie, int keyLen, char* key) {
    Node* temp = trie->root;
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
        --trie->size;
    }
}

Value* getVal(Trie* trie, int keyLen, char* key) {
    Node* temp = trie->root;
    for (int i = 0; i < keyLen; i++) {
        unsigned char c = key[i];
        temp = temp->children[c];
        if (temp == NULL) {
            return NULL;
        }
    }
    return temp->value;
}

Key** getKeys(Trie* trie, int limit) {
    Key** keys = (Key**) calloc(MIN(limit, trie->size), sizeof(Key*));
    get_node_keys(keys, trie->root, "", 0, 0);
    return keys;
}

int get_node_keys(Key** key_aggregator, Node* root_node, char * path, int length, int count) {
    int newcount = count;
    if (root_node->value) {
        key_aggregator[count] = (Key*) malloc(sizeof(Key));
        key_aggregator[count]->key = malloc(length);
        memcpy(key_aggregator[count]->key, path, length);
        newcount++;
    }
    for (int i = 0; i < CHARSIZE; i++) {
        if (root_node->children[i]) {
            char * newpath = malloc(length + 1);
            memcpy(newpath, path, length);
            newpath[length] = i;
            newcount = get_node_keys(key_aggregator, root_node->children[i], newpath, length + 1, newcount);
            free(newpath);
        }
    }
    return newcount;
}