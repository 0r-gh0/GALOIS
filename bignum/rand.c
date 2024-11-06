/*#include <openssl/rand.h> //For Random IV Generation
#include <stdio.h>
long long int a[9];
// Generate random bytes for the IV
void generate_iv() {
    if (!RAND_bytes(a, sizeof(a))) {
        perror("Error generating random IV");
        exit(EXIT_FAILURE);
    }
}

int main() {
    generate_iv();
    a[0] = a[0] & 0x000000003fffffff;
    for(int i=0; i<9; i++)
    printf("%llx ",a[i]);
     printf("%d",sizeof(unsigned long long int));
}*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Hex string (without spaces)
    const char* hexString = "1A2B3C4D05E6F708"; // Example: 16 hex chars = 2 * 32-bit unsigned integers
    
    // Calculate the length of the array (8 hex characters = 1 unsigned 32-bit integer)
    size_t hexLen = strlen(hexString);
    size_t arrayLen = (hexLen+7) / 8;  // 32 bits = 8 hex characters

    // Allocate array to store parsed 32-bit unsigned integers
    unsigned int *intArray = malloc(arrayLen * sizeof(unsigned int));

    // Parse the hex string into the array of unsigned 32-bit integers
    for (size_t i = arrayLen - 1; i >= 0; i--) {
        // Extract 8 hex characters, convert to unsigned 32-bit integer and store
        sscanf(&hexString[i * 8], "%8x", &intArray[i]);
    }

    // Print the parsed 32-bit unsigned integers
    printf("Parsed unsigned 32-bit integers:\n");
    for (size_t i = 0; i < arrayLen; i++) {
        printf("%08X ", intArray[i]);
    }
    printf("\n");

    // Free the allocated memory
    free(intArray);

    return 0;
}


for(int i=0; i<10;i++)
		result[i]=0;

