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

  Node *new_node = createLinkedList();

  putDataInNode(new_node, data);

  next->next = new_node;
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
