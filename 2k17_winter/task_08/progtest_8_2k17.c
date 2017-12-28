#ifndef __PROGTEST__
#include "employee.h"
#endif // __PROGTEST__

//  Find eployee's name in linked list
//  @param root Pointer to the first employee
//  @param name Name of employee
//  @return Adress of found employee
static inline TEMPLOYEE * findPerson (TEMPLOYEE * root, TEMPLOYEE * src, TEMPLOYEE * backup) {
    TEMPLOYEE * tmp = src;
    size_t cnt = 0;
    
    while (1) {
        if (tmp == backup) break;
        cnt++;
        tmp = tmp->m_Next;
    }
    
    tmp = root;
    for (size_t i = 0; i < cnt; i++)
        tmp = tmp->m_Next;
    
    return tmp;
}

//  Add new employee to list
//  @param name Employee's name
//  @param next Next employee in the order
//  @return Pointer to the first employee
TEMPLOYEE * newEmployee (const char * name, TEMPLOYEE * next) {
    TEMPLOYEE * root = (TEMPLOYEE*)malloc(sizeof(TEMPLOYEE));
    
    root->m_Name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(root->m_Name, name);
    root->m_Bak = NULL;
    root->m_Next = next;
    
    return root;
}

//  Create a copy from employee list
//  @param src Source employee list
//  @return Pointer to the first employee in the list
TEMPLOYEE * cloneList(TEMPLOYEE * src) {
    TEMPLOYEE * root = NULL, *ptrR, *ptrS;
    
    if (!src) return NULL;
    root = (TEMPLOYEE*)malloc(sizeof(TEMPLOYEE));
    ptrR = root;
    ptrS = src;
    while (ptrS) {
        ptrR->m_Name = (char*)malloc((strlen((ptrS->m_Name)) + 1) * sizeof(char));
        strcpy(ptrR->m_Name, ptrS->m_Name);
        ptrR->m_Bak = NULL;
        
        ptrS = ptrS->m_Next;
        if (!ptrS) break;
        else {
            ptrR->m_Next = (TEMPLOYEE*)malloc(sizeof(TEMPLOYEE));
            ptrR = ptrR->m_Next;
        }
    }
    
    ptrR = root;
    ptrS = src;
    while (ptrS) {
        if (ptrS->m_Bak) ptrR->m_Bak = findPerson(root, src, ptrS->m_Bak);
        ptrR = ptrR->m_Next;
        ptrS = ptrS->m_Next;
    }
    
    return root;
}

//  Delete list of employees
//  @param src Pointer to the first employee in the list
void freeList(TEMPLOYEE * src) {
    while (src) {
        TEMPLOYEE * tmp = src->m_Next;
        
        free(src);
        src = tmp;
    }
}
