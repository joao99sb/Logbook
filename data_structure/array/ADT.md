# Array ADT(Abstract Data Type)

Abstract Data Type means representation of data and operations of data

Data:

1. Array space
2. size
3. length

Operations:

1. Display
2. Add(x)/Append(x)
3. insert(index,x)
4. delete(index)
5. search(x)
6. get(index)
7. set(index,x)
8. max()/min()
9. reverse()
10. shift()/rotate()

exemple

```C
#include <stdio.h>
#include <stdlib.h>

struct Array{
  int *A;
  int size;
  int length;
};

void Display(struct Array arr){
  int i;
  printf("Elements are\n");
  for(i=0;i<arr.length;i++){
    printf("%d ",arr.A[i]);
  }
}

int main(int argc, char *argv[]) {

  struct Array arr;
  int n,i;
  printf("Enter size of an array ");
  scanf("%d",&arr.size);

  arr.A = (int*)malloc(arr.size*sizeof(int));
  arr.length = 0;

  printf("Enter number of numbers ");

  scanf("%d",&n);

  printf("Enter all elements\n");
  for (i=0;i<n;i++){
    scanf("%d",&arr.A[i]);
  }
  arr.length = n;

  Display(arr);

  return 0;
}

```
