#ifndef READ_H
#define READ_H

#include <stdlib.h>
#include <string.h>

typedef unsigned long long int u64;

// Structure to store a number along with its length in bytes
struct gf {
    int len;
    u64 *num;
};

// Function to create a dynamically allocated array to store a number
void makeInt(struct gf *in, int SIZE) {
    in->num = (u64 *)malloc(SIZE * sizeof(u64));
    memset(in->num, 0, SIZE * sizeof(u64));
    in->len = SIZE;
}

#endif // READ_H
