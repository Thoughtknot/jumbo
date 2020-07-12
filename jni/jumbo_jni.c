#include "../src/persistedmap.h"
#include "org_jumbo_JumboJni.h"
#include <sys/stat.h>

typedef struct State State;
struct State {
    PersistedMap** map;
    int size;
};

jfieldID get_state_pointer_field(JNIEnv* env, jobject obj) {
    jclass c = (*env)->GetObjectClass(env, obj);
    jfieldID ptrFieldId = (*env)->GetFieldID(env, c, "statePtr", "J");
    (*env)->DeleteLocalRef(env, c);
    return ptrFieldId;
}

JNIEXPORT void JNICALL Java_org_jumbo_JumboJni_init(JNIEnv * env, jobject obj, jint size) {
    struct stat st = {0};
    if (stat("db", &st) == -1) {
        mkdir("db", 0700);
    }

    State* state = (State*) malloc(sizeof(State));
    state->map = build_maps(size);
    state->size = size;
    jfieldID ptr = get_state_pointer_field(env, obj);
    (*env)->SetLongField(env, obj, ptr, state);
}

JNIEXPORT void JNICALL Java_org_jumbo_JumboJni_put(JNIEnv * env, jobject obj, jint table, jbyteArray key, jbyteArray value) {
    jfieldID ptr = get_state_pointer_field(env, obj);
    State* state = (State*) (*env)->GetLongField(env, obj, ptr);
    PersistedMap* map = getTable(table, state->map, state->size);
    int keyLen = (*env)->GetArrayLength(env, key);
    signed char keyBuf[keyLen];
    (*env)->GetByteArrayRegion(env, key, 0, keyLen, keyBuf);
    int valLen = (*env)->GetArrayLength(env, value);
    signed char valBuf[valLen];
    (*env)->GetByteArrayRegion(env, value, 0, valLen, valBuf);
    printf("PUT %s\n", keyBuf);
    persist_and_put(map, keyLen, keyBuf, valLen, valBuf);
}

JNIEXPORT jbyteArray JNICALL Java_org_jumbo_JumboJni_get(JNIEnv * env, jobject obj, jint table, jbyteArray key) {
    jfieldID ptr = get_state_pointer_field(env, obj);
    State* state = (State*) (*env)->GetLongField(env, obj, ptr);
    PersistedMap* map = getTable(table, state->map, state->size);
    int keyLen = (*env)->GetArrayLength(env, key);
    signed char keyBuf[keyLen];
    (*env)->GetByteArrayRegion(env, key, 0, keyLen, keyBuf);

    printf("GET %s\n", keyBuf);
    Value* val = get(map->map, keyLen, keyBuf);
    jbyteArray array;
    if (val == NULL) {
        array = (*env)->NewByteArray(env, 0);
    }
    else {
        array = (*env)->NewByteArray(env, val->valueSize);
        (*env)->SetByteArrayRegion(env, array, 0, val->valueSize, val->value);
    }
    return array;
}

JNIEXPORT void JNICALL Java_org_jumbo_JumboJni_del(JNIEnv * env, jobject obj, jint table, jbyteArray key) {
    jfieldID ptr = get_state_pointer_field(env, obj);
    State* state = (State*) (*env)->GetLongField(env, obj, ptr);
    PersistedMap* map = getTable(table, state->map, state->size);
    int keyLen = (*env)->GetArrayLength(env, key);
    char keyBuf[keyLen];
    (*env)->GetByteArrayRegion(env, key, 0, keyLen, keyBuf);
    del(map->map, keyLen, keyBuf);
}

JNIEXPORT jobjectArray JNICALL Java_org_jumbo_JumboJni_keys(JNIEnv * env, jobject obj, jint table, jint limit) {
    jfieldID ptr = get_state_pointer_field(env, obj);
    State* state = (State*) (*env)->GetLongField(env, obj, ptr);
    PersistedMap* map = getTable(table, state->map, state->size);

    Key** key = keys(map->map, limit);
    int no_responses = MIN(map->map->count, limit);

    jclass byteArrayClass = (*env)->FindClass(env,"[B");
    jobjectArray array = (*env)->NewObjectArray(env, no_responses, byteArrayClass, NULL);

    for (int i = 0; i < no_responses; i++) {
        jbyteArray val = (*env)->NewByteArray(env, key[i]->keySize);
        (*env)->SetByteArrayRegion(env, val, 0, key[i]->keySize, key[i]->key);
        (*env)->SetObjectArrayElement(env, array, i, val); 
    }
    return array;
}