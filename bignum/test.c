/*#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define PRIME_SIZE 64

// Function to perform modular multiplication
void modular_multiply(uint32_t* result, const uint32_t* a, const uint32_t* b, const uint32_t* modulus) {
    uint64_t temp[PRIME_SIZE * 2] = {0};
    
    for (int i = 0; i < PRIME_SIZE; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < PRIME_SIZE; j++) {
            uint64_t product = (uint64_t)a[i] * b[j] + temp[i + j] + carry;
            temp[i + j] = product & 0xFFFFFFFF;
            carry = product >> 32;
        }
        temp[i + PRIME_SIZE] = carry;
    }
    
    for (int i = PRIME_SIZE * 2 - 1; i >= PRIME_SIZE; i--) {
        uint64_t carry = temp[i];
        for (int j = PRIME_SIZE - 1; j >= 0; j--) {
            uint64_t product = (uint64_t)modulus[j] * carry + temp[i - PRIME_SIZE + j];
            temp[i - PRIME_SIZE + j] = product & 0xFFFFFFFF;
            carry = product >> 32;
        }
    }
    
    memcpy(result, temp, sizeof(uint32_t) * PRIME_SIZE);
}

// Function to perform modular exponentiation
void modular_exponentiation(uint32_t* result, const uint32_t* base, const uint32_t* exponent, const uint32_t* modulus) {
    uint32_t temp[PRIME_SIZE];
    memset(result, 0, sizeof(uint32_t) * PRIME_SIZE);
    result[0] = 1;
    
    for (int i = PRIME_SIZE - 1; i >= 0; i--) {
        for (int j = 31; j >= 0; j--) {
            modular_multiply(temp, result, result, modulus);
            memcpy(result, temp, sizeof(uint32_t) * PRIME_SIZE);
            
            if (exponent[i] & (1U << j)) {
                modular_multiply(temp, result, base, modulus);
                memcpy(result, temp, sizeof(uint32_t) * PRIME_SIZE);
            }
        }
    }
}

int main() {
    // The prime field
    uint32_t modulus[PRIME_SIZE] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xC90FDAA2, 0x2168C234, 0xC4C6628B, 0x80DC1CD1,
        0x29024E08, 0x8A67CC74, 0x020BBEA6, 0x3B139B22, 0x514A0879, 0x8E3404DD,
        0xEF9519B3, 0xCD3A431B, 0x302B0A6D, 0xF25F1437, 0x4FE1356D, 0x6D51C245,
        0xE485B576, 0x625E7EC6, 0xF44C42E9, 0xA637ED6B, 0x0BFF5CB6, 0xF406B7ED,
        0xEE386BFB, 0x5A899FA5, 0xAE9F2411, 0x7C4B1FE6, 0x49286651, 0xECE45B3D,
        0xC2007CB8, 0xA163BF05, 0x98DA4836, 0x1C55D39A, 0x69163FA8, 0xFD24CF5F,
        0x83655D23, 0xDCA3AD96, 0x1C62F356, 0x208552BB, 0x9ED52907, 0x7096966D,
        0x670C354E, 0x4ABC9804, 0xF1746C08, 0xCA18217C, 0x32905E46, 0x2E36CE3B,
        0xE39E772C, 0x180E8603, 0x9B2783A2, 0xEC07A28F, 0xB5C55DF0, 0x6F4C52C9,
        0xDE2BCBF6, 0x95581718, 0x3995497C, 0xEA956AE5, 0x15D22618, 0x98FA0510,
        0x15728E5A, 0x8AACAA68, 0xFFFFFFFF, 0xFFFFFFFF
    };

    // Example base and exponent (you can modify these)
    uint32_t base[PRIME_SIZE] = {2};  // Using 2 as the base
    uint32_t exponent[PRIME_SIZE] = {3};  // Using 3 as the exponent

    uint32_t result[PRIME_SIZE];

    modular_exponentiation(result, base, exponent, modulus);

    printf("Result of modular exponentiation:\n");
    for (int i = PRIME_SIZE - 1; i >= 0; i--) {
        printf("%08X ", result[i]);
        if (i % 8 == 0) printf("\n");
    }

    return 0;
}*/


E92E40AD6F281C8A082AFDC49E1372659455BEC8CEEA043A614C835B7FE9EFF5