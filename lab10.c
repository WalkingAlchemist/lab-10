#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
} TrieNode;

// Initializes a Trie node
TrieNode *createTrieNode()
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node != NULL)
    {
        node->isEndOfWord = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts the word into the Trie
void insert(TrieNode *root, char *word)
{
    TrieNode *current = root;
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
}

// Searches for the word in the Trie and returns the number of occurrences
int numberOfOccurrences(TrieNode *root, char *word)
{
    TrieNode *current = root;
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        int index = word[i] - 'a';
        if (current->children[index] == NULL)
        {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    return current->isEndOfWord;
}

// Deallocates the Trie structure
void deallocateTrie(TrieNode *root)
{
    if (root != NULL)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            deallocateTrie(root->children[i]);
        }
        free(root);
    }
}

// Initializes a trie structure
TrieNode *createTrie()
{
    return createTrieNode();
}

// Reads the dictionary file and returns the number of words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        pInWords[count] = strdup(buffer);
        count++;
    }

    fclose(file);
    return count;
}

int main(void)
{
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    TrieNode *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Test the Trie with some words
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate memory for Trie
    deallocateTrie(pTrie);

    return 0;
}