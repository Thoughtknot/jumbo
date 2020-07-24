#include <check.h>
#include <time.h>
#include <sys/stat.h>
#include "../src/persistedmap.h"

START_TEST (test_client_basic)
{
    char directory[] = "dbtest";
    struct stat st = {0};
    PersistedMap** pms = build_maps(directory, 1);
    PersistedMap* pm = getTable(directory, 0, pms, 100);
    persist_and_put(pm, 2, "ap", 3, "too");
    persist_and_put(pm, 2, "ap", 3, "key");
    stat(pm->persist->name, &st);
    int sizeBf = st.st_size;
    printf("Stat %s, %d\n", pm->persist->name, sizeBf);
    compact_map(pm);
    stat(pm->persist->name, &st);
    int sizeAfter = st.st_size;
    free_maps(pms, 1);
    ck_assert(sizeBf > sizeAfter);
}
END_TEST
