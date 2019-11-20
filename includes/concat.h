//This header file and all functions herein written by Ryan Villena.

#ifndef CONCAT_H
#define CONCAT_H

#include <string.h>

//Dynamically allocated - MUST be freed by client somewhere!!
char* concat_and_alloc(char* c1, char* c2) {
    
    size_t size_1 = strlen(c1);
    size_t size_2 = strlen(c2);
    
    char* combined = malloc(size_1 + size_2 + 1); //+1 for null character
    strcpy(combined, c1);
    strcat(combined, c2);   //also appends null character
    
    return combined;
    
}

#endif // CONCAT_H