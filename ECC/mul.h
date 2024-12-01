#ifndef MUL_H
#define MUL_H

#include "main.h"
#include "utils.h"
#include "add_sub.h"

// Multiplication of two multi-precision integers
void Mul(gf a, gf b, gf* res);

// Barrett Reduction implementation
void BReduction(gf x, gf* res, gf p, gf mu);

// Multiplication in Zp (modulo prime)
void MultInZp(gf a, gf b, gf* res, gf p, gf mu);

#endif // MUL_H
