#include "invertedIndex.h"

// For a TfIdfList BST
struct tree {
    TfIdfList file;
    struct tree *left;
    struct tree *right;
};
typedef struct tree *Tree;

// For a TfIdfList BST
Tree newNode(TfIdfList file);
Tree insertBST(Tree root, TfIdfList file);
void makeSortedList(Tree root, TfIdfList *newList);

// For an InvertedIndex BST
InvertedIndexBST insertTreeNode(InvertedIndexBST root, char *word, char *filename, int totalWords);
InvertedIndexBST getWord(InvertedIndexBST root, char *word);