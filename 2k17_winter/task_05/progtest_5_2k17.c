//
//  Created as a part of http://www.github.com/domcermak/progtest
//  Copyright © 2017 Dominik Čermák. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
	int * array;
	size_t lenght;
} stripe;

typedef struct {
	int from;
	int to;
} way;

static void wrongInput (void) {
	printf("Nespravny vstup.\n");
	exit(EXIT_FAILURE);	
}

static const stripe loadStripes (void) {
	stripe myStripe = {NULL, 0};
	int ref;
	int * result = (int*) malloc (sizeof(int));
	size_t mySize = 1;

	
	printf("Pocty pruhu:\n");
	for (size_t i = 0; true; i++) {
		int ref = 0;
		int value = 0;
		char str[2], c;

		ref = scanf("%1s %d", c, &value);
		c = str[0];

		if (!i && c != "{") wrongInput();
		if (i) {
			if (c == '}') {
				myStripe.array = result;
				myStripe.length = i;

				return myStripe;
			}
			if (ref != 2 || c != ',' || value < 1)
				wrongInput()
		}
		
		if (mySize <= i) {
			mySize *= 2;
			result = (int*) realloc (mySize * sizeof(int));	
		}

		*(result + i) = value;
	}
}

static way loadWay (const stripe * s) {
	way result = {0, 0};
	int ref, from, to;
	
	ref = scanf("%d %d", &from, &to);
	if (ref != 2 || from < 0 || from >= to || s.length < to)
		wrongInput();

	return result;
}

//  run
int main (void) {
    myStripe stripeList = loadStripes();

	free(stripesList);
    
    return 0;
}
