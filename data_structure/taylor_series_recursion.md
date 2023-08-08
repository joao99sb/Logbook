## Implementation of Taylor Series

# definitions

  exp(x) = e^x

  Fn(x,n) = some arbitrary function with an N as params, like:{

    cos(x)^N,
    sin(1/N),
    x/N,
    (e^x)/N,
    ...
  }

  Fn' = function by param

  sum(Fn',a,b) = Fn(x,a) + Fn(x,a+1) + ... Fn(x,b)

  inf = infinit

# Idea

  Fn(x,n) = (x/n!)^n;

  exp(x) = sum(Fn',0,inf);
  // exp(x) = 1 + x + x²/2! + x³/3! + ...

# Implementation

1. # idea:
  exp(x,n) = 1 + x/1 + x²/2! + x³/3! + ... +(x^n)/n!

  exp(x,3) => exp(x,2) + (x*x*x)/3*2*1
  exp(x,2) => exp(x,1) + (x*x)/2*1
  exp(x,1) => exp(x,0) + x/1
  exp(x,0) => 1

```C
#include <stdio.h>
#include <math.h>

float my_exp(int x, int N) {
    // p is for power
    // f is for factorial
    static int p = 1, f = 1;
    float y;

    if (N == 0) return 1.0f;

    else {
        y = my_exp(x, N - 1);
        p = p * x;
        f = f * N;
        return y + p / (float)f;
    }
}

int main(int argc, char *argv[]) {
    float e2 = my_exp(2, 9);
    printf("%f\n", e2);
    return 0;
}

```

2. # idea:
  exp(x,n) = 1 + x/1 + x²/2! + x³/3! + ... +(x^n)/n!
  
  exp(x,3) => exp(x,2) + pow(x,3)/fact(3)
  exp(x,2) => exp(x,1) + pow(x,2)/fact(2)
  exp(x,1) => exp(x,0) + pow(x,1)/fact(1)
  exp(x,0) => 1
  