#include <string.h>

#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define EP0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define EP1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SIG0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))
#define SIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))

// SHA-256 constants
static const u32 k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// SHA-256 initial hash values
static const u32 h0_init[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

// Padding and message scheduling
void sha256_transform(u32 state[8], u8 block[64]) {
    u32 a, b, c, d, e, f, g, h, t1, t2, m[64];
    int i, j;

    // Message schedule array
    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (block[j] << 24) | (block[j + 1] << 16) | (block[j + 2] << 8) | (block[j + 3]);
    for (; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

    // Initialize working variables to current hash value
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];

    // Compression function
    for (i = 0; i < 64; ++i) {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    // Add the compressed chunk to the current hash value
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

// Utility functions for padding and processing the final message
void sha256_update(u32 state[8], u8 data[], u32 len, u64 *bitlen, u8 buffer[64]) {
    u32 i;

    for (i = 0; i < len; ++i) {
        buffer[*bitlen / 8 % 64] = data[i];
        *bitlen += 8;
        if (*bitlen % 512 == 0) {
            sha256_transform(state, buffer);
        }
    }
}

void sha256_final(u32 state[8], u8 buffer[64], u64 bitlen, u8 hash[32]) {
    int i;

    i = bitlen / 8 % 64;
    buffer[i++] = 0x80;
    if (i > 56) {
        while (i < 64) buffer[i++] = 0x00;
        sha256_transform(state, buffer);
        i = 0;
    }
    while (i < 56) buffer[i++] = 0x00;

    // Append the original message length in bits
    bitlen = __builtin_bswap64(bitlen);
    memcpy(buffer + 56, &bitlen, 8);
    sha256_transform(state, buffer);

    // Convert state to hash (big-endian)
    for (i = 0; i < 8; ++i)
        state[i] = __builtin_bswap32(state[i]);
    memcpy(hash, state, 32);
}

// The main SHA-256 function
void sha256(u8 *data, size_t len, u8 hash[32]) {
    u32 state[8];
    u8 buffer[64] = {0};
    u64 bitlen = 0;

    // Initialize hash state
    memcpy(state, h0_init, sizeof(h0_init));

    // Update hash with input data
    sha256_update(state, data, len, &bitlen, buffer);

    // Final padding and produce the hash
    sha256_final(state, buffer, bitlen, hash);
}

// Utility function to print the hash as a hex string
void print_hash( u8 hash[32]) {
    for (int i = 0; i < 32; i++)
        printf("%02x", hash[i]);
    printf("\n");
}

void print_key( u8* hash, u8* key) {
    printf("SESSION KEY: ");
    for (int i = 0; i < 16; i++){
        key[i] = hash[i];
        printf("%02x", hash[i]);
    }
    printf("\n");
}