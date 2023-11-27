## Introduction

Arrays are a list of elements of the same type.

# Array in compilers

1. simple array
   int A[5] = {2,3,4,5,6}; \\ int has 2 Bytes

   ***

   A|_2_| |_3_| |_4_| |_5_| |_6_|
   ^ ^ ^ ^ ^  
    200/1 202/3 204/5 206/7 208/9

addr(A[3]) = 200 + 2*3 = 206 // addr = address;

addr(A[i]) = L0 + i*w // L0 = address of first element
// i = index
// w = size of data type

2. matrix

int A[3][4];
      m  n

    1   2   3   4

0| a00 a01 a02 a03 |
1| a10 a11 a12 a13 |
2| a20 a21 a22 a23 |

    each element has 2 bytes of length
     _______________________________________________

A|a00|a01|a02|a03|a10|a11|a12|a13|a20|a21|a22|a23|
/\ | | /\
 200/1 | | 222/23
Y0W0 | Y0W1 | Y0W2

addr(A[1][2]) = 200 + [4+2]*2 = 212

addr(A[2][3]) = 200 + [2*4+3]*2 = 222

addr(A[i][j]) = L0 + [i*n+j]*w // L0 = address of first element
// w = size of data type

3. Matrix multi dimensional

int A[d1][d2][d3][d4];

addr(A[i1][i2][i3][i4]) = L0 + [i1*d2*d3*d4 + i2*d3*d4 + i3*d4 + i4]\*w

generalization:
addr(A[i1]...[in]) = L0 + sum( ip*pi( dk,k,from=p+1,to=n),p,from=1,to=n )*w

obs: sum(a,b,from=c,to=d) is summation notation:
a = function that will be iterated
b = var of iteration
c = lower bound
d = upper bound

obs2: function pi(a,b,from=c,to=d) is a product notation:
a = function that will be iterated
b = var of iteration
c = lower bound
d = upper bound

exemple of 4 dimention:
addr(A[i1][i2][i3][i4]) = L0 + [i1*d2*d3*d4 + i2*d3*d4 + i3*d4 + i4]*w
/\ /\ /\ /\
 3 2 1 0 multiplications

4D -> 3 + 2 + 1
5D -> 4 + 3 + 2 +1
6D -> 5 + 4 + 3 + 2 +1
nD -> n-1 + n-2 + ... + 2 + 1 = n(n-1)/2 => O(n²)

factoring:

i4 + d4*[i3 + d3*[i2 + d2*i1]] => 3 multiplications

5D = 4 multiplications
6D = 5 multiplications
nD = (n-1) multiplications => O(n)

this factoring is call Horner's Rule


