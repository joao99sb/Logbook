#include "./linked_list.h"

void menu()
{
  printf("chose option\n\n");
  printf("1) append a number in Linked list\n");
  printf("2) display Linked list elements\n");
  printf("3) copy array's elements in Linked list \n");
  printf("4) get Linked list length \n");
  printf("5) do a linear search for some element \n");
  printf("6) insert a number \n");
  printf("7) close program \n");
}

int main(int argc, char const *argv[])
{

  Node *LL = createLinkedList();
  int result;
  int n;
  while (1)
  {
    menu();

    int option;
    result = scanf("%d", &option);
    if (result != 1)
    {
      printf(" invalid input!!!\n\n");
      while (getchar() != '\n')
      {
      };
      continue;
    }

    switch (option)
    {
    case 1:

      printf("append a number \n");

      result = scanf("%d", &n);
      if (result != 1)
      {
        printf(" invalid input!!!\n\n");
        while (getchar() != '\n')
        {
        };
        break;
      }

      appendLinkedList(LL, n);
      break;
    case 2:
      displayLinkedListElements(LL);

      break;
    case 3:
      printf("create your array\n");

      Array arr = createArray();

      printf("set the number of elements to copy to Linked List\n");

      result = scanf("%d", &n);

      if (result != 1)
      {
        printf(" invalid input!!!\n\n");
        while (getchar() != '\n')
          ;
        break;
      }

      copyArrayElements(LL, arr.A, n, arr.length);

      destroyArray(&arr);

      break;
    case 4:
      int LL_length = getLinkedListLength(LL);
      printf("Linked List size: %d\n", LL_length);
      break;
    case 5:
      printf("put a number to search ");

      result = scanf("%d", &n);
      if (result != 1)
      {
        printf(" invalid input!!!\n\n");
        while (getchar() != '\n')
          ;
        break;
      }
      int index = linearSearchLinkedList(LL, n);
      if (index != -1)
      {
        printf("this key is in index: %d\n", index);
      }
      else
      {
        printf("there's no key in this linked list\n");
      }
      break;
    case 6:

      printf("index: ");
      result = scanf("%d", &n);

      printf("data: ");
      int k;
      int result2 = scanf("%d", &k);

      if (result != 1 || result2 != 1)
      {
        printf(" invalid input!!!\n\n");
        while (getchar() != '\n')
          ;
        break;
      }

      insertInLinkedList(&LL, k, n);
      displayLinkedListElements(LL);

      break;
    case 7:

      printf("bye bye...");
      return 0;

    default:
      printf("invalid option, try again\n");
      break;
    }
  }

  // int A[] = {1, 2, 3, 5, 7};
  // int array_size = sizeof(A) / sizeof(A[0]);
  // copyArrayElements(LL, &A, 5, array_size);
  // displayLinkedListElements(LL);

  // int n;
  // printf("append a number \n");
  // scanf("%d", &n);
  // appendLinkedList(LL, n);
  // displayLinkedListElements(LL);

  // int LL_length = getLinkedListLength(LL);
  // printf("Linked List size: %d\n", LL_length);

  return 0;
}
