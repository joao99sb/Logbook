#include "./linked_list.h"

int main(int argc, char const *argv[])
{

  Node *LL = createLinkedList();
  int A[] = {1, 2, 3, 5, 7};
  int array_size = sizeof(A) / sizeof(A[0]);
  copyArrayElements(LL, &A, 5, array_size);
  displayLinkedListElements(LL);

  int n;
  printf("append a number \n");
  scanf("%d", &n);
  appendLinkedList(LL, n);
  displayLinkedListElements(LL);
  

  return 0;
}
