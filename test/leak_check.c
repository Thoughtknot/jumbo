#include "../src/map.h"

int main() {
    HashMap* map = create_map(10);
    put(map, 5, "banan", 3, "cow");
    put(map, 3, "apa", 2, "ko");
    del(map, 2, "ap");
    put(map, 3, "apa", 2, "ts");
    map = resize(map);
    put(map, 5, "afafa", 3, "noo");
    free_map(map);
}