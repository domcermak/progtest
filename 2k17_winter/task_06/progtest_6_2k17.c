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
    int * indexInOrigin;
} string;

//  Print error message and exit program with value 1
void wrongInput (void) {
    printf("Nespravny vstup.\n");
    exit(EXIT_FAILURE);
}

//  Verify if parameter is alpha
//  @param c    Compared character
//  @return     True if parameter is low or high alpha
static bool isAlpha (const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

//  Transform high alpha to low equivalent
//  @param c    Compared character
//  @return     Lower aplha or original parameter
static char toLower (const char c) {
    return c >= 'A' && c <= 'Z' ? (char)(c + 'a' - 'A') : c;
}

//  Transform newline to space character
//  @param c    Compared character
//  @return     Space or original character
static char toSpace (const char c) {
    return c == '\n' ? ' ' : c;
}

#ifndef __PROGTEST__
static void printSentence(string ** wordList, const size_t length) {
    for (size_t i = 0; i < length; i++) {
        string * word = *(wordList + i);
        
        for (size_t k = 0; k < word->length; k++) {
            printf("%c", *(word->word + k));
        }
        
        printf(" -> %d\n", *word->indexInOrigin);
    }
    
    exit(EXIT_SUCCESS);
}
#endif // __PROGTEST__

//  Load sentence from standard input
//  @return     Allocated sentence wrapped in string struct
static string * loadSentence (void) {
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
    sentence->indexInOrigin = NULL;
    if (!sentence->length) wrongInput();
    
    return sentence;
}

//  Select only words from sentence
//  @param sentence Sentence to be parsed
//  @param length   Count of strings in separated array
//  @return         Allocated square array with separated words
static string ** separate (const string * sentence, size_t * length) {
    string ** result = (string**) malloc(sizeof(string*));
    string * sepString = (string*) malloc(sizeof(string));
    size_t wordSize = 1, resSize = 1;
    int lastSpaceIndex = -1;
    
    *length = 0;
    sepString->indexInOrigin = (int*) malloc(sizeof(int));
    *sepString->indexInOrigin = 0;
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
            sepString->indexInOrigin = (int*) malloc(sizeof(int));
            *sepString->indexInOrigin = i + 1;
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

//  Compare function created for quick sort function to sort items type of string**
//  @param a Compared item 1
//  @param b Compared item 2
//  @return 0 when equal, >0 if a > b, <0 if a < b
static int compare (const void * a, const void * b) {
    string * ia = *(string**)a, *ib = *(string**)b;
    size_t len = ia->length;
    
    if (ia->length != ib->length)
        len = ia->length > ib->length ? ib->length : ia->length;

    for (size_t i = 0; i < len; i++)
        if (*(ia->word + i) != *(ib->word + i))
            return *(ia->word + i) > *(ib->word + i) ? 1 : -1;
    
    if (ia->length != ib->length)
        return (int)(ia->length > ib->length ? ia->length : ib->length);
    
    return 0;
}

//  Create binary tree from origin sentence
//  @param origin   Original sentence in one array
//  @return         Pointer to root of binary tree created from origin sentence
static string * parseSentence (const string * sentence) {
    size_t sepLength;
    string * list = NULL;
    string ** separateWords = separate(sentence, &sepLength);
    
    qsort(separateWords, sepLength, sizeof(string*), compare);
    
//    unique
    
    return list;
}

//	run
int main (void) {
    string * sentenceOrigin = loadSentence();
    string * list = parseSentence(sentenceOrigin);
    
//    for (int i = 0; true; i++) {
//        string * request;
//    }
    
    free(sentenceOrigin);
    
	return 0;
}
