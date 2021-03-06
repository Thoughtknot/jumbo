.PHONY: windows posix jni leak check clean

ifeq ($(OS),Windows_NT)
    LIBEXT := .dll
else
    LIBEXT := .so
endif

make:
	mkdir -p db
	gcc -Wall src/map.c -c -o src/map.o
	gcc -Wall src/persist.c -c -o src/persist.o
	gcc -Wall src/trie.c -c -o src/trie.o
	gcc -Wall src/list_files_posix.c -c -o src/list_files.o
	gcc -Wall src/client.c -c -o src/client.o
	gcc -Wall src/persistedmap.c -c -o src/persistedmap.o
	gcc -Wall src/jumbo.c -c -o src/jumbo.o
	gcc -pthread -Wall -o jumbo src/*.o

jni:
	gcc -fPIC src/map.c -c -o jni/libmap.o
	gcc -fPIC src/list_files_posix.c -c -o jni/liblist_files.o
	gcc -fPIC src/persist.c -c -o jni/libpersist.o
	gcc -fPIC src/persistedmap.c -c -o jni/libpersistedmap.o
	gcc -fPIC jni/jumbo_jni.c -I"${JAVA_HOME}/include" -I"${JAVA_HOME}/include/linux" -shared jni/*.o -o libjumbo_jni.so

jni_windows:
	gcc -fPIC src/map.c -c -o jni/libmap.o
	gcc -fPIC src/list_files_windows.c -c -o jni/liblist_files.o
	gcc -fPIC src/persist.c -c -o jni/libpersist.o
	gcc -fPIC src/persistedmap.c -c -o jni/libpersistedmap.o
	gcc -fPIC jni/jumbo_jni.c -I"${JAVA_HOME}/include" -I"${JAVA_HOME}/include/win32" -shared jni/*.o -o jumbo_jni.dll

leak:
	gcc test/leak_check.c -c -o leak_check.o
	gcc leak_check.o src/map.o -o leak_check
	valgrind --tool=memcheck --leak-check=yes -v ./leak_check

check:
	gcc test/check_trie.c -c -o test/check_trie.o
	gcc test/check_map.c -c -o test/check_map.o
	gcc test/check_client.c -c -o test/check_client.o
	gcc test/check_persist.c -c -o test/check_persist.o
	gcc src/client.o src/list_files.o src/persistedmap.o src/trie.o src/map.o src/persist.o test/*.o -o test_suite `pkg-config --cflags --libs check`
	./test_suite

clean:
	rm *.o test/*.o jni/*.o src/*.o  *.so jumbo test_suite leak_check
	rm -r db/
	rm -r dbtest/