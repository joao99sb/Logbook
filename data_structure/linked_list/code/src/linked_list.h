#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct Node
{
  int data;
  struct Node *next;
  int has_data;
} Node;

Node *createLinkedList()
{

  Node *first = (Node *)malloc(sizeof(Node));
  first->next = NULL;
  first->has_data = 0;
  return first;
}

void putDataInNode(Node *node, int data)
{
  node->data = data;
  node->has_data = 1;
}

void copyArrayElements(Node *LL, int A[], int n, int array_size)
{

  Node *root, *node, *next;

  root = LL;
  node = root;

  if (n > array_size)
  {
    printf("Erro: O valor de n (%d) não pode ser maior que o tamanho do array A (%d).\n", n, array_size);
    return;
  }
  for (int i = 0; i < n; i++)
  {
    if (!node->has_data)
    {
      putDataInNode(node, A[i]);
    }

    if (node->next == NULL)
    {
      next = createLinkedList();
      node->next = next;
    }
    node = next;
  }
}

void appendLinkedList(Node *LL, int data)
{
  Node *next = LL;

  while (next->next != NULL)
  {
    next = next->next;
  }
  if (!next->has_data)
  {
    putDataInNode(next, data);

    return;
  }

  Node *new_node = createLinkedList();

  putDataInNode(new_node, data);

  next->next = new_node;
}

int getLinkedListLength(Node *LL)
{
  int size = 0;
  Node *node = LL;
  while (1)
  {

    if (node->has_data)
    {
      size++;
    }
    if (node->next == NULL)
      break;
    else
    {
      node = node->next;
    }
  }
  return size;
}

int linearSearchLinkedList(Node *LL, int key)
{
  Node *node = LL;
  int index = 0;
  while (node->next != NULL)
  {
    if (node->has_data && node->data == key)
    {
      return index;
    }
    node = node->next;
    index++;
  }
  return -1;
}

void insertInLinkedList(Node **LL, int data, int index)
{

  if (index < 0 || index > getLinkedListLength(*LL))
  {
    printf("invalid format!!\n\n");
    return;
  }
  Node *temp, *node;

  node = *LL;
  Node *new_node = createLinkedList();
  putDataInNode(new_node, data);

  if (index == 0)
  {
    new_node->next = *LL;
    *LL = new_node;
  }
  else
  {
    for (int i = 0; i < index - 1; i++)
    {
      node = node->next;
    }
    new_node->next = node->next;
    node->next = new_node;
  }
}
void deleteElementInLinkedList(Node **LL, int index)
{
  if (index < 0 || index > getLinkedListLength(*LL) - 1)
  {
    printf("invalid format!!\n\n");
    return;
  }

  Node *temp = NULL, *node;
  node = *LL;
  if (index == 0)
  {
    *LL = (*LL)->next;

    free(node);
  }
  else
  {

    for (int i = 0; i < index - 1; i++)
    {

      node = node->next;
    }
    temp = node->next;
    node->next = temp->next;
    free(temp);
  }
}

void displayLinkedListElements(Node *LL)
{

  Node *node;
  printf("[");
  node = LL;
  while (1)
  {

    if (node->has_data)
    {
      printf(" %d ", node->data);
    }
    if (node->next == NULL)
      break;
    else
    {
      node = node->next;
    }
  }
  printf("]\n");
}

typedef struct Array
{
  int *A;
  int size;
  int length;
} Array;

Array createArray(void)
{
  Array arr;

  printf("Enter size of an Array\n");

  scanf("%d", &arr.size);

  arr.A = (int *)malloc(arr.size * sizeof(int));
  arr.length = 0;

  printf("Enter number of elements in an Array\n");
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
void destroyArray(Array *arr)
{
  free(arr->A);
  arr->A = NULL;
}
