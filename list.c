#include <stdio.h>
#include <stdlib.h>
#include "invertedIndex.h"
#include "tree.h"

#define NONE -1

FileList newListNode(char *fileName, int totalWords) {
    FileList new = malloc(sizeof(struct FileListNode));
    new->filename = fileName;
    new->tf = getTF(NONE, totalWords);
    new->next = NULL;
    return new;

}

void appendListNode(FileList head, char *filename) {

}

