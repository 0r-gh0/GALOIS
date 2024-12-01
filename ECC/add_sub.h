#ifndef ADD_SUB_H
#define ADD_SUB_H

#include "main.h"
#include "utils.h"

// Comparison function to check if one gf is less than another
int compare(gf a, gf b);

// Addition of two multi-precision integers
void Add(gf a, gf b, gf* res);

// Subtraction of two multi-precision integers
void Sub(gf a, gf b, gf* res);

// Addition in Zp (modulo prime)
void AddInZp(gf a, gf b, gf* res);

#endif // ADD_SUB_H