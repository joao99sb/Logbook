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
  printf("7) clean terminal \n");
  printf("8) delete some element \n");
  printf("9) close program \n");
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
      printf("invalid input!!!\n\n");
      while (getchar() != '\n')
      {
      };
      continue;
    }

    else if (option == 1)
    {
      printf("append a number \n");

      result = scanf("%d", &n);
      if (result != 1)
      {
        printf("invalid input!!!\n\n");
        while (getchar() != '\n')
        {
        };
        break;
      }

      appendLinkedList(LL, n);
    }

    else if (option == 2)
    {
      displayLinkedListElements(LL);
    }

    else if (option == 3)
    {
      printf("create your array\n");

      Array arr = createArray();

      printf("set the number of elements to copy to Linked List\n");

      result = scanf("%d", &n);

      if (result != 1)
      {
        printf("invalid input!!!\n\n");
        while (getchar() != '\n')
          ;
        continue;
      }

      copyArrayElements(LL, arr.A, n, arr.length);

      destroyArray(&arr);
    }

    else if (option == 4)
    {
      int LL_length = getLinkedListLength(LL);
      printf("Linked List size: %d\n", LL_length);
    }

    else if (option == 5)
    {
      printf("put a number to search ");

      result = scanf("%d", &n);
      if (result != 1)
      {
        printf("invalid input!!!\n\n");
        while (getchar() != '\n')
          ;
        continue;
        ;
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
    }

    else if (option == 6)
    {
      printf("index: ");
      result = scanf("%d", &n);

      printf("data: ");
      int k;
      int result2 = scanf("%d", &k);

      if (result != 1 || result2 != 1)
      {
        printf("invalid input!!!\n\n");
        while (getchar() != '\n')
          ;
        continue;
        ;
      }

      insertInLinkedList(&LL, k, n);
      displayLinkedListElements(LL);
    }

    else if (option == 7)
    {
      printf("\033[2J\033[H");
    }

    else if (option == 8)
    {
      printf("index: ");
      result = scanf("%d", &n);

      deleteElementInLinkedList(&LL, n);
    }
    else if (option == 9)
    {
      printf("bye bye...");
      return 0;
    }

    else
    {
      printf("invalid option, try again\n");
      continue;
    }
  }
  return 0;
}
