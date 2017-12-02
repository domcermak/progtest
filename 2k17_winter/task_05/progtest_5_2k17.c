#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

//
//  Custom alias for positive 64-bit integer
//
typedef unsigned long long int ulli;

//
//  Struct of stripe
//  @param array    Array of numbers containing values of stripes count
//  @param length   Length of array of stripes
//
typedef struct {
	ulli * array;
	size_t length;
} stripe;

//
//  Struct of way values
//  @param from Index of starting stripe in array
//  @param to   Index of ending stripe after last used stripe in order
//  @param eof  Boolean value if EOF occured while reading from stdin
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
	ulli * result = (ulli*) malloc (sizeof(ulli));
	size_t mySize = 1;

	
	printf("Pocty pruhu:\n");
	for (size_t i = 0; true; i++) {
		int ref1 = 0, ref2 = 0;
		int value = 0;
		char str[2], c;

		ref1 = scanf("%1s", str);
		c = str[0];
        
        if (i && ref1 == 1 && c == '}') {
            myStripe.array = result;
            myStripe.length = i;
            
            return myStripe;
        }
        
        ref2 = scanf("%d", &value);
		if (!i && (c != '{' || ref2 != 1 || value < 1)) wrongInput();
		if (i && (ref1 != 1 || ref2 != 1 || c != ',' || value < 1))
            wrongInput();
		
		if (mySize <= i) {
			mySize *= 2;
			result = (ulli*) realloc (result, mySize * sizeof(ulli));
		}

		*(result + i) = (ulli)value;
	}
}

//
//  Load way values
//  @param s    Stripe list with filled values
//  @return     Way with filled falues
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
    
    result.from = from;
    result.to = to;
    
	return result;
}

//
//  Greatest common divisor
//  @param a Number a
//  @param b Number b
//  @return Gcd of numbers a and b
//
static ulli gcd (const ulli a, const ulli b) {
    return !b ? a : gcd(b, a % b);
}

//
//  Lowest common multiple
//  @param a Number a
//  @param b Number b
//  @return Lcm of numbers a and b
//
static ulli lcm (const ulli a, const ulli b) {
    return a / gcd(a, b) * b;
}

//
//
//  @param myStripe
//  @param myWay
//  @return Lowest count of cars that are needed to success
//
static ulli carCountCalc(const stripe * myStripe, const way * myWay) {
    ulli result = myStripe->array[myWay->from];
    
    for (int i = myWay->from + 1; i < myWay->to; i++)
        result = lcm(result, myStripe->array[i]);
    
    return result;
}

#ifndef __PROGTEST__
//
//  Function testing
//
static void testing (void) {
    assert(gcd(12, 15) == 3);
    assert(gcd(45, 30) == 15);
    assert(lcm(6, 5) == 30);
    assert(lcm(6, 9) == 18);
}
#endif // __PROGTEST__

//  run
int main (void) {
    stripe stripeList = loadStripes();
	
#ifndef __PROGTEST__
    testing();
#endif // __PROGTEST__
    
	printf("Trasy:\n");
	while (true) {
		way myWay = loadWay(&stripeList);
		ulli cnt;

		if (myWay.eof) break;
		cnt = carCountCalc(&stripeList, &myWay);
		printf("Vozidel: %llu\n", cnt);
	}
    free(stripeList.array);
    
    return 0;
}
