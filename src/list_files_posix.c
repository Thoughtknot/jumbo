#include "list_files.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/stat.h>

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

    DIR* d = NULL;
    struct dirent *dir;
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        mkdir(directory, 0700);
        return contents;
    }
    d = opendir(directory);
    char* filenames[4096];
    int numFiles = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char v[strlen(dir->d_name)];
            strcpy(v, directory);
            strcat(v, "/");
            strcat(v, dir->d_name);
            if (strstr(dir->d_name, ".jmb")) {
                filenames[numFiles] = (char*) malloc(strlen(dir->d_name) + 1);
                strcpy(filenames[numFiles], dir->d_name);
                ++numFiles;
            }
        }
        closedir(d);
    }
    contents->fileNames = (char**) calloc(numFiles, sizeof(char *));
    for (int i = 0; i < numFiles; i++) {
        contents->fileNames[i] = filenames[i];
        printf("Found file: %s\n", contents->fileNames[i]);
    }
    contents->numFiles = numFiles;
    return contents;
}