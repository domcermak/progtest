//
//  Created as a part of http://www.github.com/domcermak/progtest
//  Copyright © 2017 Dominik Čermák. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

//
//  Struct of stripe
//  @param array
//  @param length
//
typedef struct {
	int * array;
	size_t length;
} stripe;

//
//  Struct of way values
//  @param from
//  @param to
//  @param eof
//
typedef struct {
	int from;
	int to;
	bool eof;
} way;

//
//  Print error message and exit program
//
static void wrongInput (void) {
	printf("Nespravny vstup.\n");
	exit(EXIT_FAILURE);	
}

//
//  Load stripes values in preset form
//  @return List of stripes
//
static stripe loadStripes (void) {
	stripe myStripe = {NULL, 0};
	int * result = (int*) malloc (sizeof(int));
	size_t mySize = 1;

	
	printf("Pocty pruhu:\n");
	for (size_t i = 0; true; i++) {
		int ref = 0;
		int value = 0;
		char str[2], c;

		ref = scanf("%1s %d", str, &value);
		c = str[0];

		if (!i && c != '{') wrongInput();
		if (i) {
			if (c == '}') {
				myStripe.array = result;
				myStripe.length = i;

				return myStripe;
			}
			if (ref != 2 || c != ',' || value < 1)
                wrongInput();
		}
		
		if (mySize <= i) {
			mySize *= 2;
			result = (int*) realloc (result, mySize * sizeof(int));
		}

		*(result + i) = value;
	}
}

//
//  Load way values
//  @param s Stripe list with filled values
//
static way loadWay (const stripe * s) {
	way result = {0, 0, false};
	int ref, from, to;
	
	ref = scanf("%d %d", &from, &to);
	if (ref == EOF) {
		result.eof = true;
		return result;
	}
	if (ref != 2 || from < 0 || from >= to || s->length < (size_t)to)
		wrongInput();

	return result;
}

//
//  Greatest common divisor
//  @param a Number a
//  @param b Number b
//
static int gcd (const int a, const int b) {
    return !b ? a : gcd(b, a % b);
}

//
//  Lowest common multiple
//  @param a Number a
//  @param b Number b
//
static int lcm (const int a, const int b) {
    return (a * b) / gcd(a, b);
}

static int carCountCalc(const stripe * myStripe, const way * myWay) {
    int result = myStripe->array[myWay->from];
    
    for (int i = myWay->from + 1; i < myWay->to + 1; i++)
        result = lcm(result, myStripe->array[i]);
    
    return result;
}

//
//  Deallocate allocated list
//  @param myList Stripe list with allocated array
//
static void removeStripes (stripe * myList) {
    free(myList->array);
}

//  run
int main (void) {
    stripe stripeList = loadStripes();
	
	printf("Trasy:\n");
	while (true) {
		way myWay = loadWay(&stripeList);
		int cnt;

		if (myWay.eof) break;
		cnt = carCountCalc(&stripeList, &myWay);
		printf("Vozidel: %d\n", cnt);
	}
    removeStripes(&stripeList);
    
    return 0;
}
