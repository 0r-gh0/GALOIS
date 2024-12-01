#include "main.h"
#include "utils.h"
#include "add_sub.h"
#include "mul.h"
#include "exp.h"

// Global constants for Galois Field arithmetic
gf g, p, mu;

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