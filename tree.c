#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedIndex.h"
#include "list.h"

// Inserts tree node in BST order
// TODO: need to make it so that the fileList gets correctly updates
InvertedIndexBST newTreeNode(char *word, char *filename) {
    InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));
    new->word = word;
    new->fileList = newListNode(filename);
    new->left = NULL;
    new->right = NULL;
    return new;
}

// Returns the new term frequency based on the previous TF
double getTF(float currTF, int totalWords) {
    if (currTF == NONE) {
        return 1.0 / totalWords;
    } else {
        return ((currTF * totalWords) + 1.0) / totalWords;
    }
}

// TODO: pass in InvertedIndexBST instead of filelistnode
void updateFileList(InvertedIndexBST root, char *filename, int totalWords) {
    if (root->fileList == NULL) {
        root->fileList = newListNode(filename);
    }

    FileList curr = root->fileList;
    FileList prev = NULL;
    // Find the correct node
    while (curr != NULL) {
        if (strcmp(filename, curr->filename) == 0) {
            root->fileList->tf = getTF(root->fileList->tf, totalWords);
            return;
        } else if (strcmp(filename, curr->filename) < 0) {
            // Insert a new file list node in the middle of the list
            if (prev != NULL) {
                prev->next = newListNode(filename);
                prev->next->next = curr;
            } else {
                // insert file list node at the head of the list
                root->fileList = newListNode(filename);
                root->fileList->next = curr;

            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    // add to the end of the list
    if (prev != NULL) {
        prev->next = newListNode(filename);
    }
}

// Finds the correct location to insert
// Updates `tf` if the same word is found
InvertedIndexBST insertTreeNode(InvertedIndexBST root, char *word, char *filename, int totalWords) {
    if (root == NULL) {
        return newTreeNode(word, filename);
    }

    // Word was smaller than the current root
    if (strcmp(word, root->word) > 0) {
        root->right = insertTreeNode(root->right, word, filename, totalWords);
    } else if (strcmp(word, root->word) < 0) {
        root->left = insertTreeNode(root->left, word, filename, totalWords);
    } else if (strcmp(word, root->word) == 0) {
        // TODO: navigate through the linked list, find the current file, if we reach NULL, add a new file
        //  else, update the td
        updateFileList(root, filename, totalWords);
    }

    return root;
}