// add_sub.h
#ifndef ADD_SUB_H
#define ADD_SUB_H
#include "read.h"

// Function to pad the number with zeros up to targetLen
struct gf pad(struct gf* num0, int target) {
    // Create a new structure to store the padded result
    struct gf padded;

    // Allocate memory for the new padded array
    padded.num = (u64*)malloc(target * sizeof(u64));
    memset(padded.num, 0, target * sizeof(u64));  // Initialize with zeros
    padded.len = target;

    // Copy the original data into the padded structure
    for (int i = 0; i < num0->len; i++) {
        padded.num[i] = num0->num[i];   }
    return padded;
}

// Function to make two arrays of equal length
void eqLen(struct gf* num1, struct gf* num2) {
    if (num1->len < num2->len)
        *num1 = pad(num1, num2->len);
    else if (num1->len > num2->len)
        *num2 = pad(num2, num1->len);
}

// Function to add two integers
struct gf add(struct gf* num1, struct gf* num2) {
    u64 carry = 0;
    u64 sum;
    eqLen(num1, num2);

    // Create a new structure to store the result
    struct gf result;

    // Allocate memory for the result array
    result.num = (u64*)malloc(num1->len * sizeof(u64));
    memset(result.num, 0, num1->len * sizeof(u64));  // Initialize with zeros
    result.len = num1->len;

    for (int i = 0; i < result.len; i++) {
		if (i < result.len - 1) {
			sum = (num1->num[i] & 0x1FFFFFFF) + (num2->num[i] & 0x1FFFFFFF) + carry;
	    	result.num[i] = sum & 0x1FFFFFFF;  // Store 29-bit result
	        carry = sum >> 29; } // Carry should only consider 29 bits
		else {
			// Handle the last block with the remaining bits (assuming 24 bits)
            sum = (num1->num[i] & 0xFFFFFF) + (num2->num[i] & 0xFFFFFF) + carry;
            result.num[i] = sum & 0xFFFFFF;  // Store 24-bit result
	    carry = sum >> 24; } // Update carry for 24 bits
	}
    return result;  // Return the result structure
}

// Function to subtract two intgers, using 2's complement
struct gf sub(struct gf* num1, struct gf* num2) {
    eqLen(num1, num2);

    // Create a new structure to store the result
    struct gf one, comp;
	makeInt(&one, num1->len);
	makeInt(&comp, num2->len);

    one.num[0] = 1;

    for (int i = 0; i < num2->len; i++)	{
		if (i < num2->len - 1) {
            comp.num[i] = ~num2->num[i] & 0x1FFFFFFF;	}// Mask to 29 bits for the first 8 blocks
        else {
            comp.num[i] = ~num2->num[i] & 0xFFFFFF;		}// Mask to 24 bits for the last block
	}

    // Add 2's complement (1's complement + 1)
    struct gf two_Comp = add(&comp, &one); // 2's complement of num2

    // Perform addition with num1
    struct gf sub_Res = add(num1, &two_Comp);

     // Free the temporary structures
    free(comp.num);
    free(one.num);
    free(two_Comp.num);

	// Adjust the results for the bit constraints (masking)
    for (int i = 0; i < sub_Res.len; i++) {
        if (i < sub_Res.len - 1) {
            sub_Res.num[i] &= 0x1FFFFFFF;	}  // Mask to 29 bits for the first 8 blocks
        else {
            sub_Res.num[i] &= 0xFFFFFF;		}  // Mask to 24 bits for the last block
    }

    // Remove leading zeros if needed
    while (sub_Res.len > 1 && sub_Res.num[sub_Res.len - 1] == 0) {
        sub_Res.len--;
    }

    return sub_Res;  // Return the result
}

#endif  // End include guard
