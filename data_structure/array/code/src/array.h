#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct Array
{
  int *A;
  int size;
  int length;
} Array;

void verifyArray(Array arr)
{
  if (arr.A == NULL)
  {
    fprintf(stderr, "Error: Array doesnt exist\n");
    abort();
  }
}

// create
Array createArray(void)
{
  Array arr;
  printf("Enter size of an Array\n");

  scanf("%d", &arr.size);
  arr.A = (int *)malloc(arr.size * sizeof(int));
  arr.length = 0;

  printf("Enter number of numbers in an Array\n");
  int n;
  scanf("%d", &n);

  while (n > arr.size)
  {
    printf("The length cannot exceed the size, please try again");
    printf("Enter number of numbers in an Array\n");
    scanf("%d", &n);
  }

  printf("Enter all Elements\n");
  for (int i = 0; i < n; i++)
  {
    scanf("%d", &arr.A[i]);
  }
  arr.length = n;

  return arr;
}

// display
void displayArrayElements(Array *arr)
{
  verifyArray(*arr);
  int i;
  printf("Elements are: ");
  printf("[ ");
  for (i = 0; i < arr->length; i++)
  {
    if (i == arr->length - 1)
    {
      printf("%d ", arr->A[i]);
    }
    else
    {

      printf("%d, ", arr->A[i]);
    }
  }
  printf("]");
  printf("\n");
}

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
    printf("Error: no memory in array\n");
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
    printf("Error: no memory in array\n");
    abort();
  }
  else if (index >= arr->size)
  {
    printf("Error: invalid array index\n");
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

int deleteArrayElement(Array *arr, int index)
{
  verifyArray(*arr);
  if (index < 0 || index > arr->length)
  {
    printf("Error: Invalid index\n");
    abort();
  }

  int x = 0;
  int i;

  x = arr->A[index];
  for (i = index; i < arr->length - 1; i++)
  {
    arr->A[i] = arr->A[i + 1];
  }
  arr->length--;
  return x;
}