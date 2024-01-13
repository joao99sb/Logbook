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
// create
Array createArray(int size)
{
  Array arr;
  arr.size = size;

  arr.A = (int *)malloc(size * sizeof(int));
  arr.length = 0;

  arr.pin_level = 0;
  arr.current_pin = 0;
  arr.next = 1;

  return arr;
}

// verify if arrays exists
void verifyArray(Array arr)
{
  if (arr.A == NULL)
  {
    abort();
  }
}



// destroi array
void destroyArray(Array *arr)
{
  free(arr->A);
  arr->A = NULL;
}


// add/append
void appendArray(Array *arr, int x)
{

  verifyArray(*arr);

  if (arr->length + 1 > arr->size)
  {
    abort();
  }
  else
  {
    arr->A[arr->length] = x;
    arr->length++;
  }
}

void insertArray(Array *arr, int index, int x)
{
  verifyArray(*arr);

  if (arr->length + 1 > arr->size)
  {
    abort();
  }
  else if (index >= arr->size)
  {
    abort();
  }
  else
  {
    for (int i = arr->length; i > index; i--)
    {
      arr->A[i] = arr->A[i - 1];
    }
    arr->A[index] = x;
    arr->length++;
  }
}


int  delete(Array *arr,int index){
  int x=0;
  int i;
  if(index =>0 && index < arr->length ){
    x = arr->A[index];
    for ( i =index; i< arr->length -1; i++){
      arr->A[i]= arr->A[i+1];
    }
    arr->length--;
    return x;
  }

  return 0;
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
