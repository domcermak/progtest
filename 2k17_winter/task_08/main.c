#include "employee.h"

int main (void) {
    TEMPLOYEE * a, *b;
    char tmp[100];
    
    assert ( sizeof ( TEMPLOYEE ) == 3 * sizeof ( void * ) );
    a = NULL;
    a = newEmployee ( "Peter", a );
    a = newEmployee ( "John", a );
    a = newEmployee ( "Joe", a );
    a = newEmployee ( "Maria", a );
    a -> m_Bak = a -> m_Next;
    a -> m_Next -> m_Next -> m_Bak = a -> m_Next -> m_Next -> m_Next;
    a -> m_Next -> m_Next -> m_Next -> m_Bak = a -> m_Next;
    assert ( a
            && ! strcmp ( a -> m_Name, "Maria" )
            && a -> m_Bak == a -> m_Next );
    assert ( a -> m_Next
            && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
            && a -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
            && a -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    b = cloneList ( a );
    strncpy ( tmp, "Moe", sizeof ( tmp ) );
    a = newEmployee ( tmp, a );
    strncpy ( tmp, "Victoria", sizeof ( tmp ) );
    a = newEmployee ( tmp, a );
    strncpy ( tmp, "Peter", sizeof ( tmp ) );
    a = newEmployee ( tmp, a );
    b -> m_Next -> m_Next -> m_Next -> m_Bak = b -> m_Next -> m_Next;
    assert ( a
            && ! strcmp ( a -> m_Name, "Peter" )
            && a -> m_Bak == NULL );
    assert ( a -> m_Next
            && ! strcmp ( a -> m_Next -> m_Name, "Victoria" )
            && a -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "Moe" )
            && a -> m_Next -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Maria" )
            && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Joe" )
            && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "John" )
            && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    assert ( b
            && ! strcmp ( b -> m_Name, "Maria" )
            && b -> m_Bak == b -> m_Next );
    assert ( b -> m_Next
            && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
            && b -> m_Next -> m_Bak == NULL );
    assert ( b -> m_Next -> m_Next
            && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
            && b -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
            && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && b -> m_Next -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    freeList ( a );
    b -> m_Next -> m_Bak = b -> m_Next;
    a = cloneList ( b );
    assert ( a
            && ! strcmp ( a -> m_Name, "Maria" )
            && a -> m_Bak == a -> m_Next );
    assert ( a -> m_Next
            && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
            && a -> m_Next -> m_Bak == a -> m_Next );
    assert ( a -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
            && a -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    assert ( b
            && ! strcmp ( b -> m_Name, "Maria" )
            && b -> m_Bak == b -> m_Next );
    assert ( b -> m_Next
            && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
            && b -> m_Next -> m_Bak == b -> m_Next );
    assert ( b -> m_Next -> m_Next
            && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
            && b -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
            && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && b -> m_Next -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    freeList ( b );
    freeList ( a );
    
    printf("Test succeeded.\n");
    
    return 0;
}
