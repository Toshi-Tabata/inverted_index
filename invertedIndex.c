// COMP2521 Assignment 1 - Information Retrieval
// Written by Toshi Tabata, z5280803, 16/03/2020
// Program implements an information retrieval system using "tf-idf"s, a term
// weighting scheme as described at https://en.wikipedia.org/wiki/Tf-idf.

// The program creates an "inverted index" that creates a sorted list of
// filenames for every word in a collection of files. Using this inverted index,
// word(s) can be searched for across all the files, returning which files the
// word is contained in. This is sorted by summed tf-idf values, i.e. the word's
// importance for each file.

// The program uses a binary search tree (BST) to create and store the inverted
// index, and a linked list (List) to store and display a singular word's tf-idf
// or a group of words' summed tf-idf.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "invertedIndex.h"
#include "list.h"
#include "tree.h"

void strip_spaces(char *str);
int numWords(char *filename);
FILE *openFile(char *filename);
void printList(InvertedIndexBST root, FILE *fp);
double getIdf(InvertedIndexBST treeNode, int D);
void parseFile(char *filename, InvertedIndexBST *root);
void printInvertedIndexHelper(InvertedIndexBST tree, FILE *fp);

// Normalises a string by altering it and removing:
//  `.`, `,`, `;`, `?` when they are the last character of the given string
//  leading/trailing spaces
//  and converting everything to lowercase
char *normaliseWord(char *str) {

    // Remove leading/trailing spaces
    strip_spaces(str);

    // Remove the last punctuation mark
    int length = strlen(str);
    if (str[length - 1] == '.' ||
        str[length - 1] == ',' ||
        str[length - 1] == '?' ||
        str[length - 1] == ';')
    {
        str[length - 1] = '\0';
    }

    // Converts letters to lowercase
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }

    return str;
}

// Creates an Inverted Index stored in a BST
/*
How it works:
    1. Parses the collectionFilename file to get all the filenames to be parsed
    2. Parses each file given:
        a) Normalises all the words in the file
        b) Inserts it into a BST sorted alphabetically by filename
    3. Returns the InvertedIndexBST
*/
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    InvertedIndexBST root = NULL;

    // Open the collection file
    FILE *collectionP = openFile(collectionFilename);
    char filename[1000];

    // Generate the inverted index, file by file
    while (fscanf(collectionP, "%s", filename) != EOF) {

        // Normalise words, get its tf, then inserts into BST
        parseFile(filename, &root);
    }
    fclose(collectionP);
    return root;
}

// Creates the invertedIndex file and calls the helper to write to it
void printInvertedIndex(InvertedIndexBST tree) {
    FILE *fp = fopen("invertedIndex.txt", "w+");
    // Couldn't open file
    if (fp == NULL) exit(1);

    printInvertedIndexHelper(tree, fp);

    fclose(fp);
}

// Creates a TfIdfList from an existing InvertedIndexBST
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {

    // Get the tree node with the desired searchWord
    InvertedIndexBST treeNode = getWord(tree, searchWord);
    if (treeNode == NULL) return NULL;

    // Get the idf of the searchNode
    double idf = getIdf(treeNode, D);

    // Calculate the TfIdf and create the TfIdfList for each FileList node
    TfIdfList newList = NULL;
    FileList curr = treeNode->fileList;
    while (curr != NULL) {
        double tfIdf = idf * curr->tf;
        TfIdfList new = newIdfListNode(curr->filename, tfIdf);
        newList = insertOrdered(newList, new);

        curr = curr->next;
    }
    return newList;
}


// Note: it was stated that O(n^2) is fine for this function
// Returns a TfIdfList
/*
How it works:
    For each searchWord:
    1. Create its TfIdf list
    2. Insert it into a BST, sorted alphabetically by filename
    3. Insertion sort into the final list by decreasing TfIdfSum

    On BST insertion, if the filename is repeated, calculate the TfIdfSum.
    Due to how insertion sort works, the file is both alphabetically sorted and
    decreasing TfIdfSum sorted.
 */
TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {
    Tree newTree = NULL;
    for (int i = 0; searchWords[i] != NULL; i++) {
        TfIdfList tempList = calculateTfIdf(tree, searchWords[i], D);
        TfIdfList curr = tempList;

        // Insert each node of tempList into a BST
        while (curr != NULL) {

            // Hold next since we call free(curr)
            TfIdfList next = curr->next;
            newTree = insertBST(newTree, curr);
            curr = next;
        }
    }

    // Traverse BST in-order and insertion sort each TfIdfList node into sortedList
    TfIdfList sortedList = NULL;
    makeSortedList(newTree, &sortedList);
    return sortedList;
}

////// Helper Functions //////

// Removes all leading and trailing spaces
// Assumes that tabs and newlines do not count
void strip_spaces(char *str) {

    // Remove trailing spaces
    int length = strlen(str);
    while (str[length - 1] == ' ' || str[length - 1] == '\n' || str[length - 1] == '\t') {
        length--;
    }
    str[length] = '\0';

    // Get the index of the first non-whitespace character
    int startIndex = 0;
    while (str[startIndex] == ' ' || str[startIndex] == '\n' || str[startIndex] == '\t') {
        startIndex++;
    }

    // Shift all chars to the start and overwrite spaces
    int currIndex = 0;
    while (str[currIndex + startIndex] != '\0'){
        str[currIndex] = str[currIndex + startIndex];
        currIndex++;
    }
    str[currIndex] = '\0';
}

// Returns number of words in a file for calculating `tf`
// Note: (2n) time regardless of which order these are done, this is just simpler
int numWords(char *filename) {
    FILE *fp;
    char words[1000];
    fp = fopen(filename, "r");
    if (fp == NULL) return 0;

    int count = 0;
    while (fscanf(fp, "%s", words) != EOF) {
        count++;

    }
    fclose(fp);
    return count;
}

// Normalises and gets the tf of each word in the file, then inserts it into BST
void parseFile(char *filename, InvertedIndexBST *root) {

    // Get number of words in file so the `tf` can be calculated
    int totalWords = numWords(filename);

    // Open the file
    FILE *fp = openFile(filename);

    // Iterate through all the words in the file
    char words[1000];
    while (fscanf(fp, "%s", words) != EOF) {
        normaliseWord(words);
        *root = insertTreeNode(*root, words, filename, totalWords);
    }
    fclose(fp);
}

// Opens the given file and error checks
FILE *openFile(char *filename) {
    FILE *fp = fopen(filename, "rb");
    // Couldn't open file
    if (fp == NULL) exit(1);

    return fp;
}

// Prints the tree in infix order
void printInvertedIndexHelper(InvertedIndexBST tree, FILE *fp) {
    if (tree == NULL) return;
    printInvertedIndexHelper(tree->left, fp);
    printList(tree, fp);
    printInvertedIndexHelper(tree->right, fp);
}

// Prints out the InvertedIndex node to stdout with correct formatting
void printList(InvertedIndexBST root, FILE *fp) {
    fprintf(fp, "%s ", root->word);

    // Traverses through the fileList of a given tree node and prints it
    FileList curr = root->fileList;
    while (curr != NULL) {
        fprintf(fp, "%s ", curr->filename);
        curr = curr->next;
    }

    fprintf(fp, "\n");
}

// Returns TfIdf of the given treeNode (TfIdf of the word)
// Counts the number of files a word is in to find the Idf
double getIdf(InvertedIndexBST treeNode, int D) {
    // Count number of files inside of the list
    FileList curr = treeNode->fileList;
    double count = 0;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }

    return log10(D / count);
}

