#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

//  Struct wrapping string
//  @param word     Allocated array of characters
//  @param lenght   Lenght of word
typedef struct {
    char * word;
    size_t length;
    int indexInOrigin;
} string;

//  Node in binary tree
//  @param left                 Pointer to node on the left side
//  @param right                Pointer to node on the right side
//  @param word                 String in node
//  @param originPositionIndex  Index where word start in original sentence
struct wordNode {
    struct wordNode * left;
    struct wordNode * right;
    string * word;
    int originPositionIndex;
};

//  Print error message and exit program with value 1
void wrongInput (void) {
    printf("Nespravny vstup.\n");
    exit(EXIT_FAILURE);
}

//  Verify if parameter is defined whitespace
//  @param c    Compared character
//  @return     True if parameter is space or newline
bool isWhite (const char c) {
    return c == ' ' || c == '\n';
}

//  Verify if parameter is alpha
//  @param c    Compared character
//  @return     True if parameter is low or high alpha
bool isAlpha (const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

//  Transform high alpha to low equivalent
//  @param c    Compared character
//  @return     Lower aplha or original parameter
char toLower (const char c) {
    if (isAlpha(c)) {
        return c >= 'a' && c <= 'z' ? c : (char)(c + 'a' - 'A');
    }
    
    return c;
}

//  Transform newline to space character
//  @param c    Compared character
//  @return     Space or original character
char toSpace (const char c) {
    return c == '\n' ? ' ' : c;
}

//  Load sentence from standard input
//  @return     Allocated sentence wrapped in string struct
string * loadSentence (void) {
    string * sentence = (string*) malloc(sizeof(string));
    char * word = NULL;
    size_t mySize = 0, length = 0;
    
    printf("Text:\n");
    for (int i = 0; true; i++) {
        char c;
        int ref = scanf("%c", &c);
        
        if (ref != 1) wrongInput();
        c = toLower(c);
        
        if (c == '\n') {
            char m;
            
            if (scanf("%c", &m) != 1) wrongInput();
            if (m == c) break;
            else {
                ungetc(m, stdin);
                c = toSpace(c);
            }
        }
        if (!i && !isAlpha(c)) {
            i--;
            continue;
        }
        if (i && !isAlpha(c)) {
            char m;
            
            if (scanf("%c", &m) != 1) wrongInput();
            if (isAlpha(m)) {
                ungetc(m, stdin);
                c = ' ';
            }
            else {
                ungetc(m, stdin);
                i--;
                continue;
            }
        }
        
        if (!i) {
            word = (char*) malloc(sizeof(char));
            mySize = 1;
        }
        if (i >= mySize) {
            mySize *= 2;
            word = (char*) realloc(word, mySize * sizeof(char));
        }
        
        *(word + i) = c;
        length = i + 1;
    }
    
    sentence->word = word;
    sentence->length = length;
    if (!sentence->length) wrongInput();
    
    return sentence;
}

//  Select only words from sentence
//  @param sentence Sentence to be parsed
//  @return         Allocated square array with separated words
string ** separate (const string * sentence, size_t * length) {
    string ** result = (string**) malloc(sizeof(string*));
    string * sepString = (string*) malloc(sizeof(string));
    size_t wordSize = 1, resSize = 1;
    int lastSpaceIndex = -1;
    
    *length = 0;
    sepString->indexInOrigin = 0;
    sepString->length = 0;
    sepString->word = (char*) malloc(sizeof(char));
    
    for (int i = 0; i < sentence->length; i++) {
        char c = sentence->word[i];
        
        if (isAlpha(c)) {
            if (lastSpaceIndex >= wordSize) {
                wordSize *= 2;
                sepString->word = (char*)realloc(sepString->word, wordSize * sizeof(char));
            }
            
            sepString->length++;
            *(sepString->word + i - lastSpaceIndex - 1) = c;
        }
        else {
            if (*length >= resSize) {
                resSize *= 2;
                result = (string**) realloc(result, resSize * sizeof(string*));
            }
            
            *(result + *length) = sepString;
            ++*length;
            sepString = (string*) malloc(sizeof(string));
            sepString->indexInOrigin = i + 1;
            sepString->length = 0;
            sepString->word = (char*) malloc(sizeof(char));
            wordSize = 1;
            lastSpaceIndex = i;
        }
    }
    
    if (*length >= resSize) {
        resSize++;
        result = (string**) realloc(result, resSize * sizeof(string*));
    }
    *(result + *length) = sepString;
    ++*length;
    
    return result;
}

void printSentence(string ** sentence, const size_t length) {
    for (size_t i = 0; i < length; i++) {
        for (size_t k = 0; k < (*(sentence + i))->length; k++) {
            printf("%c", *((*(sentence + i))->word + k));
        }
        printf("\n");
    }
    
    exit(EXIT_SUCCESS);
}

//  Create binary tree from origin sentence
//  @param origin   Original sentence in one array
//  @return         Pointer to root of binary tree created from origin sentence
struct wordNode * parseSentence (const string * sentence) {
    size_t sepLength;
    struct wordNode * root = NULL;
    string ** separateWords = separate(sentence, &sepLength);
    
    printSentence(separateWords, sepLength);
    
//    sort
//    order to binary tree
    
    return root;
}

//	run
int main (void) {
    bool eof;
    string * sentenceOrigin = loadSentence();
    
    struct wordNode * root = parseSentence(sentenceOrigin);
    
//    for (int i = 0; true; i++) {
//        string * request;
//    }
    
    free(sentenceOrigin);
    
	return 0;
}
