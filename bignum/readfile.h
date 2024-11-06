#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int u32;
typedef unsigned long long int u64;

// Structure to store a number along with its length in byte
struct number{
	int len;
	u32 *num;
};

// Function to create a dyamically allocated array to store a number
void makeInt(struct number* input, int SIZE){
    input->num = (u32*)malloc(SIZE * sizeof(u32));
	memset(input->num,0,SIZE * 4);
	input->len = SIZE;
}

//const char* prime = "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF0 6F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF";

/*
// Function to convert a hexadecimal string to an array of u32
u32 hextoint(char hex) {
	//Conversion of hex characters to decimal value 
    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    } else if (hex >= 'a' && hex <= 'f') {
        return hex - 'a' + 10;
    } else if (hex >= 'A' && hex <= 'F') {
        return hex - 'A' + 10;
    }
    return -1;						// Invalid hexadecimal character
}
struct number hexToInt(char* hex,int size){
	int s = (size+7)/8;
	struct number bigInt= makeInt(s);
	for (int i=size-1; i>=0; i--)
		bigInt.num[s-1] = hextoint(hex[i])    ^ hextoint(hex[i+1])<<4
						^ hextoint(hex[i])<<8 ^ hextoint(hex[i+1])<<12
						^ hextoint(hex[i])<<16 ^ hextoint(hex[i+1])<<20
						^ hextoint(hex[i])<<24 ^ hextoint(hex[i+1])<<28;
	return bigInt;
}*/