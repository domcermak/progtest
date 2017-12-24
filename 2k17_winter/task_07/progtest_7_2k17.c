#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

//  Print error message and exit program with failure value
static void wrongInput (void) {
    printf("Nespravny vstup.\n");
    exit(EXIT_FAILURE);
}

//  Verify modul value
//  @param modul Value of modul
//  @return Statement of validity
static bool isValidModul (const int modul) {
    return modul >= 1 && modul <= 10;
}

//  Validate request value
//  @param request String of request to be validated
//  @param print Boolean value if parcel list shall be printed
//  @return Statement of validity
static bool isValidRequest (const char request[], bool * print) {
    if (!strcmp(request, "list")) *print = true;
    else if (!strcmp(request, "count")) *print = false;
    else return false;
    
    return true;
}

//  Load modul value from stdin
//  @return Verified modul value
static int loadModul (void) {
    int modul;
    
    printf("Modul:\n");
    if (scanf("%d", &modul) != 1 || !isValidModul(modul))
        wrongInput();
    
    return modul;
}

//  Load length of list of parcels
//  @param print
//  @return Verified value of list length
static inline int loadParcelListLength (bool * print) {
    int len, ref;
    char request [6];
    
    printf("Vypocty:\n");
    ref = scanf("%6s %d", request, &len);
    
    if (!isValidRequest(request, print)) wrongInput();
    if (ref == EOF) return -1;
    if ( ref != 1 || len < 1) wrongInput();
    
    return len;
}

//  Evaluate list of possible parcel combination
//  @param modul Value of modul
//  @param list Length of parcel list
//  @param printList Boolean value if parcel list should be printed
static inline void evaluateParcelList (const register int modul, const register int list, const bool printList) {
//        @TODO
}

//run
int main (void) {
    register int modul, list;
    bool print;
    
    modul = loadModul();
    while ((list = loadParcelListLength(&print)) != EOF)
        evaluateParcelList(modul, list, print);
    
    return 0;
}
