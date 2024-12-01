#include "main.h"
#include "utils.h"
#include "add_sub.h"

// Comparison function to check if one gf is less than another
int compare(gf a, gf b) {
    // Compare from most significant chunk downwards
    for (int i = a.size - 1; i >= 0; i--) {
        if (a.num[i] != b.num[i]) {
            return a.num[i] < b.num[i];
        }
    }
    return 0; // Numbers are equal
}

// Addition of two multi-precision integers
void Add(gf a, gf b, gf* res) {
    u64 carry = 0;
    
    // Determine the maximum size for addition
    int max_size = (a.size > b.size) ? a.size : b.size;
    
    // Perform addition chunk by chunk
    for (int i = 0; i < max_size; i++) {
        // Get values, defaulting to 0 if out of bounds
        u64 a_val = (i < a.size) ? a.num[i] : 0;
        u64 b_val = (i < b.size) ? b.num[i] : 0;
        
        // Add current chunks and previous carry
        u64 sum = a_val + b_val + carry;
        
        // Store lower 29 bits
        res->num[i] = sum & 0x1FFFFFFF;
        
        // Calculate carry for next iteration
        carry = sum >> 29;
    }
    
    // Handle final carry if needed
    if (carry) {
        res->num[max_size] = carry & 0x1FFFFFFF;
    }
}

void Sub(gf a, gf b, gf* res, gf p) {
    u64 borrow = 0;
    
    // Iterate through all chunks
    for (int i = 0; i < a.size; i++) {
        // Get current chunks, defaulting to 0 if out of bounds
        u64 a_val = a.num[i];
        u64 b_val = (i < b.size) ? b.num[i] : 0;
        
        // Add p to handle negative results
        u64 adjusted_a = a_val + (borrow ? 0 : 0x20000000);
        u64 adjusted_b = b_val + (borrow ? 1 : 0);
        
        // Perform subtraction with borrow handling
        if (adjusted_a < adjusted_b) {
            res->num[i] = (adjusted_a + 0x20000000) - adjusted_b;
            borrow = 1;
        } else {
            res->num[i] = adjusted_a - adjusted_b;
            borrow = 0;
        }
        
        // Mask to 29 bits
        res->num[i] &= 0x1FFFFFFF;
    }
}


// Addition in Zp (modulo prime)
void AddInZp(gf a, gf b, gf* res, gf p) {
    // Perform standard addition
    Add(a, b, res);
    
    // If result is greater than or equal to prime, subtract prime
    if (!compare(*res, p)) {
        Sub(*res, p, res, p);
    }
}
