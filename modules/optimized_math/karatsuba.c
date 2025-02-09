#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "karatsuba.h"

#define MAX 256  //number of elements in array
#define limt 4  //when number of digits afe less than 'cutoff' program uses usual multipliaction method


void multi(int *a, int *b, int *ret, int d) 
{
    int i;
    int *af = &a[0]; // 1st half of a
    int *as = &a[d/2]; // 2nd half of a
    int *bf = &b[0]; // 1st half of b
    int *bs = &b[d/2]; // 2nd half of b
    int *asum = &ret[d * 5]; // sum of a's halves
    int *bsum = &ret[d * 5 + d/2]; // sum of b's halves
    int *x1 = &ret[d * 0]; // af*bf's location
    int *x2 = &ret[d * 1]; // as*bs's location
    int *x3 = &ret[d * 2]; // asum*bsum's location
    
    if(d <= limt) 
	{
        SimpleM(a, b, ret, d);
        return;
    }

    // compute asum and bsum
    for(i = 0; i < d / 2; i++) 
	{
        asum[i] = as[i] + af[i];
        bsum[i] = bs[i] + bf[i];
    }

    // do recursive calls (I have to be careful about the order,
    // since the scratch space for the recursion on x1 includes
    // the space used for x2 and x3)
    multi(af, bf, x1, d/2);
    multi(as, bs, x2, d/2);
    multi(asum, bsum, x3, d/2);

    // combine recursive steps
    for(i = 0; i < d; i++) 
        x3[i] = x3[i] - x1[i] - x2[i];
    for(i = 0; i < d; i++)
        ret[i + d/2] += x3[i];
}

void carry(int *a, int d);
void SimpleM(int *a, int *b, int *ret, int d);
void getNum(int *a, int *d_a, char x[]);
char *stringify_result(int *a, int d);

void SimpleM(int *a, int *b, int *ret, int d)   //function multiplying number in usual way
{
    int i, j;	

    for(i = 0; i < 2 * d; i++) 
	    ret[i] = 0;
	
    for(i = 0; i < d; i++) 
	{
        for(j = 0; j < d; j++) 
            ret[i + j] += a[i] * b[j];
    }
}

void carry(int *a, int d)
{
    int c;
    int i;

    c = 0;
    for(i = 0; i < d; i++) 
	{
        a[i] += c;

        if(a[i] < 0) 
            c = -(-(a[i] + 1) / 10 + 1);
		else 
            c = a[i] / 10;
            
        a[i] -= c * 10;
    }
   				//condition when number of digits are greater than max_digit.
}

void getNum(int *a, int *size_of_a, char x[])
{				
    int c;
    int i;
    *size_of_a = 0;
    for (size_t j = 0; j < strlen(x); j++){
        c = x[j];
        a[*size_of_a] = c - '0';
        ++(*size_of_a);
    }
    // reverse the number so that the 1's digit is first
    for(i = 0; i * 2 < *size_of_a - 1; i++)
        c = a[i], a[i] = a[*size_of_a - i - 1], a[*size_of_a - i - 1] = c;
}

char *stringify_result(int *a, int d)
{
    int i;
    int n = 0; 
    int required_size = 0;
    
    for(i = d - 1; i >= 0; i--) 
        if(a[i] != 0 || i == 0) break;
    
    // Calculate the required size
    for (int j = i; j >= 0; j--) {
        int temp = a[j];
        if (temp == 0) {
            required_size += 1; // For the '0' character
        } else {
            required_size += snprintf(NULL, 0, "%d", a[j]); // Use snprintf to get the required length
        }
    }
    required_size++; // Add 1 for the null terminator

    char *result = (char *)malloc(required_size * sizeof(char));
    if (result == NULL) {
        perror("malloc failed"); // Handle malloc failure
        exit(1);
    }

    for (; i >= 0; i--) 
        n += sprintf(result + n, "%d", a[i]);
    result[required_size - 1] = '\0'; // Ensure null termination (important!)
        
    return result;
}


char *multiply(char* x, char* y){
    int a[MAX]; // first multiplicand
    int b[MAX]; // second multiplicand
    int res[3 * MAX]; // result
    int size_of_a; // length of a
    int size_of_b; // length of b
    int d; // maximum length
    int i; // counter

	getNum(a, &size_of_a, x);
    getNum(b, &size_of_b, y);

    // let d be the smallest power of 2 greater than size_of_a and size_of_b,
    // and zero out the rest of a and b.
    i = (size_of_a > size_of_b) ? size_of_a : size_of_b;
    for(d = 1; d < i; d++);  
    for(i = size_of_a; i < d; i++) a[i] = 0;
    for(i = size_of_b; i < d; i++) b[i] = 0;

    {
        multi(a, b, res, d); // compute product w/o regard to carry
        carry(res, 2 * d); // now do any carrying when simple multiplication is used
    }

    return stringify_result(res, 2 * d);
}
