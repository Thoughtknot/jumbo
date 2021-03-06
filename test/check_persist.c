#include <check.h>
#include "../src/persist.h"

START_TEST (test_persist_basic)
{
    Persist* p = create_persist("tst.file", true);
    persist(p, 2, PUT, "Hi");
    persist(p, 3, PUT, "Boo");
    persist(p, 3, PUT, "Cow");
    free_persist(p);
    
    Persist* q = create_persist("tst.file", false);
    Object* val1 = load(q);
    Object* val2 = load(q);
    Object* val3 = load(q);
    ck_assert(memcmp(val1->bytes, "Hi", 2) == 0);
    ck_assert(memcmp(val2->bytes, "Boo", 3) == 0);
    ck_assert(memcmp(val3->bytes, "Cow", 3) == 0);
    free_object(val1);
    free_object(val2);
    free_object(val3);
    free_persist(q);
    remove("tst.file");
}
END_TEST