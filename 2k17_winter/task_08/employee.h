#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//  Employee struct
typedef struct TEmployee {
    struct TEmployee * m_Next; // pointer to following employee int the list
    struct TEmployee * m_Bak; // pointer to backup employee
    char * m_Name; // Name of person
} TEMPLOYEE; // Employee data type

//  Add new employee to list
//  @param name Employee's name
//  @param next Next employee in the order
//  @return Pointer to the first employee
TEMPLOYEE * newEmployee (const char * name, TEMPLOYEE * next);

//  Create a copy from employee list
//  @param src Source employee list
//  @return Pointer to the first employee in the list
TEMPLOYEE * cloneList(TEMPLOYEE * src);

//  Delete list of employees
//  @param src Pointer to the first employee in the list
void freeList(TEMPLOYEE * src);
