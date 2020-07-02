#include <check.h>
#include "../src/trie.h"

START_TEST (test_trie_basic)
{
    Node* root = createNode();
    putVal(root, 4, "test", 2, "-1-");
    putVal(root, 4, "team", 5, "-heal");
    putVal(root, 4, "test", 2, "99");
    putVal(root, 5, "roast", 2, "RsR");
    delVal(root, 4, "team");
    char * val1 = getVal(root, 3, "tea");
    char * val2 = getVal(root, 4, "team");
    char * val3 = getVal(root, 4, "test");
    char * valr = getVal(root, 5, "roast");
    printf("Val: %s\n", val1);
    printf("Val: %s\n", val2);
    printf("Val: %s\n", val3);
    printf("Val: %s\n", valr);
    
    putVal(root, 3, "tea", 2, "44");
    char * val4 = getVal(root, 3, "tea");
    printf("Val: %s\n", val4);

    ck_assert_str_eq(val4, "44");
    free_node(root);
}
END_TEST

Suite * suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("trie");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_trie_basic);
    suite_add_tcase(s, tc_core);

    return s;
}

int main() {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}