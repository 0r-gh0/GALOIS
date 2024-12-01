#ifndef MAIN_H
#define MAIN_H

// Including the necessary standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils.h"
#include "add_sub.h"
#include "mul.h"
#include "exp.h"

// Typedef for unsigned 64-bit integer to ensure consistent width
typedef unsigned long long u64;

// Structure to represent a multi-precision integer in Galois Field
typedef struct 
{
    u64* num;   // Pointer to array of 29-bit chunks
    int size;   // Number of chunks in the array
} gf;

#endif // MAIN_H