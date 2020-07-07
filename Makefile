.PHONY: make jni check clean

make:
	mkdir -p db
	gcc -Wall src/client.c -c -o client.o
	gcc -Wall src/map.c -c -o map.o
	gcc -Wall src/trie.c -c -o trie.o
	gcc -Wall src/jumbo.c -c -o jumbo.o
	gcc -Wall src/persist.c -c -o persist.o
	gcc -pthread -Wall jumbo.o client.o map.o trie.o persist.o -o jumbo

jni:
	gcc -fPIC src/map.c -c -o libmap.o
	gcc -fPIC src/persist.c -c -o libpersist.o
	gcc -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -shared libmap.o libpersist.o -o libjumbo_jni.so jni/jumbo_jni.c

check:
	gcc test/check_trie.c -c -o check_trie.o
	gcc test/check_map.c -c -o check_map.o
	gcc test/check_client.c -c -o check_client.o
	gcc test/check_persist.c -c -o check_persist.o
	gcc trie.o map.o client.o persist.o check_trie.o check_client.o check_map.o check_persist.o -o test_suite `pkg-config --cflags --libs check`
	./test_suite

clean:
	rm *.o *.so jumbo test_suite
	rm -r db/