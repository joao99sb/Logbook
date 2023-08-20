## Introduction

  Arrays are a list of elements of the same type.

# Array in compilers

1. simple array
int A[5] = {2,3,4,5,6}; \\ int has 8 Bytes

   ___  ___  ___  ___  ___  
 A|_2_||_3_||_4_||_5_||_6_| 
    ^    ^    ^    ^    ^   
   200  208  216  224  232

  addr(A[3]) = 200 + 8*3 = 224 // addr = address;

  addr(A[i]) = L0 +  i*w // L0 = address of first element
                        // i = index
                        // w = size of data type


2. matrix 

int A[3][2];