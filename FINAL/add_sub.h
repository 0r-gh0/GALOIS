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
    for (int i = num0->len - 1 ; i >= 0 ; i--) {
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
    u32 carry = 0;
    u64 sum;
    eqLen(num1, num2);

    // Create a new structure to store the result
    struct gf result;

    // Allocate memory for the result array
    result.num = (u64*)malloc(num1->len * sizeof(u64));
    memset(result.num, 0, num1->len * sizeof(u64));  // Initialize with zeros
    result.len = num1->len;

    for (int i = 0; i < result.len; i++) {
        sum = (u64)num1->num[i] + (u64)num2->num[i] + carry;
        result.num[i] = (u32)(sum);  // Store the full sum in result
        carry = sum >> 32;  // Update carry
    }
    return result;  // Return the result structure
}

// Function to subtract two intgers, using 2's complement
struct gf sub(struct gf* num1, struct gf* num2) {
    eqLen(num1, num2);

    // Create a new structure to store the result
    struct gf one, comp, result;

    // Allocate memory for the arrays
    result.num = (u64*)malloc(num1->len * sizeof(u64));
    comp.num = (u64*)malloc(num1->len * sizeof(u64));
    one.num = (u64*)malloc(sizeof(u64));

    memset(result.num, 0, num1->len * sizeof(u64));  // Initialize with zeros
    memset(comp.num, 0, num1->len * sizeof(u64));
    memset(one.num, 0, sizeof(u64));
    one.num[0] = 1;

    result.len = num1->len;
    comp.len = num1->len;

    for (int i = 0; i < num1->len; i++)
		comp.num[i]= ~num2->num[i];	// 1's complement of the subtrahend    
	
    // Add 2's complement (1's complement + 1)
    struct gf two_Comp = add(&comp, &one); // 2's complement of num2

    // Perform addition with num1
    struct gf sub_Res = add(num1, &two_Comp);

     // Free the temporary structures
    free(comp.num);
    free(one.num);
    free(two_Comp.num); // Only if add allocates new memory

    // Handle possible overflow (carry)
    if (sub_Res.len > num1->len) {
        // If there is a carry, ignore it (usually indicates an overflow in unsigned arithmetic)
        memmove(sub_Res.num, sub_Res.num + 1, (num1->len) * sizeof(u64));
        sub_Res.len--;
    }

    // Remove leading zeros
    while (sub_Res.len > 1 && sub_Res.num[0] == 0) {
        memmove(sub_Res.num, sub_Res.num + 1, (sub_Res.len - 1) * sizeof(u64));
        sub_Res.len--;
    }

    return sub_Res;  // Return the result

	// result = add(&comp, &one);		// 2's complement of the subtrahend
    // memset(one.num, 0, one.len * sizeof(u64));
	// one = add(num1, &result); //struct number result;
	
    // //Ignore the carry bit(if there is any)
    // if (one.len > num1->len){
    //     memmove(one.num, one.num + 1 , (num1->len) *4);
    //     one.len--;
    // }
    // while (one.num[0] == 0 && one.len > 1){
    //     memmove(one.num, one.num + 1 , one.len*4 - 4);
    //     one.len--;
    // }

    // return one;
}


#endif  // End include guard