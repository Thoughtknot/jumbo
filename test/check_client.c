#include <check.h>
#include <time.h>
#include "../src/persistedmap.h"

START_TEST (test_client_basic)
{
    PersistedMap** pms = (PersistedMap**) calloc(1, sizeof(PersistedMap*));
    pms[0] = NULL;

    PersistedMap* pm = getTable(0, pms, 100);
    char a[100][1];
    for (int i = 0; i < 100; i++) {
        a[i][0] = i;
    }

    /*
    clock_t start = clock(), diff;
    for (int i = 0; i < 10000; i++) {
        persist_and_put(pm, 1, a[i % 100], 13, "Hello world!");
    }
    diff = (double)(clock() - start) * 1000000 / CLOCKS_PER_SEC;
    printf("Clock: %ld\n", diff);
    */
}
END_TEST