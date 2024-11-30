#ifndef ADD_SUB_H
#define ADD_SUB_H

#include "read.h"

// Function to ensure two numbers have the same length by padding with zeros
void eqLen(struct gf *num1, struct gf *num2) {
    int maxLen = (num1->len > num2->len) ? num1->len : num2->len;

    if (num1->len < maxLen) {
        num1->num = (u64 *)realloc(num1->num, maxLen * sizeof(u64));
        memset(num1->num + num1->len, 0, (maxLen - num1->len) * sizeof(u64));
        num1->len = maxLen;
    }

    if (num2->len < maxLen) {
        num2->num = (u64 *)realloc(num2->num, maxLen * sizeof(u64));
        memset(num2->num + num2->len, 0, (maxLen - num2->len) * sizeof(u64));
        num2->len = maxLen;
    }
}

// Function to perform modular reduction
void modReduce(struct gf *num, const struct gf *prime) {
    while (num->len > prime->len || (num->len == prime->len && num->num[num->len - 1] >= prime->num[prime->len - 1])) {
        for (int i = 0; i < num->len; i++) {
            u64 borrow = (prime->num[i] > num->num[i]);
            num->num[i] = (num->num[i] - prime->num[i]) & 0xFFFFFFFFFFFFFFFF;
            if (i + 1 < num->len)
                num->num[i + 1] -= borrow;
        }
        while (num->len > 1 && num->num[num->len - 1] == 0)
            num->len--;
    }
}

// Function to add two numbers modulo the prime
struct gf add(const struct gf *num1, const struct gf *num2, const struct gf *prime) {
    struct gf result;
    makeInt(&result, num1->len);
    eqLen((struct gf *)num1, (struct gf *)num2);

    u64 carry = 0;
    for (int i = 0; i < result.len; i++) {
        u64 sum = num1->num[i] + num2->num[i] + carry;
        result.num[i] = sum & 0xFFFFFFFFFFFFFFFF;
        carry = (sum < num1->num[i]) || ((sum == num1->num[i]) && carry);
    }

    if (carry) {
        result.num = (u64 *)realloc(result.num, (result.len + 1) * sizeof(u64));
        result.num[result.len++] = carry;
    }

    modReduce(&result, prime);
    return result;
}


// Function to subtract two integers in the field defined by the prime
struct gf sub(struct gf *num1, struct gf *num2, struct gf *prime) {
    eqLen(num1, num2);  // Equalize lengths for subtraction

    // Create a structure for 2's complement of num2
    struct gf one, comp, two_Comp;
    makeInt(&one, num2->len);
    makeInt(&comp, num2->len);

    // Initialize one with a single 1
    one.num[0] = 1;

    // Compute 1's complement of num2
    for (int i = 0; i < num2->len; i++) {
        if (i < num2->len - 1)
            comp.num[i] = ~num2->num[i] & 0x1FFFFFFF;  // Mask to 29 bits
        else
            comp.num[i] = ~num2->num[i] & 0xFFFFFF;  // Mask to 24 bits
    }

    // Add 1 to compute 2's complement of num2
    two_Comp = add(&comp, &one, prime);

    // Perform addition: num1 + 2's complement of num2
    struct gf tempResult = add(num1, &two_Comp, prime);

    // Add prime to ensure result is positive
    struct gf result = add(&tempResult, prime, prime);

    // Reduce the result modulo the prime
    modReduce(&result, prime);

    // Free temporary structures
    free(comp.num);
    free(one.num);
    free(two_Comp.num);
    free(tempResult.num);

    return result;
}

#endif // ADD_SUB_H
