// Function to multiply two integers
void karatsuba(struct number* x, struct number* y,  struct number* result) {
    eqLen(x,y); //Make x & y of equal length
    int len = x->len;    
    // Base case: If both number is of single byte
    if(len==1){
    	makeInt(result, 1);
       	u64 product= (u64)x->num[0] * (u64)y->num[0];
       	u64 carry=0;
       	result->num[0] = product;
       	carry = product >> 32;
        if(carry != 0){
			padZero(result,2);
    		result->num[0]=carry;
  	    }
    	//return result;
	} else {
    	// Split the input numbers into two halves
    	int n2 = len/2;
    	struct number xL, xR, yL, yR, P1, P2, P1P2, xLR, yLR, P3, P4;
		makeInt(&xL,n2);
    	makeInt(&xR,len - n2);
    	makeInt(&yL,n2);
    	makeInt(&yR,len - n2);
    	memcpy(xL.num, x->num, n2 * 4);
    	memcpy(xR.num, x->num + n2, (len - n2)*4);
    	memcpy(yL.num, y->num, n2 * 4);
    	memcpy(yR.num, y->num + n2, (len - n2)*4);
    	
    	// Perform the recursive steps
    	karatsuba(&xL, &yL, &P1);
    	karatsuba(&xR, &yR, &P2);
    	add(&P1,&P2, &P1P2);
    	add(&xL, &xR, &xLR);
    	add(&yL, &yR, &yLR);
    	karatsuba(&xLR, &yLR, &P3);
    	sub(&P3,&P1P2, &P4);

		// Get the final result from P1, P2, P3, P4
    	struct number result1;
		makeInt(&result1,2*len);
		/*while ((P1.num[0] == 0) && (P1.len > 1)){
			memmove(P1.num, P1.num + 1 , P1.len*4 - 4);
			P1.len--;
		}*/
    	memmove(result1.num + 2*n2 - P1.len, P1.num, 4*P1.len);
    	memcpy(result1.num + 2*len - P2.len, P2.num, 4*P2.len);
		int l = P4.len;
		struct number p4;
		makeInt(&p4, l+len-n2);
		memcpy(p4.num, P4.num, 4*P4.len);

    	//struct number result1;
		add(&result1,&p4, result);
		while ((result->num[0] == 0) && (result->len > 1)){
			memmove(result->num, result->num + 1 , result->len*4 - 4);
			result->len--;
		}

    	// Free dynamically allocated memory
		free(xL.num);
		free(xR.num);
		free(yL.num);
		free(yR.num);
		free(P1.num);
		free(P2.num);
		free(P3.num);
		free(P4.num);
		free(p4.num);
		free(P1P2.num);
		free(xLR.num);
		free(yLR.num);
		free(result1.num);
    }
}
