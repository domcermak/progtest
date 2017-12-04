#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

//
//  Struct wrapping string
//  @param word     Allocated array of characters
//  @param lenght   Lenght of word
//
typedef struct {
    char * word;
    size_t length;
} string;

//
//  Node in binary tree
//  @param left                 Pointer to node on the left side
//  @param right                Pointer to node on the right side
//  @param word                 String in node
//  @param originPositionIndex  Index where word start in original sentence
//
struct wordNode {
    struct wordNode * left;
    struct wordNode * right;
    string * word;
    int originPositionIndex;
};

//
//  Print error message and exit program with value 1
//
void wrongInput (void) {
    printf("Nespravny vstup.\n");
    exit(EXIT_FAILURE);
}

//
//  Verify if parameter is defined whitespace
//  @param c    Compared character
//  @return     True if parameter is space or newline
//
bool isWhite (const char c) {
    return c == ' ' || c == '\n';
}

//
//  Verify if parameter is alpha
//  @param c    Compared character
//  @return     True if parameter is low or high alpha
//
bool isAlpha (const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

//
//  Transform verified alpha to its lower version
//  @param c    Compared character
//  @return     Low version of original parameter or the parameter if it is already in low version
//
char toLower (const char c) {
    return c >= 'a' && c <= 'z' ? c : (char)(c - 'a' + 'A');
}

//
//  Load sentence from standard input
//  @return     Allocated sentence wrapped in string struct
//
string * loadSentence (void) {
    string * sentence = (string*) malloc(sizeof(string));
    char * word = NULL;
    char * lastc = NULL;
    size_t mySize = 1, length = 0;
    
    for (int i = 0; true; i++) {
        char c;
        int ref = scanf("%c", &c);
        
        if (ref != 1) wrongInput();
        if ((!isAlpha(c) && !isWhite(c)) || (!i && c == ' ')) {
            i--;
            
            continue;
        }
        if (!i) word = (char*) malloc(sizeof(char));
        if (*lastc == '\n' && c == '\n') {
            *(word + i - 1) = '\0';
            length--;
            
            break;
        }
        
        length = i + 1;
        if (i >= mySize) {
            mySize *= 2;
            word = (char*) realloc(word, mySize * sizeof(char));
        }
        *(word + i) = c;
        *lastc = c;
    }
    
    sentence->word = word;
    sentence->length = length;
    
    return sentence;
}

//
//  Select only words from sentence
//  @param sentence Sentence to be parsed
//  @return         Allocated square array with separated words
//
string ** separate (const string * sentence) {
    string ** result = NULL;
    
//    create new word when whitespace occured
    
    return result;
}

//  Create binary tree from origin sentence
//  @param origin   Original sentence in one array
//  @return         Pointer to root of binary tree created from origin sentence
//
struct wordNode * parseSentence (const string * sentence) {
    struct wordNode * root;
    string ** separateWords = separate(sentence);
    
//    sort
//    order to binary tree
    
    return root;
}

//	run
int main (void) {
    string * sentenceOrigin = loadSentence();
    struct wordNode * root = parseSentence(sentenceOrigin);
    
    
    string * request = loadSentence();


	return 0;
}
