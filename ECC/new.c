#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Typedef for unsigned 64-bit integer to ensure consistent width
typedef unsigned long long u64;

// Structure to represent a multi-precision integer in Galois Field
typedef struct 
{
    u64* num;   // Pointer to array of 29-bit chunks
    int size;   // Number of chunks in the array
} gf;

// Global constants for Galois Field arithmetic
gf g, p, mu;

// Function Prototypes
gf init(int size);
void free_gf(gf* a);
int compare(gf a, gf b);
void Add(gf a, gf b, gf* res);
void Sub(gf a, gf b, gf* res);
void AddInZp(gf a, gf b, gf* res);
void Mul(gf a, gf b, gf* res);
void BReduction(gf x, gf* res);
void MultInZp(gf a, gf b, gf* res);
void print_gf(gf* a);

// Exponentiation Method Prototypes
void Exp_RTL(gf base, gf exp, gf* res);
void Exp_LTR(gf base, gf exp, gf* res);
void Exp_LTR_e(gf base, gf exp, gf* res);

// Initialize a multi-precision integer with given size
gf init(int size) {
    // Allocate memory for the number array
    gf result;
    result.num = (u64*)calloc(size, sizeof(u64));
    
    // Check for memory allocation failure
    if (!result.num) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    
    // Set the size of the array
    result.size = size;
    return result;
}

// Free memory allocated for a multi-precision integer
void free_gf(gf* a) {
    // Check if memory is allocated before freeing
    if (a->num) {
        free(a->num);
        a->num = NULL;
        a->size = 0;
    }
}

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

// Subtraction of two multi-precision integers
void Sub(gf a, gf b, gf* res) {
    u64 borrow = 0;
    
    // Iterate through all chunks
    for (int i = 0; i < a.size; i++) {
        // Get current chunks, defaulting to 0 if out of bounds
        u64 a_val = a.num[i];
        u64 b_val = (i < b.size) ? b.num[i] : 0;
        
        // Add 2^29 to handle negative results
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
void AddInZp(gf a, gf b, gf* res) {
    // Perform standard addition
    Add(a, b, res);
    
    // If result is greater than or equal to prime, subtract prime
    if (!compare(*res, p)) {
        Sub(*res, p, res);
    }
}

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

// Debugging function to print multi-precision integer
void print_gf(gf* a) {
    printf("Size: %d, Values: ", a->size);
    for (int i = a->size - 1; i >= 0; i--) {
        printf("%llu ", a->num[i]);
    }
    printf("\n");
}

int main() {
    // Initialize global constants
    // Prime modulus p
    p = init(9);
    p.num[0] = 535425013;
    p.num[1] = 174332635;
    p.num[2] = 444665496;
    p.num[3] = 192778653;
    p.num[4] = 388389189;
    p.num[5] = 518147849;
    p.num[6] = 304619691;
    p.num[7] = 363717891;
    p.num[8] = 15281728;

    // Precomputed reduction constant mu
    mu = init(10);
    mu.num[0] = 450887704;
    mu.num[1] = 490307913;
    mu.num[2] = 387807083;
    mu.num[3] = 403879883;
    mu.num[4] = 291135210;
    mu.num[5] = 307268612;
    mu.num[6] = 110539282;
    mu.num[7] = 24605042;
    mu.num[8] = 70628772;
    mu.num[9] = 35;

    // Example usage demonstration
    gf a = init(9);
    a.num[0] = 4;
    a.num[3] = 12;
    
    gf b = init(9);
    b.num[0] = 10;
    b.num[1] = 10;
    
    gf result = init(9);
    
    printf("a : ");
    print_gf(&a);
    printf("b : ");
    print_gf(&b);
    
    // Demonstrate addition
    printf("Addition Demonstration:\n");
    AddInZp(a, b, &result);
    printf("a + b (mod p) : ");
    print_gf(&result);
    
    // Demonstrate subtraction
    printf("\nSubtraction Demonstration:\n");
    Sub(a, b, &result);
    printf("a - b (mod p) : ");
    print_gf(&result);
    
    // Demonstrate multiplication in Zp
    printf("\nMultiplication in Z_p Demonstration:\n");
    MultInZp(a, b, &result);
    printf("a * b (mod p): ");
    print_gf(&result);
    
    gf base = init(9);
    gf exp = init(9);
    gf res = init(9);
    // Example values
    base.num[0] = 2;     // Base: 2
    exp.num[0] = 29;     // Exponent: 29

    // Print input values
    printf("Base: ");
    print_gf(&base);
    printf("Exponent: ");
    print_gf(&exp);

    // Perform and compare different exponentiation methods
    printf("\n1. Right-to-Left Binary Method:\n");
    Exp_RTL(base, exp, &res);
    printf("Result (RTL): ");
    print_gf(&res);

    printf("\n2. Left-to-Right Binary Method:\n");
    Exp_LTR(base, exp, &res);
    printf("Result (LTR): ");
    print_gf(&res);

    printf("\n3. Efficient Left-to-Right Method:\n");
    Exp_LTR_e(base, exp, &res);
    printf("Result (LTR Efficient): ");
    print_gf(&res);

    // Clean up allocated memory
    free_gf(&a);
    free_gf(&b);
    free_gf(&result);
    free_gf(&p);
    free_gf(&mu);
    free_gf(&base);
    free_gf(&exp);
    free_gf(&res);
    return 0;
}
