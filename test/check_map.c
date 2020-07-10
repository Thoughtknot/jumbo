#include <check.h>
#include "../src/map.h"

START_TEST (test_map_basic)
{
    HashMap* root = create_map(1024);
    put(root, 4, "test", 2, "-1-");
    put(root, 4, "team", 5, "-heal");
    put(root, 4, "test", 2, "99");
    put(root, 5, "roast", 2, "RsR");
    del(root, 4, "team");
    Value* val1 = get(root, 3, "tea");
    Value* val2 = get(root, 4, "team");
    Value* val3 = get(root, 4, "test");
    Value* val4 = get(root, 5, "roast");

    del(root, 4, "team");
    Value* val5 = get(root, 4, "team");

    put(root, 3, "tea", 2, "45");
    const Value* val6 = get(root, 3, "tea");

    ck_assert(val1 == NULL);
    ck_assert(val2 == NULL);
    ck_assert(memcmp(val3->value, "99", val3->valueSize) == 0);
    ck_assert_str_eq(val4->value, "Rs");
    ck_assert(val5 == NULL);
    ck_assert(memcmp(val6->value, "45", val6->valueSize) == 0);

    Key** k = keys(root, 10);
    ck_assert(k != NULL);
    ck_assert(memcmp(k[0]->key, "test", k[0]->keySize) == 0);
    ck_assert(memcmp(k[1]->key, "tea", k[1]->keySize) == 0);
    ck_assert(memcmp(k[2]->key, "roast", k[2]->keySize) == 0);
    free(k);

    free_map(root);
}
END_TEST

START_TEST (test_map_unorthodox)
{
    HashMap* root = create_map(1024);
    char f[] = {50};
    char g[] = {50};
    char a[] = {57, 57};
    char b[] = {57, 57};
    char c[] = {49, 49, 48};
    char d[] = {49, 49, 48};
    put(root, 1, f, 1, g);
    put(root, 2, a, 2, b);
    put(root, 3, c, 3, d);
    Value* val = get(root, 2, b);
    ck_assert(val != NULL);
    ck_assert(memcmp(val->value, "99", 2) == 0);

    free_map(root);
}
END_TEST

START_TEST (test_map_resize)
{
    HashMap* root = create_map(3);
    put(root, 1, "a", 1, "g");
    put(root, 1, "b", 1, "g");
    put(root, 1, "c", 1, "g");
    root = resize(root);
    put(root, 1, "d", 1, "g");
    put(root, 1, "e", 1, "g");
    ck_assert_int_eq(root->count, 5);
    ck_assert_int_eq(root->size, 12);

    free_map(root);
}
END_TEST