#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie structure
struct Trie
{
    struct Trie* children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Initializes a trie structure
struct Trie* createTrie()
{
    struct Trie* pNode = (struct Trie*)malloc(sizeof(struct Trie));
    pNode->isEndOfWord = 0;
    pNode->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->children[i] = NULL;
    }
    return pNode;
}

// Inserts the word into the trie structure
void insert(struct Trie* pTrie, char* word)
{
    struct Trie* pNode = pTrie;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (pNode->children[index] == NULL) {
            pNode->children[index] = createTrie();
        }
        pNode = pNode->children[index];
    }
    pNode->isEndOfWord = 1;
    pNode->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie* pTrie, char* word)
{
    struct Trie* pNode = pTrie;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (pNode->children[index] == NULL) {
            return 0;
        }
        pNode = pNode->children[index];
    }
    if (pNode != NULL && pNode->isEndOfWord) {
        return pNode->count;
    }
    else {
        return 0;
    }
}

// Deallocate the trie structure
void deallocateTrie(struct Trie* pTrie)
{
    if (pTrie == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (pTrie->children[i] != NULL) {
            deallocateTrie(pTrie->children[i]);
        }
    }
    free(pTrie);
}

// Reads the dictionary from file
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int numWords = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; // remove newline character
        pInWords[numWords] = strdup(buffer);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Test some words
    char* pWords[] = { "notaword", "ucf", "no", "note", "corg" };
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the trie
    deallocateTrie(pTrie);
    printf("Trie deallocated.\n");

    return 0;
}


