#include "main.h"
#include "utils.h"

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

// Debugging function to print multi-precision integer
void print_gf(gf* a) {
    printf("Size: %d, Values: ", a->size);
    for (int i = a->size - 1; i >= 0; i--) {
        printf("%llu ", a->num[i]);
    }
    printf("\n");
}