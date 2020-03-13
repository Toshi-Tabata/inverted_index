#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "invertedIndex.h"
//#include "list.h"
#include "tree.h"

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

// Normalises a string by altering it and removing:
//  `.`, `,`, `;`, `?` when they are the last character of the given string
//  leading/trailing spaces
//  and converting everything to lowercase
char *normaliseWord(char *str) {
    int i = 0;
    strip_spaces(str);
    int length = strlen(str);

    // Remove the last punctuation mark
    if (str[length - 1] == '.' ||
        str[length - 1] == ',' ||
        str[length - 1] == '?' ||
        str[length - 1] == ';') {

        str[length - 1] = '\0';
    }

    for (; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }

    return str;
}

// Returns number of words in a file for calculating `tf`
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

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    InvertedIndexBST node = NULL;
    InvertedIndexBST root = NULL;

    // Open the collection file
    FILE *collectionP;
    char filename[1000];

    collectionP = fopen(collectionFilename, "rb");

    if (collectionP == NULL) {
        exit(1);
    }

    // Generate the inverted index, file by file
    while (fscanf(collectionP, "%s", filename) != EOF) {
        // Get number of words in file so the `tf` can be calculated
        // (2n) regardless of which order these are done, this is just simpler
        int totalWords = numWords(filename);

        // Open the file
        FILE *fp;
        char words[1000];
        fp = fopen(filename, "r");
        char *currFile = malloc(sizeof(filename));
        strcpy(currFile, filename);

        // If we can't open the file, we skip it
        //  TODO: check if specs require special handling
        if (fp == NULL) continue;

        // Iterate through all the words in the file
        // remove white spaces and symbols etc. using normaliseWord
        while (fscanf(fp, "%s", words) != EOF) {
            normaliseWord(words);
            char *currWord = malloc(sizeof(words));
            strcpy(currWord, words);
            if (root == NULL) {

                root = insertTreeNode(NULL, currWord, currFile, totalWords);
                node = root;
            } else {
                node = insertTreeNode(node, currWord, currFile, totalWords);
            }

//             TODO: add the words to a BST
            // TODO: find the correct place to insert
            //  if we find the same word on the way there, update `tf`
            //  else if left/right == NULL, create a new node and insert
            // Should be done but idk


        }

        fclose(fp);

    }
    fclose(collectionP);

    return root;

}

void printInvertedIndex(InvertedIndexBST tree) {
    if (tree == NULL) {
        return;
    }

    printInvertedIndex(tree->left);
    printf("%s ", tree->word);
    FileList curr = tree->fileList;
    while (curr != NULL) {
        printf("%s ", curr->filename);
        curr = curr->next;
    }
    printf("\n");
    printInvertedIndex(tree->right);
}

// TODO: remove this later, this is only for debugging
void printTF(InvertedIndexBST tree) {
    if (tree == NULL) {
        return;
    }

    printTF(tree->left);
    printf("%s ", tree->word);
    FileList curr = tree->fileList;
    while (curr != NULL) {
        printf("%s ", curr->filename);
        printf("%lf ", curr->tf);
        curr = curr->next;
    }
    printf("\n");
    printTF(tree->right);

}

// Returns tfIdf of the given treeNode (tfIdf of the word)
double getTfIdf(InvertedIndexBST treeNode, int D) {
    // Count number of files inside of the list
    FileList curr = treeNode->fileList;
    double count = 0;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    printf("count: %lf\n", count);
    return log10(D / count);
}

TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {
    InvertedIndexBST treeNode = getWord(tree, searchWord);
    if (treeNode == NULL) return NULL;
    printf("tf for %s was %lf\n", treeNode->fileList->next->filename, treeNode->fileList->next->tf);
    printf("D was %d, and tfIdf was %lf\n", D, getTfIdf(treeNode, D));




}