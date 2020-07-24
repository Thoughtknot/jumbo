#include <stdlib.h>

typedef struct DirectoryContents DirectoryContents;
struct DirectoryContents {
    char** fileNames;
    char* dirName;
    int numFiles;
};

DirectoryContents* list_directory_contents(const char *directory);
void free_contents(DirectoryContents* contents);