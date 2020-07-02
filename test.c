#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

int main() {
    char filename[50];
    strcpy(filename, "db/");
    char prefix[6]; 
    sprintf(prefix, "%d", 32);
    strcat(filename, prefix);
    strcat(filename, ".jmb");
    printf("%s\n", filename);
}