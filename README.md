# COMP2521 Assignment 1 - Information Retrieval
### Written by Toshi Tabata, z5280803, 16/03/2020

 This program implements an information retrieval system using "tf-idf"s, a term
 weighting scheme as described [here](https://en.wikipedia.org/wiki/Tf-idf).

 The program creates an "inverted index" that creates a sorted list of
 filenames for every word in a collection of files. Using this inverted index,
 word(s) can be searched for across all the files, returning which files the
 word is contained in. This is sorted by summed tf-idf values, i.e. the word's
 importance for each file.

 The program uses a binary search tree (BST) to create and store the inverted
 index, and a linked list (List) to store and display a singular word's tf-idf
 or a group of words' summed tf-idf.
