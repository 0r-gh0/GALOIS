// Function to pad zeros to the beginning of the array
void padZero(struct number* integer, int targetLen) {
    int l = integer->len;
    int diff = targetLen - l;
    u32* temp = (u32*)malloc(targetLen * 4);
    memcpy(temp + diff, integer->num, l*4);// Shift the original string to the right by num_zeroes positions
    memset(temp, 0 , diff*4);// Fill the first num_zeroes characters with 0
    free(integer->num);
    integer->num = temp;
    integer->len = targetLen;
}           

// Function to make two arrays of equal length
void eqLen(struct number* num1, struct number* num2) {
    int l1=num1->len;
    int l2=num2->len;
    if (l1 < l2)
        padZero(num1,l2);
    else if (l1 > l2)
        padZero(num2,l1);
}   

// Function to add two integers
void add(struct number* num1, struct number* num2, struct number* result) {
    u64 carry = 0;
    eqLen(num1,num2);
    int l = num1->len;
    makeInt(result,l);
    for (int i = l-1; i >= 0; --i) {
        u64 sum = (u64)num1->num[i] + (u64)num2->num[i] + carry;
        result->num[i] = sum;
        carry = sum >> 32;
    }   
    if(carry != 0){
    	padZero(result,l+1);
    	result->num[0]=1;
    }
}

// Function to subtract two intgers, using 2's complement
// N.B.: For this particular interest we would always have positive resultant
void sub(struct number* num1, struct number* num2, struct number* result) {
	eqLen(num1,num2);
	int l = num1->len;
	struct number complement, just1, result1;
    makeInt(&complement, l);
	for (int i=0; i<l;++i)
		complement.num[i]= num2->num[i] ^ 0xffffffff;	// 1's complement of the subtrahend
    makeInt(&just1,1);
	just1.num[0]=1;
	add(&complement,&just1,&result1);		// 2's complement of the subtrahend
	add(num1,&result1, result); //struct number result;
	//Ignore the carry bit(if there is any)
    if (result->len > l){
        memmove(result->num, result->num + 1 , l*4);
        result->len--;
    }
    while (result->num[0] == 0 && result->len > 1){
        memmove(result->num, result->num + 1 , result->len*4 - 4);
        result->len--;
    }

	//Free allocated memory
	free(just1.num);
	free(complement.num);
    free(result1.num);
}