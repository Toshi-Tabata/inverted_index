#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedIndex.h"
#include "list.h"

struct tree {
    TfIdfList file;
    struct tree *left;
    struct tree *right;
};
typedef struct tree *Tree;

// Function prototypes for creating the inverted index BST
InvertedIndexBST newTreeNode(char *word, char *filename, int totalWords);
void updateFileList(InvertedIndexBST root, char *filename, int totalWords);
InvertedIndexBST insertTreeNode(InvertedIndexBST root, char *word, char *filename, int totalWords);
InvertedIndexBST getWord(InvertedIndexBST root, char *word);

// Function prototypes for creating a TfIdfList BST
Tree newNode(TfIdfList file);
Tree insertBST(Tree root, TfIdfList file);
void makeSortedList(Tree root, TfIdfList *newList);

InvertedIndexBST newTreeNode(char *word, char *filename, int totalWords) {
    InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));
    new->word = mallocString(word);
    new->fileList = newListNode(filename, totalWords);
    new->left = NULL;
    new->right = NULL;
    return new;
}

// Creates the FileList
void updateFileList(InvertedIndexBST root, char *filename, int totalWords) {
    if (root->fileList == NULL) {
        root->fileList = newListNode(filename, totalWords);
    }

    FileList curr = root->fileList;
    FileList prev = NULL;
    // Find the correct node
    while (curr != NULL) {

        // If the filename already exists, update Tf instead
        if (strcmp(filename, curr->filename) == 0) {
            curr->tf = ((curr->tf * totalWords) + 1.0) / totalWords;
            return;

        } else if (strcmp(filename, curr->filename) < 0) {
            if (prev != NULL) {
                // Insert a new file list node in the middle of the list
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

// Returns the node in the BST with key "word"
InvertedIndexBST getWord(InvertedIndexBST root, char *word) {
    if (root == NULL) return NULL;
    if (strcmp(root->word, word) == 0) {
        return root;

    } else if (strcmp(word, root->word) < 0) {
        return getWord(root->left, word);

    } else { // (strcmp(word, root->word) > 0)
        return getWord(root->right, word);
    }
}

////// Functions for Tree in Part 2 of the Assignment //////

Tree newNode(TfIdfList file) {
    Tree new = malloc(sizeof(struct tree));
    new->file = newIdfListNode(file->filename, file->tfIdfSum); //newFileName
    new->left = NULL;
    new->right = NULL;
    free(file->filename);
    free(file);

    return new;
}

// Sorted by filename, if there is a filename repeat, update the TfIdf
// Free the TfIdfList file node for simplicity
Tree insertBST(Tree root, TfIdfList file) {
    if (root == NULL) {
        return newNode(file);
    }

    // If the filename is the same as the one to be inserted, update Tfidf
    if (strcmp(file->filename, root->file->filename) == 0) {
        root->file->tfIdfSum += file->tfIdfSum;
        free(file->filename);
        free(file);

        return root;
    } else if (strcmp(file->filename, root->file->filename) < 0) {
        root->left = insertBST(root->left, file);
    } else if (strcmp(file->filename, root->file->filename) > 0) {
        root->right = insertBST(root->right, file);
    }

    return root;
}

// Inserts in infix order
// Since the tree is already in alphabetical order, insertion by TfIdfSum order
// inserts it in TfIdfSum order first, then alphabetically second if equal sums
void makeSortedList(Tree root, TfIdfList *newList) {
    if (root == NULL) return;
    if (root->left != NULL) makeSortedList(root->left, newList);

    *newList = insertOrdered(*newList, root->file);

    if (root->right != NULL) makeSortedList(root->right, newList);
    free(root);
}