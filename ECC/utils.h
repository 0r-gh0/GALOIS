#ifndef UTILS_H
#define UTILS_H

#include "main.h"

// Initialize a multi-precision integer with given size
gf initlz(int size);

// Free memory allocated for a multi-precision integer
void free_gf(gf* a);

// Debugging function to print multi-precision integer
void print_gf(gf* a);

#endif // UTILS_H
