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
    free_map(root);
}
END_TEST