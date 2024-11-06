#include <wmmintrin.h> // For AES-NI instructions

// Perform AES key expansion using AES-NI
void aes_key_expansion(const u8 *key, __m128i *round_keys) {
    round_keys[0] = _mm_loadu_si128((__m128i *)key);
    __m128i temp1, temp2;

    temp2 = _mm_aeskeygenassist_si128(round_keys[0], 0x01);
    round_keys[1] = _mm_xor_si128(round_keys[0], _mm_slli_si128(round_keys[0], 4));
    round_keys[1] = _mm_xor_si128(round_keys[1], _mm_slli_si128(round_keys[1], 4));
    round_keys[1] = _mm_xor_si128(round_keys[1], _mm_slli_si128(round_keys[1], 4));
    round_keys[1] = _mm_xor_si128(round_keys[1], _mm_shuffle_epi32(temp2, 0xff));

    // Repeat key expansion for further rounds
    // The code for key expansion would be repeated for the full AES-128 key schedule (11 rounds total).
}

// Perform AES encryption using AES-NI
__m128i aes_encrypt_block(__m128i block, const __m128i *round_keys) {
    block = _mm_xor_si128(block, round_keys[0]);

    for (int i = 1; i < 10; i++) {
        block = _mm_aesenc_si128(block, round_keys[i]);
    }

    block = _mm_aesenclast_si128(block, round_keys[10]);
    return block;
}

// Perform AES-CTR encryption (encrypting plaintext block with counter and AES)
void aes_ctr_encrypt(const u8 *key, const u8 *iv, const u8 *plaintext, u8 *ciphertext, u64 length) {
    __m128i round_keys[11];
    aes_key_expansion(key, round_keys);

    __m128i counter = _mm_loadu_si128((__m128i *)iv);

    u8 block[AES_BLOCK_SIZE];
    for (u64 i = 0; i < length; i += AES_BLOCK_SIZE) {
        __m128i encrypted_counter = aes_encrypt_block(counter, round_keys);
        _mm_storeu_si128((__m128i *)block, encrypted_counter);

        for (int j = 0; j < AES_BLOCK_SIZE && i + j < length; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ block[j];
        }

        // Increment counter
        counter = _mm_add_epi64(counter, _mm_set_epi64x(0, 1));
    }
}

// Galois field multiplication (GF(2^128)) using AES-NI
__m128i ghash_multiply(__m128i X, __m128i Y) {
    return _mm_clmulepi64_si128(X, Y, 0x00);  // Perform carry-less multiplication
}

// Perform GHASH (Galois hash) for GCM mode
void ghash(const u8 *H, const u8 *input, u64 length, u8 *output) {
    __m128i H_key = _mm_loadu_si128((__m128i *)H);
    __m128i result = _mm_setzero_si128();

    for (u64 i = 0; i < length; i += AES_BLOCK_SIZE) {
        __m128i block = _mm_loadu_si128((__m128i *)&input[i]);
        result = _mm_xor_si128(result, block);
        result = ghash_multiply(result, H_key);
    }

    _mm_storeu_si128((__m128i *)output, result);
}

// Perform AES-GCM encryption
void aes_gcm_encrypt(const u8 *key, const u8 *iv, const u8 *plaintext, u8 *ciphertext, u64 length, const u8 *aad, u64 aad_len, u8 *tag) {
    __m128i H;
    u8 hash_subkey[AES_BLOCK_SIZE] = {0};

    // 1. Generate hash subkey H = AES_k(0^128)
    aes_ctr_encrypt(key, (u8[]){0}, hash_subkey, hash_subkey, AES_BLOCK_SIZE);
    H = _mm_loadu_si128((__m128i *)hash_subkey);

    // 2. Encrypt plaintext using AES-CTR mode
    aes_ctr_encrypt(key, iv, plaintext, ciphertext, length);

    // 3. Compute GHASH(AAD || ciphertext || lengths)
    u8 ghash_input[AES_BLOCK_SIZE * 2]; // Simplified, should be AAD || ciphertext
    memset(ghash_input, 0, sizeof(ghash_input));
    ghash(hash_subkey, ghash_input, sizeof(ghash_input), tag);

    // Authentication Tag is now in 'tag'
}

int main() {
    // Example usage of AES-GCM
    const u8 key[16] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81};
    const u8 iv[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    const u8 plaintext[64] = "Hello, this is a plaintext message to be encrypted!";
    u8 ciphertext[64];
    u8 tag[16];

    aes_gcm_encrypt(key, iv, plaintext, ciphertext, strlen((char *)plaintext), NULL, 0, tag);

    printf("Ciphertext: ");
    for (int i = 0; i < strlen((char *)plaintext); i++) {
        printf("%02x ", ciphertext[i]);
    }
    printf("\nTag: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", tag[i]);
    }
    printf("\n");

    return 0;
}
