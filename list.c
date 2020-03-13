#include <stdio.h>
#include <stdlib.h>
#include "invertedIndex.h"
#include "tree.h"


FileList newListNode(char *fileName, int totalWords) {
    FileList new = malloc(sizeof(struct FileListNode));
    new->filename = fileName;

    // getTF(0, totalWords); - this passes a random number to getTF instead of totalWords for some reason
    new->tf = 1.0 / totalWords;
    new->next = NULL;
    return new;

}

void appendListNode(FileList head, char *filename) {

}

