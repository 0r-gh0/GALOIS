#include <stdio.h>
#include <openssl/rand.h>
#include "read.h"
#include "add_sub.h"

int main(){
	struct gf num1, num2, result, pad0, pad1;
	makeInt(&num1,10);
	makeInt(&num2,10);

	// Binary data for each block
    char data1[9][65] = {
        "10100110110100110000100100000",
        "01100001010100010001111111000",
        "11010010000001001100000001111",
        "10010110110111100101010011001",
        "11101110110101001010001001110",
        "11000111000011011010110100110",
        "01110101110110101111011101111",
        "11101011001110110100110001110",
        "00000101111111111001110101110"
    };

	char data2[9][65] = {
		"11000010111110100100101011001",
		"01011100001100100111010110001",
		"10010000000001110101010010100",
		"10111010000001011100011011110",
		"11011011000101101100000011111",
		"00111000111100010101000101010",
		"11001010100011100100100000111",
		"11111111101111010110100110011",
		"00000010000101111000001100111",
	};


    // fill29(&num1);
    // fill29(&num2);

	fill(&num1, data1, 9);
	fill(&num2, data2, 9);

    printf("\nNumber 1 : \n");
    for(int i = num1.len - 1; i >= 0; i--)
        printf("%08llx \n", num1.num[i]);

    printf("\nNumber 2 : \n");
	for(int i = num2.len - 1; i >= 0; i--)
        printf("%08llx \n", num2.num[i]);

    // pad1 = sub(&num1, &num2);

	/*
    printf("\nNumber 4 : \n");
	for(int i = pad1.len - 1; i >= 0; i--)
        printf("%016llx \n", pad1.num[i]);
	printf("\n");
	*/

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
