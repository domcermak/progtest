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
//  @param lenght Lenght size of array
//  @param width Width size of array
//  @param fieldPrice Array of prices of fields
//
typedef struct {
    int length;
    int width;
    int ** fieldPrice;
} map;

//
//  Struct of request
//  @param keyname Keyname of request
//  @param investment Value of investment
//  @param eof Is true when EOF occur
//
typedef struct {
    char keyname [10];
    int investment;
    bool eof;
} request;

//
//  List of parameters, that need to be printed when 'list' request is called
//  @param w Width
//  @param h Height
//  @param x X value of left top corner
//  @param y Y value of left top corner
//  @param price Summary price of fields
//
typedef struct {
    int w;
    int h;
    int x;
    int y;
    int price;
} list;

//
//  Wrapped result values of count request
//  @param count Count of fields to be bought
//  @param maxSize Maximal size of fields to be bought
//
typedef struct {
    int count;
    int maxSize;
    list * listed;
} countRequest;

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
    printf("Velikost mapy:\n");
    if (scanf("%d %d", &(myMap->width), &(myMap->length)) != 2
        || myMap->length < 1
        || myMap->length > 2000
        || myMap->width < 1
        || myMap->width > 2000)
        wrongInput();
    
    myMap->length++;
    myMap->width++;
}

//
//  Load map of prices from stdin of size length x width in myMap
//  @param myMap Map struct pointer with filled lenght and width
//
static void loadMapValues (map * myMap) {
    printf("Cenova mapa:\n");
    myMap->fieldPrice = (int**) malloc(myMap->length * sizeof(int*));
    for (int i = 0; i < myMap->length; i++) {
        myMap->fieldPrice[i] = (int*) malloc(myMap->width * sizeof(int));
        for (int k = 0; k < myMap->width; k++) {
            int n, **prices = myMap->fieldPrice;
            
            if (!i || !k) myMap->fieldPrice[i][k] = 0;
            else if (scanf("%d", &n) != 1 || n < 1) wrongInput();
            else myMap->fieldPrice[i][k] = n + prices[i - 1][k] + prices[i][k - 1] - prices[i - 1][k - 1];
        }
    }
}

//
//  Load command and investment
//  @return Command struct with information about command and investment
//
static request loadRequest (void) {
    request result = {"0", 0, false};
    char req [10];
    int n, ref;
    
    ref = scanf("%9s %d", req, &n);
    if (n < 1) {
        wrongInput();
    }
    if (ref == EOF) {
        result.eof = true;
        return result;
    }
    else if (ref != 2) {
        wrongInput();
    }
    
    result.investment = n;
    strcpy(result.keyname, req);
    result.eof = false;
    
    return result;
}

//
//  Print content for 'count' reguest
//  @param myMap Constant pointer to instance of map with all filled values
//  @param investment Valid integer value of investment
//
static countRequest countRequestCalc(const map * myMap, const int investment) {
    countRequest result = {0, 0, NULL};
    list * myList = (list*) malloc(sizeof(list));
    int listSize = 1;
    int cnt = 0;
    int maxReachedSize = 0;
    
    for (int i = 1; i < myMap->length; i++) {
        for (int k = 1; k < myMap->width; k++) {
//            already summaried?
            if (myMap->fieldPrice[i][k] <= investment) {
                int mySize = i * k;
                
                if (maxReachedSize == mySize) {
                    cnt++;
                    if (cnt > listSize) {
                        listSize *= 2;
                        myList = (list*) realloc(myList, listSize * sizeof(list));
                    }
                    myList[cnt - 1].w = i;
                    myList[cnt - 1].h = k;
                    myList[cnt - 1].x = 0;
                    myList[cnt - 1].y = 0;
                    myList[cnt - 1].price = myMap->fieldPrice[i][k];
                }
                else if (mySize > maxReachedSize) {
                    cnt = 1;
                    maxReachedSize = mySize;
                    free(myList);
                    myList = (list*) malloc(sizeof(list));
                    listSize = 1;
                    myList[cnt - 1].w = i;
                    myList[cnt - 1].h = k;
                    myList[cnt - 1].x = 0;
                    myList[cnt - 1].y = 0;
                    myList[cnt - 1].price = myMap->fieldPrice[i][k];
                }
                
                continue;
            }
            
//            find subrectangles
            for (int m = i; m > 0; m--) {
                for (int p = k; p > 0; p--) {
                    int value_rightTop, value_leftBottom, value_item, value_leftTop;
                    int mySize, summary;
                    
                    value_item = myMap->fieldPrice[i][k];
                    value_leftTop = myMap->fieldPrice[m - 1][p - 1];
                    value_rightTop = myMap->fieldPrice[i][p - 1];
                    value_leftBottom = myMap->fieldPrice[m - 1][k];
                    summary = value_item + value_leftTop - value_rightTop - value_leftBottom;
                    mySize = (i - m + 1) * (k - p + 1);
                    
                    if (summary <= investment) {
                        if (maxReachedSize == mySize) {
                            cnt++;
                            if (cnt > listSize) {
                                listSize *= 2;
                                myList = (list*) realloc(myList, listSize * sizeof(list));
                            }
                            myList[cnt - 1].w = i - m + 1;
                            myList[cnt - 1].h = k - p + 1;
                            myList[cnt - 1].x = p - 1;
                            myList[cnt - 1].y = m - 1;
                            myList[cnt - 1].price = summary;
                        }
                        else if (mySize > maxReachedSize) {
                            cnt = 1;
                            maxReachedSize = mySize;
                            free(myList);
                            myList = (list*) malloc(sizeof(list));
                            listSize = 1;
                            myList[cnt - 1].w = i - m + 1;
                            myList[cnt - 1].h = k - p + 1;
                            myList[cnt - 1].x = p - 1;
                            myList[cnt - 1].y = m - 1;
                            myList[cnt - 1].price = summary;
                        }
                    }
                }
            }
        }
    }
    result.count = cnt;
    result.maxSize = maxReachedSize;
    result.listed = myList;
    
    return result;
}

int compare (const void * a, const void * b) {
    return ( (*(list*)a).x - (*(list*)b).x );
}

//
//  Print content for 'list' request
//  @param myMap Constant pointer to instance of map with all filled values
//  @param investment Valid integer value of investment
//
static void listRequestCalc (const map * myMap, const int investment) {
    countRequest values = countRequestCalc(myMap, investment);
    
    if (values.maxSize) {
        printf("Max. rozloha: %d (x %d)\n", values.maxSize, values.count);
        
        qsort(values.listed, values.count, sizeof(list), compare);
        for (int i = 0; i < values.count; i++) {
            printf("%d: %d x %d @ (%d,%d)\n", values.listed[i].price, values.listed[i].h, values.listed[i].w, values.listed[i].x, values.listed[i].y);
        }
    }
    else
        printf("Nenalezeno.\n");
    free(values.listed);
}

//
//  Calculate behavior of map
//  @param myMap Constant pointer to instance of map with all its values filled
//
static void behaviorCalc (const map * myMap) {
    request currRequest;
    
    printf("Dotazy:\n");
    while (!(currRequest = loadRequest()).eof) {
        if (!strcmp(currRequest.keyname, "count")) {
            countRequest v = countRequestCalc(myMap, currRequest.investment);
            
            if (v.maxSize)
                printf("Max. rozloha: %d (x %d)\n", v.maxSize, v.count);
            else
                printf("Nenalezeno.\n");
            free(v.listed);
        }
        else if (!strcmp(currRequest.keyname, "list")) {
            listRequestCalc(myMap, currRequest.investment);
        }
        else {
            wrongInput();
        }
    }
}

//  run
int main(void) {
    map myMap = {0, 0, NULL};
    
    loadMapSize(&myMap);
    loadMapValues(&myMap);
    behaviorCalc(&myMap);

    return 0;
}
