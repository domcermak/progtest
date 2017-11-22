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


#ifndef __PROGTEST__

//
//  Print formated values of map to stdout
//  @param myMap Map containing values
//
void print (const map * myMap) {
    printf("Size: %d x %d\n", myMap->length, myMap->width);
    for (size_t i = 0; i < myMap->length; i++) {
        for (size_t k = 0; k < myMap->width; k++) {
            printf("%d ", myMap->fieldPrice[i][k]);
        }
        printf("\n");
    }
}

#endif // __PROGTEST__


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
    
    myMap->length++;
    myMap->width++;
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
            int n, **prices = myMap->fieldPrice;
            
            if (!i || !k) myMap->fieldPrice[i][k] = 0;
            else if (scanf("%d", &n) != 1 || n < 1) wrongInput();
            else myMap->fieldPrice[i][k] = n + prices[i - 1][k] + prices[i][k - 1] - prices[i - 1][k - 1];
        }
    }
}

#ifndef __PROGTEST__

//
//  Fuction unit testing
//
void testing (void) {
    
    
    
    
    
}

#endif // __PROGTEST__

//  run
int main(void) {
    map myMap;
    
#ifndef __PROGTEST__
    testing();
#endif // __PROGTEST__
    
    loadMapSize(&myMap);
    print(&myMap);

    return 0;
}
