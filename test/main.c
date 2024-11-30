#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read.h"
#include "add_sub.h"

int main() {
    struct gf num1, num2, prime, sum, difference;

    makeInt(&num1, 9);  // Initialize with 9 blocks
    makeInt(&num2, 9);  // Initialize with 9 blocks
    makeInt(&prime, 9); // Initialize with 9 blocks

    // Binary data for each block
    u64 num1_data[9] = {
        0b10100110110100110000100100000,
        0b01100001010100010001111111000,
        0b11010010000001001100000001111,
        0b10010110110111100101010011001,
        0b11101110110101001010001001110,
        0b11000111000011011010110100110,
        0b01110101110110101111011101111,
        0b11101011001110110100110001110,
        0b00000101111111111001110101110
    };

    u64 num2_data[9] = {
        0b11000010111110100100101011001,
        0b01011100001100100111010110001,
        0b10010000000001110101010010100,
        0b10111010000001011100011011110,
        0b11011011000101101100000011111,
        0b00111000111100010101000101010,
        0b11001010100011100100100000111,
        0b11111111101111010110100110011,
        0b00000010000101111000001100111
    };

    u64 prime_data[9] = {
        0b11111111010011110111111110101,
        0b01010011001000001101011011011,
        0b11010100000010000111010011000,
        0b01011011111011001000110011101,
        0b10111001001100101100101000101,
        0b11110111000100100111100001001,
        0b10010001010000010000010101011,
        0b10101101011011110010100000011,
        0b00000111010010010111001000000
    };

    memcpy(num1.num, num1_data, 9 * sizeof(u64));
    memcpy(num2.num, num2_data, 9 * sizeof(u64));
    memcpy(prime.num, prime_data, 9 * sizeof(u64));

    printf("\nNumber 1:\n");
    for (int i = num1.len - 1; i >= 0; i--)
        printf("%08llx\n", num1.num[i]);

    printf("\nNumber 2:\n");
    for (int i = num2.len - 1; i >= 0; i--)
        printf("%08llx\n", num2.num[i]);

    // Perform addition
    sum = add(&num1, &num2, &prime);

    // Perform subtraction
    difference = sub(&num1, &num2, &prime);

    // Print results
    printf("\nAddition Result:\n");
    for (int i = sum.len - 1; i >= 0; i--)
        printf("%08llx ", sum.num[i]);
    printf("\n");

    printf("\nSubtraction Result:\n");
    for (int i = difference.len - 1; i >= 0; i--)
        printf("%08llx ", difference.num[i]);
    printf("\n");

    // Free allocated memory
    free(num1.num);
    free(num2.num);
    free(prime.num);
    free(sum.num);
    free(difference.num);

    return 0;
}
