
// Create a FileList node
FileList newListNode(char *fileName, int totalWords);

// Function for insertion of a TfIdfList node in insertion sort
TfIdfList insertOrdered(TfIdfList head, TfIdfList new);

// Create and returns a new TfIdfList node
TfIdfList newIdfListNode(char *fileName, double tfIdfSum);

// Returns a malloced copy of the given string
char *mallocString(char *str);