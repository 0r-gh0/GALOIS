#include<stdio.h>
#include<stdlib.h>

typedef long long unsigned int u64;
typedef unsigned int u32;
typedef unsigned char u8;

u64 m[10] = {0};
u64 mu[10] = {0};
u64 g[10] = {0};

// Function to convert a hexadecimal string to an array of u32
u64 hextoint(char hex) {
    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    } else if (hex >= 'a' && hex <= 'f') {
        return hex - 'a' + 10;
    } else if (hex >= 'A' && hex <= 'F') {
        return hex - 'A' + 10;
    }
	perror("Invalid hex char");
	exit(EXIT_FAILURE);
}

void parse_to_int(char* hex, u64* bignum){
	bignum[0] = 0;
	bignum[1] =   hextoint(hex[5])      ^ hextoint(hex[4])<<4
				^ hextoint(hex[3])<<8   ^ hextoint(hex[2])<<12
				^ hextoint(hex[1])<<16  ^ hextoint(hex[0])<<20;
    bignum[2] =   hextoint(hex[13])>>3  ^ hextoint(hex[12])<<1
			    ^ hextoint(hex[11])<<5  ^ hextoint(hex[10])<<9
				^ hextoint(hex[9])<<13  ^ hextoint(hex[8])<<17
				^ hextoint(hex[7])<<21  ^ (hextoint(hex[6])& 0xf)<<25;
    bignum[3] =   hextoint(hex[20])>>2  ^ hextoint(hex[19])<<2
			    ^ hextoint(hex[18])<<6  ^ hextoint(hex[17])<<10
				^ hextoint(hex[16])<<14 ^ hextoint(hex[15])<<18
				^ hextoint(hex[14])<<22 ^ (hextoint(hex[13])& 0x7)<<26;
    bignum[4] =   hextoint(hex[27])>>1  ^ hextoint(hex[26])<<3
			    ^ hextoint(hex[25])<<7  ^ hextoint(hex[24])<<11
				^ hextoint(hex[23])<<15 ^ hextoint(hex[22])<<19
				^ hextoint(hex[21])<<23 ^ (hextoint(hex[20])& 0x3)<<27;
    bignum[5] =   hextoint(hex[34])     ^ hextoint(hex[33])<<4
			    ^ hextoint(hex[32])<<8  ^ hextoint(hex[31])<<12
				^ hextoint(hex[30])<<16 ^ hextoint(hex[29])<<20
				^ hextoint(hex[28])<<24 ^ (hextoint(hex[27])& 0x1)<<28;
    bignum[6] =   hextoint(hex[42])>>3  ^ hextoint(hex[41])<<1
			    ^ hextoint(hex[40])<<5  ^ hextoint(hex[39])<<9
				^ hextoint(hex[38])<<13 ^ hextoint(hex[37])<<17
				^ hextoint(hex[36])<<21 ^ (hextoint(hex[35])& 0xf)<<25;
    bignum[7] =   hextoint(hex[49])>>2  ^ hextoint(hex[48])<<2
			    ^ hextoint(hex[47])<<6  ^ hextoint(hex[46])<<10
				^ hextoint(hex[45])<<14 ^ hextoint(hex[44])<<18
				^ hextoint(hex[43])<<22 ^ (hextoint(hex[42])& 0x7)<<26;
    bignum[8] =   hextoint(hex[56])>>1  ^ hextoint(hex[55])<<3
			    ^ hextoint(hex[54])<<7  ^ hextoint(hex[53])<<11
				^ hextoint(hex[52])<<15 ^ hextoint(hex[51])<<19
				^ hextoint(hex[50])<<23 ^ (hextoint(hex[49])& 0x3)<<27;
    bignum[9] =   hextoint(hex[63])     ^ hextoint(hex[62])<<4
			    ^ hextoint(hex[61])<<8  ^ hextoint(hex[60])<<12
				^ hextoint(hex[59])<<16 ^ hextoint(hex[58])<<20
				^ hextoint(hex[57])<<24 ^ (hextoint(hex[56])& 0x1)<<28;
}


void parse_to_hex(u64* bignum, char* hex){
    u64 temp[4];
    temp[0] = bignum[3]>>18 ^ bignum[2]<<11 ^ bignum[1]<<40;
	temp[1] = bignum[5]>>12 ^ bignum[4]<<17 ^ bignum[3]<<46;
	temp[2] = bignum[7]>>6  ^ bignum[6]<<23 ^ bignum[5]<<52;
	temp[3] = bignum[9]     ^ bignum[8]<<29 ^ bignum[7]<<58;
	for (int i=0; i<4; i++)
		sprintf(hex + (i * 16), "%016llx", temp[i]);
	hex[64] = '\0';
}

//Multiplication of 10 word(29bit) long integers
void mult(u64* a, u64* b, u64* p){
	p[19] = a[9]*b[9];
	p[18] = a[8]*b[9] + a[9]*b[8];
	p[17] = a[7]*b[9] + a[8]*b[8] + a[9]*b[7];
	p[16] = a[6]*b[9] + a[7]*b[8] + a[8]*b[7] + a[9]*b[6];
	p[15] = a[5]*b[9] + a[6]*b[8] + a[7]*b[7] + a[8]*b[6] + a[9]*b[5];
	p[14] = a[4]*b[9] + a[5]*b[8] + a[6]*b[7] + a[7]*b[6] + a[8]*b[5] + a[9]*b[4];
	p[13] = a[3]*b[9] + a[4]*b[8] + a[5]*b[7] + a[6]*b[6] + a[7]*b[5] + a[8]*b[4] + a[9]*b[3];
	p[12] = a[2]*b[9] + a[3]*b[8] + a[4]*b[7] + a[5]*b[6] + a[6]*b[5] + a[7]*b[4] + a[8]*b[3] + a[9]*b[2];
	p[11] = a[1]*b[9] + a[2]*b[8] + a[3]*b[7] + a[4]*b[6] + a[5]*b[5] + a[6]*b[4] + a[7]*b[3] + a[8]*b[2] + a[9]*b[1];
	p[10] = a[0]*b[9] + a[1]*b[8] + a[2]*b[7] + a[3]*b[6] + a[4]*b[5] + a[5]*b[4] + a[6]*b[3] + a[7]*b[2] + a[8]*b[1] + a[9]*b[0];
	p[9]  = a[0]*b[8] + a[1]*b[7] + a[2]*b[6] + a[3]*b[5] + a[4]*b[4] + a[5]*b[3] + a[6]*b[2] + a[7]*b[1] + a[8]*b[0];
	p[8]  = a[0]*b[7] + a[1]*b[6] + a[2]*b[5] + a[3]*b[4] + a[4]*b[3] + a[5]*b[2] + a[6]*b[1] + a[7]*b[0];
	p[7]  = a[0]*b[6] + a[1]*b[5] + a[2]*b[4] + a[3]*b[3] + a[4]*b[2] + a[5]*b[1] + a[6]*b[0];
	p[6]  = a[0]*b[5] + a[1]*b[4] + a[2]*b[3] + a[3]*b[2] + a[4]*b[1] + a[5]*b[0];
	p[5]  = a[0]*b[4] + a[1]*b[3] + a[2]*b[2] + a[3]*b[1] + a[4]*b[0];
	p[4]  = a[0]*b[3] + a[1]*b[2] + a[2]*b[1] + a[3]*b[0];
	p[3]  = a[0]*b[2] + a[1]*b[1] + a[2]*b[0];
	p[2]  = a[0]*b[1] + a[1]*b[0];
	p[1]  = a[0]*b[0];
	p[0]  = 0;
	
	u64 carry = 0;
	for(int i=19; i>=0; i--){
		p[i] += carry;
		carry = p[i] >> 29;
		p[i] &= 0x1fffffff;
	}
}

void rshift1(u64* a){
	int temp = 0;
	for (int i = 1; i < 10; i++){
		a[i] ^= temp;
		temp = a[i] & 0x1;
		a[i] = a[i] >> 1;
		temp = temp << 29;
	}
}

unsigned char geq(u64* a, u64* b){
	unsigned char indicator = 1;
	for(int i=0; i<10; i++){
		if (a[i] > b[i]) break;
		else if (a[i] < b[i]){
			indicator = 0;
			break;
		}
	}
	return indicator;
}

void sub(u64* a, u64* b, u64* result){
	u64 carry = 1;
	for (int i=9; i>=0; i--){
		result[i] = a[i] + (b[i]^0x1fffffff) + carry;
		carry = result[i] >> 29;
		result[i] &= 0x1fffffff;
	}
}

void barrett(u64* a, u64* result){
	for(int i=0; i<10;i++)
		result[i]=0;

	u64 q2[20]={0};
	mult(a+2, mu, q2);
	u64 q3[20]={0}; 
	mult(q2, m, q3);
	u64* r1 = a + 10;
	u64* r2 = q3 + 10;
	u64 r[10]={0};
	sub(r1, r2, r);

	for (int i =0; i<10;i++)
		result[i] = r[i];

	if(geq(r,m)) sub(r,m,result);
	
	for (int i =0; i<10;i++)
		r[i] = result[i];

	if(geq(r,m)) sub(r,m,result);
}

unsigned char gtz(u64* a){
	unsigned char is_positive = 0;
	for (int i =9; i>=0; i--){
		if (a[i] != 0){
			is_positive = 1;
			break;
		}
	}
	return is_positive;
}

void modular_pow(u64* base, u64* exp, u64* result){
	result[9] = 1;
	u64 temp[20] = {0};

	while (gtz(exp)){
		if ((exp[9] & 0x1) == 1){
		    for(int i = 0; i < 20; i++) temp[i] = 0;
			mult(result, base, temp);
			barrett(temp, result);
		}
		rshift1(exp);
		for(int i = 0; i < 20; i++) temp[i] = 0;
		mult(base, base, temp);
		barrett(temp, base);
	}
}
