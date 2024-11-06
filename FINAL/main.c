#include <stdio.h>
#include <openssl/rand.h>
#include "read.h"
#include "add_sub.h"


int main(){
	struct gf num1, num2, result, pad0, pad1;
	makeInt(&num1,9);
	makeInt(&num2,9);

    RAND_bytes((unsigned char*) &num1.num[0], 4);
    RAND_bytes((unsigned char*) &num1.num[1], 4);
    RAND_bytes((unsigned char*) &num1.num[2], 4);
    RAND_bytes((unsigned char*) &num1.num[3], 4);
    RAND_bytes((unsigned char*) &num1.num[4], 4);
    RAND_bytes((unsigned char*) &num1.num[5], 4);
    RAND_bytes((unsigned char*) &num1.num[6], 4);
    RAND_bytes((unsigned char*) &num1.num[7], 4);

    RAND_bytes((unsigned char*) &num2.num[0], 4);
    RAND_bytes((unsigned char*) &num2.num[1], 4);
    RAND_bytes((unsigned char*) &num2.num[2], 4);
    RAND_bytes((unsigned char*) &num2.num[3], 4);
    RAND_bytes((unsigned char*) &num2.num[4], 4);
    RAND_bytes((unsigned char*) &num2.num[5], 4);
    RAND_bytes((unsigned char*) &num2.num[6], 4);
    RAND_bytes((unsigned char*) &num2.num[7], 4);

    printf("\nNumber 1 : ");
    for(int i = num1.len - 1; i >= 0; i--)
        printf("%08llx", num1.num[i]);

    printf("\nNumber 2 : ");
	for(int i = num2.len - 1; i >= 0; i--)
        printf("%08llx", num2.num[i]);

    pad1 = sub(&num1, &num2);
    printf("\nNumber 3 : ");
	for(int i = pad1.len - 1; i >= 0; i--)
        printf("%08llx", pad1.num[i]);
	printf("\n");

    printf("\nNumber 4 : \n");
	for(int i = pad1.len - 1; i >= 0; i--)
        printf("%010llx\n", pad1.num[i]);
	printf("\n");

    // Pad Testing
    // pad0 = pad(&num2,12);
    // printf("\n Number 3 : ");
	// for(int i = pad0.len - 1; i >= 0; i--)
    //     printf("%08llx\n", pad0.num[i]);
    // eqLen(&pad0, &num1);

    // // Equal Length
    // printf("Number 4 : ");
    // for(int i = num1.len - 1; i >= 0; i--)
    //     printf("%08llx\n", num1.num[i]);
    // return 0;
} 
