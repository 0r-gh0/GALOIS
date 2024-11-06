#include "readfile.h"
#include "add&sub.h"
#include "karatsuba.h"
//#include "mont.h"
#include <openssl/rand.h>


int main(){
/*
    // Calculate the length of the array (8 hex characters = 1 unsigned 32-bit integer)
    size_t hexLen = strlen(prime);
    size_t arrayLen = hexLen / 8;  // 32 bits = 8 hex characters
	struct number bigprime = makeInt(arrayLen);
    // Parse the hex string into the array of unsigned 32-bit integers
    for (size_t i = 0; i < arrayLen; i++)
        sscanf(&prime[i * 8], "%8x", &bigprime.num[i]);
*/

	struct number num1, num2, result;
	makeInt(&num1,8);
	makeInt(&num2,8);
	//for(int i=0; i<2;i++)
	//num1.num[i]=num2.num[i]=0x12345678;
	//num1.num[0]=num1.num[1]=num1.num[2]=num1.num[3]=0x12345678;
	//num2.num[0]=num2.num[1]=num2.num[2]=num2.num[3]=0x12345678;
	RAND_bytes((unsigned char*)num1.num, num1.len * 4);
	RAND_bytes((unsigned char*)num2.num, num2.len * 4);
	for(int i=0; i<num1.len; i++)
	    printf("%08x",num1.num[i]);
	printf("\n");
	for(int i=0; i<num2.len; i++)
    	printf("%08x",num2.num[i]);
	printf("\n");
    //getInt(infile,&num1, &num2);
	for (int i=0; i<10000;i++)
	karatsuba(&num1,&num2,&result);
	for(int i=0; i<result.len; i++)
    	printf("%08x",result.num[i]);
	//printInt(result, outfile);
	//printf("Result has been written to %s \n", outfile);
	free(num1.num);
	free(num2.num);
	free(result.num);
	return 0;
} 
