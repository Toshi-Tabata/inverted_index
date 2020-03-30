#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedIndex.h"
#include "tree.h"

FileList newListNode(char *fileName, int totalWords);
TfIdfList newIdfListNode(char *fileName, double tfIdfSum);
TfIdfList insertAlphabetically(TfIdfList head, TfIdfList new);
TfIdfList insertOrdered(TfIdfList head, TfIdfList new);

// Creates and returns a new FileList node
FileList newListNode(char *fileName, int totalWords) {
    FileList new = malloc(sizeof(struct FileListNode));
    new->filename = fileName;
    new->tf = 1.0 / totalWords;
    new->next = NULL;
    return new;

}

// Create and returns a new TfIdfList node
TfIdfList newIdfListNode(char *fileName, double tfIdfSum) {
    TfIdfList new = malloc(sizeof(struct TfIdfNode));
    new->filename = fileName;
    new->next = NULL;
    new->tfIdfSum = tfIdfSum;
    return new;
}

// Insertion in an ordered list (head), sorted by descending TfIdfSum
TfIdfList insertOrdered(TfIdfList head, TfIdfList new) {
    TfIdfList curr = head;
    TfIdfList prev = NULL;

    // Inserting into empty list
    if (head == NULL) return new;

    while (curr != NULL) {

        // Insert in tfIdf order
        if (new->tfIdfSum > curr->tfIdfSum) {
            if (prev == NULL) {
                head = new;
            } else {
                prev->next = new;
            }

            new->next = curr;
            return head;

        // Insert alphabetically instead if TfIdfSums are identical
        // Continues the insert from curr, instead of the head of the list
        } else if (curr->tfIdfSum == new->tfIdfSum) {
            curr = insertAlphabetically(curr, new);

            // insertAlpha() returns the head if prev was NULL
            if (prev == NULL) return curr;

            // insertAlpha() returns the middle of the list and we need to reattach
            prev->next = curr;

            return head;
        }

        prev = curr;
        curr = curr->next;
    }

    // Insert at the end of the list
    if (prev != NULL) {
        prev->next = new;

    }
    return head;
}


// Insertion sort by increasing alphabetical order
TfIdfList insertAlphabetically(TfIdfList head, TfIdfList new) {
    TfIdfList curr = head;
    TfIdfList prev = NULL;

    // Loop through the head and insert the "new" TfIdfList node
    while (curr != NULL) {
        if (new->tfIdfSum != curr->tfIdfSum) {
            if (prev != NULL) {
                prev->next = new;
            }
            new->next = curr;
            return head;
        }

        // Insert with ascending fileName order
        if (strcmp(new->filename, curr->filename) < 0) {
            if (prev == NULL) head = new;
            else prev->next = curr;

            new->next = curr;
            return head;

        }

        prev = curr;
        curr = curr->next;
    }

    // Insert at the end of the list
    if (prev != NULL) {
        prev->next = new;

    }
    return head;

}


