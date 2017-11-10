//
//@file main.c
//@project Progtest
//@author Dominik Čermák
//@date 4.11.2017
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#define maxsize 10000000

typedef long long int lli;
typedef struct { lli width; lli length; } bathroom;
typedef struct { lli mySize; lli jointSize; } tile;

#ifndef __PROGTEST__
void printList (const lli * list, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%lli, ", *(list + i));
    }
    printf("\n");
}
#endif // __PROGTEST__

void failText (void) {
    printf("Nespravny vstup.\n");
    exit(EXIT_FAILURE);
}

void notExistingSolutionText (void) {
    printf("Reseni neexistuje.\n");
    exit(EXIT_FAILURE);
}

void printResultRoom (const bathroom * room) {
    printf("Velikost: %.1f x %.1f\n", (double)room->width / 10, (double)room->length / 10);
}

bool isUnsupportedRoomOrTileSize (const double value) {
    return value <= 0
        || value > maxsize
        || (int)(value * 10) != value * 10;
}

bool isUnsupportedJointSize (const double value) {
    return value < 0
        || value > maxsize
        || (double)(int)(value * 10) != value * 10;
}

lli greatestCommonDivisor (const lli a, const lli b) {
    return !b ? a : greatestCommonDivisor(b, a % b);
}

bathroom * loadBathroomParams (void) {
    bathroom * result;
    double tmpLength, tmpWidth;
    
    printf("Minimalni velikost:\n");
    if (fscanf(stdin, "%lf %lf", &tmpWidth, &tmpLength) != 2
        || isUnsupportedRoomOrTileSize(tmpWidth)
        || isUnsupportedRoomOrTileSize(tmpLength)) {
        failText();
    }
    result = (bathroom*)malloc(sizeof(bathroom));
    result->length = tmpLength * 10;
    result->width = tmpWidth * 10;
    
    return result;
}

int tileCompareHighToLow (const void * a, const void * b) {
    return (int)(((tile*)b)->mySize - ((tile*)a)->mySize);
}

tile * loadTiles (size_t * tileCount) {
    size_t mySize = 1;
    size_t i = 0;
    tile * result = (tile*) malloc(mySize * sizeof(tile));
    int ref;
    
    *tileCount = 0;
    
    printf("Dlazdicky:\n");
    while (true) {
        double tmpSelfSize, tmpJointSize;
        
        ref = scanf("%lf %lf", &tmpSelfSize, &tmpJointSize);
        if ((ref != 2 && ref != EOF) || (ref == EOF && !*tileCount)
            || isUnsupportedJointSize(tmpJointSize) || isUnsupportedRoomOrTileSize(tmpSelfSize)) {
            free(result);
            failText();
        }
        else if (ref == EOF) break;
        
        (result + i)->mySize = tmpSelfSize * 10;
        (result + i)->jointSize = tmpJointSize * 10;
        
        if (i >= mySize - 1) {
            mySize <<= 1;
            result = (tile*) realloc(result, mySize * sizeof(tile));
        }
        
        (*tileCount)++;
        i++;
    }
    qsort(result, *tileCount, sizeof(tile), tileCompareHighToLow);
    
    return result;
}

bool canFindUsableIndex (const double tileSize, const double jointSize, const double expectedResult) {
    double index = (expectedResult - jointSize) / (tileSize + jointSize);
    
    return index == (double)(lli)index;
}

bathroom * finalBathroomSizeCalc (const bathroom * minimalRoomSize, const tile * myTiles , const size_t tileCount) {
    bathroom * resultRoom = (bathroom*) malloc(sizeof(bathroom));
    lli startingWidthIndex = (minimalRoomSize->width - myTiles->jointSize) / (myTiles->jointSize + myTiles->mySize);
    lli startingLengthIndex = (minimalRoomSize->length - myTiles->jointSize) / (myTiles->jointSize + myTiles->mySize);
    lli finalWidth, finalLength, increaseValue, compareIndex;
    
    if (((double)minimalRoomSize->width - (double)myTiles->jointSize) / ((double)myTiles->jointSize + (double)myTiles->mySize) != (double)startingWidthIndex)
        startingWidthIndex++;
    if (((double)minimalRoomSize->length - (double)myTiles->jointSize) / ((double)myTiles->jointSize + (double)myTiles->mySize) != (double)startingWidthIndex)
        startingLengthIndex++;
    
    finalWidth = myTiles->jointSize + startingWidthIndex * (myTiles->mySize + myTiles->jointSize);
    finalLength = myTiles->jointSize + startingLengthIndex * (myTiles->mySize + myTiles->jointSize);
    
    for (size_t i = 0; i < tileCount; i++)
        for (size_t k = i + 1; k < tileCount; k++) {
            lli value1 = (myTiles + i)->jointSize + (myTiles + i)->mySize;
            lli value2 = (myTiles + k)->jointSize + (myTiles + k)->mySize;
            
            if (((myTiles + i)->jointSize - (myTiles + k)->jointSize) % greatestCommonDivisor(value1, value2))
                notExistingSolutionText();
        }
    
//    width
    increaseValue = 1;
    compareIndex = 1;
    for (lli i = startingWidthIndex; tileCount > 1; i += increaseValue) {
        lli value1 = myTiles->jointSize + i * (myTiles->mySize + myTiles->jointSize);
        bool ref = canFindUsableIndex((myTiles + compareIndex)->mySize, (myTiles + compareIndex)->jointSize, value1);
        
        if (ref) {
            increaseValue *= ((myTiles + compareIndex)->jointSize + (myTiles + compareIndex)->mySize)
                            / (greatestCommonDivisor(increaseValue * (myTiles->jointSize + myTiles->mySize), (myTiles + compareIndex)->jointSize + (myTiles + compareIndex)->mySize));
            compareIndex++;
            
            if (compareIndex >= (lli)tileCount) {
                finalWidth = value1;
                break;
            }
            
            i -= increaseValue;
        }
    }
    
//    length
    increaseValue = 1;
    compareIndex = 1;
    for (lli i = startingLengthIndex; tileCount > 1; i += increaseValue) {
        lli value1 = myTiles->jointSize + i * (myTiles->mySize + myTiles->jointSize);
        bool ref = canFindUsableIndex((myTiles + compareIndex)->mySize, (myTiles + compareIndex)->jointSize, value1);
        
        if (ref) {
            increaseValue *= ((myTiles + compareIndex)->jointSize + (myTiles + compareIndex)->mySize)
                            / (greatestCommonDivisor(increaseValue * (myTiles->jointSize + myTiles->mySize), (myTiles + compareIndex)->jointSize + (myTiles + compareIndex)->mySize));
            compareIndex++;
            
            if (compareIndex >= (lli)tileCount) {
                finalLength = value1;
                break;
            }
            
            i -= increaseValue;
        }
    }
    resultRoom->length = finalLength;
    resultRoom->width = finalWidth;
    
    return resultRoom;
}

#ifndef __PROGTEST__
void tests (void) {
    assert(!isUnsupportedJointSize(0));
    assert(!isUnsupportedJointSize(3556.2));
    assert(isUnsupportedJointSize(3556.22));
    assert(isUnsupportedJointSize(-1));
    assert(isUnsupportedJointSize(maxsize + 0.1));
    
    assert(!isUnsupportedRoomOrTileSize(3556.2));
    assert(isUnsupportedRoomOrTileSize(3556.22));
    assert(isUnsupportedRoomOrTileSize(0));
    assert(isUnsupportedRoomOrTileSize(-1));
    assert(isUnsupportedRoomOrTileSize(maxsize + 0.1));
    
    assert(canFindUsableIndex(2, 0, 8));
    assert(canFindUsableIndex(3.1, 0.9, 4.9));
    assert(!canFindUsableIndex(2, 0, 9));
    assert(!canFindUsableIndex(3.1, 0.9, 5));
}
#endif // __PROGTEST__

//run
int main (void) {
    size_t tileCount;
    bathroom * myMinSizeRoom, *finalRoom;
    tile * myTiles;
    
#ifndef __PROGTEST__
    tests();
#endif // __PROGTEST__
    
    myMinSizeRoom = loadBathroomParams();
    myTiles = loadTiles(&tileCount);
    finalRoom = finalBathroomSizeCalc(myMinSizeRoom, myTiles, tileCount);
    printResultRoom(finalRoom);
    
    free(finalRoom);
    free(myMinSizeRoom);
    free(myTiles);
    
    return 0;
}
