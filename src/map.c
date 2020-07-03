#include "map.h"

unsigned long hash(int size, int len, char* str) {
    unsigned long  hash = 5381;
    for (int j=0; j < len; j++)
        hash = ((hash << 5) + hash) + str[j];
    return hash % size;
}
 
LinkedEntries* linked_entries() {
    LinkedEntries* list = (LinkedEntries*) malloc (sizeof(LinkedEntries));
    list->entry = NULL;
    list->next = NULL;
    return list;
}

LinkedEntries* insert(LinkedEntries* list, KeyValue* entry) {
    if (!list) {
        list = linked_entries();
        list->entry = entry;
        return list;
    }
    if (list->next == NULL) {
        LinkedEntries* node = linked_entries();
        node->entry = entry;
        list->next = node;
        return list;
    }

    LinkedEntries* entries = list;
    while (entries->next->next) {
        entries = entries->next;
    }

    LinkedEntries* tail = linked_entries();
    tail->entry = entry;
    entries->next = tail;
    return list;
}

KeyValue* removeEntry(LinkedEntries* list) {
    // Removes the head from the linked list
    // and returns the item of the popped element
    if (!list)
        return NULL;
    if (!list->next)
        return NULL;
    LinkedEntries* node = list->next;
    LinkedEntries* temp = list;
    temp->next = NULL;
    list = node;
    KeyValue* it = NULL;

    memcpy(temp->entry, it, sizeof(KeyValue));
    free_entry(temp->entry);
    temp->entry = NULL;
    free(temp);
    return it;
}

HashMap* create_map(int size) {
    HashMap* table = (HashMap*) malloc(sizeof(HashMap));
    table->size = size;
    table->count = 0;
    table->entries = (LinkedEntries**) calloc (table->size, sizeof(LinkedEntries*));
    for (int i=0; i < table->size; i++) {
        table->entries[i] = NULL;
    }
 
    return table;
}

KeyValue* create_entry(int keyLen, char* key, int valLen, char* value) {
    // Creates a pointer to a new hash table item
    KeyValue* item = (KeyValue*) malloc (sizeof(KeyValue));
    item->key = (Key*) malloc(sizeof(Key));
    item->value = (Value*) malloc(sizeof(Value));
    item->key->keySize = keyLen;

    item->value->valueSize = valLen;
    item->key->key = (char*) malloc (keyLen);
    item->value->value = (char*) malloc (valLen);
     
    memcpy(item->key->key, key, keyLen);
    memcpy(item->value->value, value, valLen);
    return item;
}

int compare(int aLen, char * a, int bLen, char * b) {
    if (aLen > bLen) {
        return 1;
    }
    else if (aLen < bLen) {
        return -1;
    }
    for (int i = 0; i < aLen; i++) {
        int cmp = a[i] - b[i];
        if (cmp > 0) {
            return 1;
        } 
        else if (cmp < 0) {
            return -1;
        }
    }
    return 0;
}

Key** keys(HashMap* map, int limit) {
    Key** keys = (Key**) calloc(MIN(limit, map->size), sizeof(Key*));
    int idx = 0;
    for (int i = 0; i < map->size; i++) {
        LinkedEntries* entry = map->entries[i];
        while (entry) {
            keys[idx++] = entry->entry->key;
            entry = entry->next;
        }
    }
    return keys;
}

void handle_collision(HashMap* table, int index, KeyValue* entry) {
    LinkedEntries* head = table->entries[index];
    table->entries[index] = insert(head, entry);
}

void put(HashMap* table, int keyLen, char* key, int valLen, char* value) {
    KeyValue* item = create_entry(keyLen, key, valLen, value);
    unsigned long h = hash(table->size, keyLen, key);
    LinkedEntries* current = table->entries[h];
    if (current == NULL) {
        if (table->count == table->size) {
            printf("Hash map full. TODO: resize!\n");
            free_entry(item);
            return;
        }
        // Insert directly
        table->entries[h] = linked_entries();
        table->entries[h]->entry = item;
        table->count++;
    }
    else if (compare(current->entry->key->keySize, current->entry->key->key, item->key->keySize, item->key->key) == 0) {
        free_entry(current->entry);
        current->entry = item;
    }
    else {
        handle_collision(table, h, item);
    }
}

void free_list(LinkedEntries* list) {
    LinkedEntries* temp = list;
    while (temp) {
        free_entry(temp->entry);
        temp->entry == NULL;
        free(temp);
        temp = temp->next;
    }
}

void free_entry(KeyValue* entry) {
    free(entry->key->key);
    entry->key->key = NULL;
    free(entry->key);
    entry->key = NULL;
    if (entry->value != NULL) {
        free(entry->value->value);
        free(entry->value);
    }
    free(entry);
}

void free_overflow_buckets(HashMap* table) {
    // Free all the overflow bucket lists
    LinkedEntries** buckets = table->entries;
    for (int i = 0; i < table->size; i++) {
        free_list(buckets[i]);
    }
}

void free_map(HashMap* map) {
    free_overflow_buckets(map);
    free(map->entries);
    free(map);
}

void del(HashMap* table, int keyLen, char* key) {
    unsigned long index = hash(table->size, keyLen, key);
    LinkedEntries* prev = NULL;
    LinkedEntries* item = table->entries[index];
    while (item != NULL) {
        if (compare(item->entry->key->keySize, item->entry->key->key, keyLen, key) == 0) {
            if (prev == NULL) {
                table->entries[index] = item->next;
            }
            else if (prev != NULL) {
                prev->next = item->next;
            }
            item->next = NULL;
            free_entry(item->entry);
            item->entry = NULL;
            free(item);
            table->count--;
            break;
        }
        prev = item;
        item = item->next;
    }
    //printf("DEL: %d\n", table->entries[index] == NULL);
    return;
}

Value* get(HashMap* table, int keyLen, char* key) {
    unsigned long index = hash(table->size, keyLen, key);
    LinkedEntries* item = table->entries[index];
    while (item != NULL) {
        //printf("%s\n", key == NULL);
        if (compare(item->entry->key->keySize, item->entry->key->key, keyLen, key) == 0) {
            return item->entry->value;
        }
        if (item == NULL) {
            return NULL;
        }
        item = item->next;
    }
    return NULL;
}