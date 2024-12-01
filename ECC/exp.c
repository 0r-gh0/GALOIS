#include "main.h"
#include "utils.h"
#include "add_sub.h"
#include "mul.h
#include "exp.h"

// Right-to-Left Modular Exponentiation
void Exp_RTL(gf base, gf exp, gf* res)
{   
    // Initialize result to 1
    for (int i = 0; i < res->size; i++) {
        res->num[i] = 0;
    }
    res->num[0] = 1; 

    // Iterate through each chunk of the exponent
    for (int i = exp.size - 1; i >= 0; i--) { 
        u64 current = exp.num[i]; 
        
        // Process each bit in the current chunk
        for (int j = 0; j < 64; j++) { 
            // Check if current bit is 1
            if (current & (1ULL << j)) { 
                // Multiply result by base if bit is 1
                MultInZp(*res, base, res); 
            }

            // Always square the base
            MultInZp(base, base, &base);
        }
    }
}

// Left-to-Right Modular Exponentiation
void Exp_LTR(gf base, gf exp, gf* res)
{   
    // Initialize result to 1
    for (int i = 0; i < res->size; i++) {
        res->num[i] = 0;
    }
    res->num[0] = 1;

    // Process each chunk of the exponent
    for (int i = exp.size - 1; i >= 0; i--) {
        u64 current = exp.num[i];
        
        // Process bits from most significant to least significant
        for (int k = 63; k >= 0; k--) {
            // Square the current result
            MultInZp(*res, *res, res);
            
            // Multiply by base if current bit is 1
            if (current & (1ULL << k)) {
                MultInZp(*res, base, res); 
            }
        }
    }
}

// Efficient Left-to-Right Modular Exponentiation without branching
void Exp_LTR_e(gf base, gf exp, gf* res) {   
    // Initialize result to 1
    for (int i = 0; i < res->size; i++) {
        res->num[i] = 0;
    }
    res->num[0] = 1;

    // Process each chunk of the exponent
    for (int i = exp.size - 1; i >= 0; i--) {
        u64 current = exp.num[i];
        
        // Process bits from most significant to least significant
        for (int k = 63; k >= 0; k--) {
            // Always square the result
            MultInZp(*res, *res, res);
            
            // Use bitwise masking to conditionally multiply
            gf temp_result;
            temp_result = init(res->size);
            MultInZp(*res, base, &temp_result);
            
            // Conditionally update result using bit mask
            for (int idx = 0; idx < res->size; idx++) {
                u64 bit_mask = (current >> k) & 1;
                res->num[idx] = (res->num[idx] & ~bit_mask) | 
                                (temp_result.num[idx] & bit_mask);
            }
            
            free_gf(&temp_result);
        }
    }
}