//
//  Created by Dominik Čermák as a part of http://www.github.com/domcermak/progtest
//  Copyright © 2017 Dominik Čermák. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


//
//  Struct of map of fields with prices
//  @param lenght
//  @param width
//  @param fieldPrice
//
typedef struct {
    int length;
    int width;
    int ** fieldPrice;
} map;


//
//Print error message and quit program
//
static void wrongInput (void) {
    printf("Nespravny vstup.\n");
    exit(EXIT_FAILURE);
}

//
//  Load sizes of map
//  @param myMap Map struct pointer
//
static void loadMapSize (map * myMap) {
    printf("Velikost:\n");
    if (scanf("%d %d", &myMap->length, &myMap->width) != 1
        || myMap->length < 1
        || myMap->width < 1)
        wrongInput();
}


//
//  Load map of prices from stdin of size length x width in myMap
//  @param myMap Map struct pointer with filled lenght and width
//
static void loadMapValues (map * myMap) {
    printf("Cena:\n");
    myMap->fieldPrice = malloc(myMap->length * sizeof(int*));
    for (int i = 0; i < myMap->length; i++) {
        *(myMap->fieldPrice + i) = malloc(myMap->width * sizeof(int));
        for (int k = 0; k < myMap->width; i++) {
            if (scanf("%d", &(*(*(myMap->fieldPrice + i) + k))) != 1
                || *(*(myMap->fieldPrice + i) + k) < 1)
                wrongInput();
        }
    }
}

//  run
int main(void) {
    map myMap;
    
    loadMapSize(&myMap);
    
    
    
    
    return 0;
}
