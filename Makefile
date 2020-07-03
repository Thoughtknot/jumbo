make:
	mkdir -p db
	gcc src/map.c -c -o map.o
	gcc src/trie.c -c -o trie.o
	gcc src/jumbo.c -c -o jumbo.o
	gcc src/persist.c -c -o persist.o
	gcc jumbo.o map.o trie.o persist.o -o jumbo

checks:
	gcc test/check_trie.c -c -o check_trie.o
	gcc test/check_map.c -c -o check_map.o
	gcc test/check_persist.c -c -o check_persist.o
	gcc trie.o map.o persist.o check_trie.o check_map.o check_persist.o -o test_suite `pkg-config --cflags --libs check`
	./test_suite

clean:
	rm *.o jumbo test_suite
	rm -r db/