// read.h
#ifndef READ_H  // Start include guard
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h>

typedef unsigned char u8;
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

// Function to push random 29-bit data into the struct
/*
void fill29(struct gf* in) {
    for (int i = 0; i < in->len; i++) {
        u8 buffer[4]; // 29 bits can fit into 4 bytes
        if (RAND_bytes(buffer, 4) != 1) {
            fprintf(stderr, "RAND_bytes failed\n");
            exit(EXIT_FAILURE);
        }

        // Mask the top 3 bits to ensure only 29 bits are set
        u64 randomValue = ((u64)(buffer[0] & 0x1F) << 24) | ((u64)buffer[1] << 16) | ((u64)buffer[2] << 8) | (u64)buffer[3];

        randomValue &= 0x1FFFFFFF; // Ensure only 29 bits are kept
        in->num[i] = randomValue;
    }
}
*/

// Function to push random 29-bit data into the first 8 blocks and 24-bit data into the last block
void fill29(struct gf* in) {
    for (int i = 0; i < in->len; i++) {
        u8 buffer[4]; // Buffer to hold random bytes
        if (RAND_bytes(buffer, 4) != 1) {
            fprintf(stderr, "RAND_bytes failed\n");
            exit(EXIT_FAILURE);
        }

        if (i < 8) { // For the first 8 blocks
            u64 randomValue = ((u64)(buffer[0] & 0x1F) << 24) |
                              ((u64)buffer[1] << 16) |
                              ((u64)buffer[2] << 8) |
                              (u64)buffer[3];
            randomValue &= 0x1FFFFFFF; // Mask to ensure only 29 bits
            in->num[i] = randomValue;
        } else { // For the last block (24 bits)
            u64 randomValue = ((u64)buffer[0] << 16) |
                              ((u64)buffer[1] << 8) |
                              (u64)buffer[2];
            randomValue &= 0xFFFFFF; // Mask to ensure only 24 bits
            in->num[i] = randomValue;
        }
    }
}

#endif  // End include guard
