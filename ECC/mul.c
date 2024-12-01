#include "main.h"
#include "utils.h"
#include "add_sub.h"
#include "mul.h"

// Multiplication of two multi-precision integers
void Mul(gf a, gf b, gf* res) {
    // Reset result to zero to prevent accumulation
    memset(res->num, 0, res->size * sizeof(u64));

    // Determine the maximum possible size of multiplication result
    int s = a.size + b.size;

    // Perform multiplication
    for (int i = 0; i < a.size; i++) { 
        u64 carry = 0;
        for (int j = 0; j < b.size; j++) {
            // Multiply current chunks and add to result with carry
            u64 product = (u64)a.num[i] * b.num[j];
            u64 current = res->num[i + j] + product + carry;
            
            // Store lower 29 bits
            res->num[i + j] = current & 0x1FFFFFFF;
            
            // Calculate carry for next iteration
            carry = current >> 29;
        }
        
        // Handle any remaining carry
        if (carry) {
            res->num[i + b.size] += carry;
        }
    }

    // Normalize result to ensure each chunk is 29 bits
    for (int i = 0; i < s - 1; i++) { 
        res->num[i + 1] += (res->num[i] >> 29);
        res->num[i] &= 0x1FFFFFFF;
    }
    
    // Ensure the most significant chunk is also masked
    res->num[s - 1] &= 0x1FFFFFFF;
}

// Barrett Reduction implementation
void BReduction(gf x, gf* res) {
    // Create temporary variables with appropriate sizes
    gf q1 = init(10); 
    gf q2 = init(20);
    gf q4 = init(19);

    // Step 1: Extract relevant chunks from x
    for (int i = 0; i < 10; i++) { 
        q1.num[i] = x.num[i + 8];
    }
   
    // Step 2: Multiply q1 by mu (precomputed reduction constant)
    Mul(q1, mu, &q2);

    // Step 3: Extract appropriate chunks for next multiplication
    for (int i = 0; i < 10; i++) { 
        q1.num[i] = q2.num[10 + i];
    }

    // Step 4: Multiply q1 by prime modulus p
    Mul(q1, p, &q4);

    // Step 5: Subtract q4 from original number
    Sub(x, q4, &q4);

    // Step 6: Reduce if result is larger than prime
    while(compare(p, q4)) { 
        Sub(q4, p, &q4);
    }

    // Step 7: Copy final reduced value to result
    memcpy(res->num, q4.num, res->size * sizeof(u64));

    // Clean up temporary variables
    free_gf(&q1);
    free_gf(&q2);
    free_gf(&q4);
}

// Multiplication in Zp (modulo prime)
void MultInZp(gf a, gf b, gf* res) {
    // Create temporary variable for full multiplication result
    gf temp = init(18); 
    
    // Perform multiplication
    Mul(a, b, &temp);
    
    // Reduce using Barrett reduction
    BReduction(temp, res);
    
    // Clean up temporary variable
    free_gf(&temp);
}