#include "list_files.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

void free_contents(DirectoryContents* contents) {
    for (int i = 0; i < contents->numFiles; i++) {
        free(contents->fileNames[i]);
    }
    free(contents->fileNames);
    free(contents->dirName);
    free(contents);
}

DirectoryContents* list_directory_contents(const char *directory) {
    DirectoryContents* contents = (DirectoryContents*) malloc(sizeof(DirectoryContents));
    contents->dirName = (char*) malloc(strlen(directory) + 1);
    contents->fileNames = NULL;
    contents->numFiles = 0;
    strcpy(contents->dirName, directory);

    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
    
    char sPath[2048];
    sprintf(sPath, "%s\\*.jmb", directory);
    char* filenames[4096];
    int numFiles = 0;
    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Path empty: [%s]\n", directory);
        return contents;
    }
    do {
        sprintf(sPath, "%s\\%s", directory, fdFile.cFileName);
        printf("Found file: %s\n", sPath);
        filenames[numFiles] = (char*) malloc(strlen(sPath) + 1);
        strcpy(filenames[numFiles], sPath);
        ++numFiles;
    }
    while(FindNextFile(hFind, &fdFile));

    contents->fileNames = (char**) calloc(numFiles, sizeof(char *));
    for (int i = 0; i < numFiles; i++) {
        contents->fileNames[i] = (char*) malloc(strlen(filenames[i] + 1));
        strcpy(contents->fileNames[i], filenames[i]);
    }
    contents->numFiles = numFiles;

    FindClose(hFind);
    return contents;
}