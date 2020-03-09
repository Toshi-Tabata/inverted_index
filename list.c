#include <stdio.h>
#include <stdlib.h>
#include "invertedIndex.h"

#define NONE -1

FileList newListNode(char *fileName) {
    FileList new = malloc(sizeof(struct FileListNode));
    new->filename = fileName;
    new->tf = NONE;
    new->next = NULL;
    return new;

}

void appendListNode(FileList head, char *filename) {

}

