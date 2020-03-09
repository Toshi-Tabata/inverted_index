#include <stdio.h>
#include "invertedIndex.h"
#include <string.h>
#include <ctype.h>

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

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {

}
