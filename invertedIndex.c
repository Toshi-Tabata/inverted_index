#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "invertedIndex.h"
#include "list.h"
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

TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {
    InvertedIndexBST treeNode = getWord(tree, searchWord);
    if (treeNode == NULL) return NULL;

    double idf = getIdf(treeNode, D);

    TfIdfList newList = NULL;
    FileList curr = treeNode->fileList;

    while (curr != NULL) {
        double tfIdf = idf * curr->tf;
        TfIdfList new = newIdfList(curr->filename, tfIdf);
        newList = insertOrdered(newList, new);

        curr = curr->next;
    }

    return newList;

}


struct tree {
    TfIdfList file;
    struct tree *left;
    struct tree *right;
};
typedef struct tree *Tree;



TfIdfList newList(char *fileName, double tfIdfSum) {
    TfIdfList new = malloc(sizeof(struct TfIdfNode));
    new->filename = fileName;
    new->next = NULL;
    new->tfIdfSum = tfIdfSum;
}

Tree newNode(TfIdfList file) {
    Tree new = malloc(sizeof(struct tree));

    char *newFileName = malloc(100 * sizeof(char *));
    strcpy(newFileName, file->filename);

    new->file = newList(newFileName, file->tfIdfSum);
    new->left = NULL;
    new->right = NULL;
    free(file);
    return new;
}

void updateTfIdf(TfIdfList head, TfIdfList temp) {
    head->tfIdfSum += temp->tfIdfSum;
    free(temp);
}

// Sorted by filename, if there is a filename repeat, update the TfIdf
Tree insertBST(Tree root, TfIdfList file) {

    if (root == NULL) {
        return newNode(file);
    }

    // If the filename is the same as the one to be inserted, update tfidf
    if (strcmp(file->filename, root->file->filename) == 0) {
        updateTfIdf(root->file, file);

        return root;
    } else if (strcmp(file->filename, root->file->filename) < 0) {
        root->left = insertBST(root->left, file);
    } else if (strcmp(file->filename, root->file->filename) > 0) {
        root->right = insertBST(root->right, file);
    }

    return root;
}

// Prints in infix order
void printTree(Tree root) {
    if (root == NULL) return;
    printTree(root->left);
    printf("%s ", root->file->filename);
    printf("%lf\n", root->file->tfIdfSum);
    printTree(root->right);
}

// Prints in infix order
void makeSortedList(Tree root, TfIdfList *newList) {
    if (root == NULL) return;
    if (root->left != NULL) {

        makeSortedList(root->left, newList);
    }


    *newList = insertOrdered(*newList, root->file);

    if (root->right != NULL)
        makeSortedList(root->right, newList);

}

// Note: it was stated that O(n^2) is fine for this function
TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {

    Tree newTree = NULL;
    for (int i = 0; searchWords[i] != NULL; i++) {
        TfIdfList tempList = calculateTfIdf(tree, searchWords[i], D);

        TfIdfList curr = tempList;



        while (curr != NULL) {

            // Hold next since we call free(curr)
            TfIdfList next = curr->next;
            newTree = insertBST(newTree, curr);


            curr = next;
        }



    }

    printf("=== Tree Before sorting ===\n");
    printTree(newTree);
    printf("=== Tree Before sorting ===\n\n");

    TfIdfList sortedList = NULL;
    makeSortedList(newTree, &sortedList);
    return sortedList;
//    // Print the sorted list
//    TfIdfList curr = sortedList;
//    while (curr != NULL) {
//
//        printf("%lf\t %s\n",  curr->tfIdfSum, curr->filename);
//        curr = curr->next;
//    }


}