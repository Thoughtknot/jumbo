make:
	mkdir -p db
	gcc map.c -c -o map.o
	gcc trie.c -c -o trie.o
	gcc jumbo.c -c -o jumbo.o
	gcc persist.c -c -o persist.o
	gcc jumbo.o map.o trie.o persist.o -o jumbo

clean:
	rm *.o jumbo
	rm -r db/