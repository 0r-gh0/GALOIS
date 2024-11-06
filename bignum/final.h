//E92E40AD6F281C8A082AFDC49E1372659455BEC8CEEA043A614C835B7FE9EFF5
#include"sbookmul.h"
#include "sha256.h"
//#include "gcm.h"
#include <openssl/rand.h>  // For RAND_bytes


void rand_str(char* output) {
    // Length in bytes (since 1 byte = 2 hex characters, divide by 2)
    unsigned char random_bytes[32];

    // Generate random bytes using OpenSSL RAND_bytes
    if (RAND_bytes(random_bytes, 32) != 1) {
        fprintf(stderr, "Error generating random bytes\n");
        exit(EXIT_FAILURE);
    }

    // Convert each byte to its hex representation
    for (size_t i = 0; i < 32; i++) 
        sprintf(output + (i * 2), "%02x", random_bytes[i]);

    output[0] = 48;
    output[64] = '\0';    // Null-terminate the string
}


void public_key_generation(char* s_key, char* p_key){
    char my_prime[] = "E92E40AD6F281C8A082AFDC49E1372659455BEC8CEEA043A614C835B7FE9EFF5";
    parse_to_int(my_prime, m);
    
    char my_mu[] = "35b5a40bbb8b91a5ac84a4a1180915a5eeac095be5dc75ddafa730293ae00018";
    parse_to_int(my_mu, mu);
    mu[0]= 35;
    mu[1] ^= 0x4000000;

    g[9] = 0x2;

    u64 exp[10] = {0};
    rand_str(s_key);
    parse_to_int(s_key, exp);
  
    u64 result[10] = {0};
    modular_pow(g, exp, result);
    parse_to_hex(result ,p_key);
    p_key[64] = '\0';

    printf("Server SECRET KEY: %s\n",s_key);
    printf("Server PUBLIC KEY: %s\n",p_key);
}

void shared_key_generation(char* s_key, char* p_key, u8* aes_key){
    u64 exp[10] = {0};
    parse_to_int(s_key, exp);

    u64 base[10] = {0};
    parse_to_int(p_key, base);

    u64 result[10] = {0};
    modular_pow(base, exp, result);
	char shared_key[65]={48};
    parse_to_hex(result ,shared_key);
    shared_key[64] = '\0';

    printf("SHARED KEY: %s\n",shared_key);
    
    u8 hash[32];
    sha256((u8 *)shared_key, strlen(shared_key), hash);
    print_key(hash, aes_key);
}
