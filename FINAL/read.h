// read.h
#ifndef READ_H  // Start include guard
#define READ_H  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int u32;
typedef unsigned long long int u64;

// Structure to store a number along with its length in byte
struct gf{
	int len;
	u64 *num;
};

// Function to create a dyamically allocated array to store a number
void makeInt(struct gf* in, int SIZE){
    in->num = (u64*)malloc(SIZE * sizeof(u64));
	memset(in->num, 0, SIZE * sizeof(u64));
	in->len = SIZE;
}

#endif  // End include guard