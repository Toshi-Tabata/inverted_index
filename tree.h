#include "invertedIndex.h"
InvertedIndexBST newTreeNode(char *word, char *filename);
InvertedIndexBST insertTreeNode(InvertedIndexBST root, char *word, char *filename, int totalWords);
double getTF(float currTF, int totalWords);
InvertedIndexBST getWord(InvertedIndexBST root, char *word);