#include <check.h>
#include "../src/persist.h"

START_TEST (test_persist_basic)
{
    Persist* p = create("tst.file", true);
    persist(p, 2, "Hi");
    persist(p, 3, "Boo");
    persist(p, 3, "Cow");
    free_persist(p);
    
    Persist* q = create("tst.file", false);
    Object* val1 = load(q);
    Object* val2 = load(q);
    Object* val3 = load(q);
    ck_assert(memcmp(val1->bytes, "Hi", 2) == 0);
    ck_assert(memcmp(val2->bytes, "Boo", 3) == 0);
    ck_assert(memcmp(val3->bytes, "Cow", 3) == 0);
    free_object(val1);
    free_object(val2);
    free_object(val3);
    remove_free_persist(q);
}
END_TEST