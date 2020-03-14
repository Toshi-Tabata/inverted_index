#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedIndex.h"
#include "list.h"




InvertedIndexBST newTreeNode(char *word, char *filename, int totalWords) {
    InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));
    new->word = word;
    new->fileList = newListNode(filename, totalWords);
    new->left = NULL;
    new->right = NULL;
    return new;
}

// Returns the new term frequency based on the previous TF
double getTF(double currTF, int totalWords) {
    return ((currTF * totalWords) + 1.0) / totalWords;

}

void updateFileList(InvertedIndexBST root, char *filename, int totalWords) {
    if (root->fileList == NULL) {
        root->fileList = newListNode(filename, totalWords);
    }

    FileList curr = root->fileList;
    FileList prev = NULL;
    // Find the correct node
    while (curr != NULL) {
        if (strcmp(filename, curr->filename) == 0) {

            curr->tf = getTF(curr->tf, totalWords); // getTF(root->fileList->tf, totalWords);
            return;
        } else if (strcmp(filename, curr->filename) < 0) {
            // Insert a new file list node in the middle of the list
            if (prev != NULL) {

                prev->next = newListNode(filename, totalWords);
                prev->next->next = curr;
            } else {
                // insert file list node at the head of the list

                root->fileList = newListNode(filename, totalWords);
                root->fileList->next = curr;

            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    // add to the end of the list
    if (prev != NULL) {

        prev->next = newListNode(filename, totalWords);
    }
}

// Finds the correct location to insert
// Updates `tf` if the same word is found
InvertedIndexBST insertTreeNode(InvertedIndexBST root, char *word, char *filename, int totalWords) {
    if (root == NULL) {
        return newTreeNode(word, filename, totalWords);
    }

    // Word was smaller than the current root
    if (strcmp(word, root->word) > 0) {
        root->right = insertTreeNode(root->right, word, filename, totalWords);
    } else if (strcmp(word, root->word) < 0) {
        root->left = insertTreeNode(root->left, word, filename, totalWords);
    } else if (strcmp(word, root->word) == 0) {
        updateFileList(root, filename, totalWords);
    }

    return root;
}

InvertedIndexBST getWord(InvertedIndexBST root, char *word) {
    if (root == NULL) return NULL;
    if (strcmp(root->word, word) == 0) {
        return root;
    }

    if (strcmp(word, root->word) < 0) {
        return getWord(root->left, word);
    }

    if (strcmp(word, root->word) > 0){
        return getWord(root->right, word);
    }


}

