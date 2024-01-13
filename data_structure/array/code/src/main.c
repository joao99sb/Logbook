#include "./array.h"

int main(int argc, char const *argv[])
{
  Array arr = createArray();

  displayArrayElements(&arr);
  // destroyArray(&arr);
  // displayArrayElements(&arr);

  int n;
  printf("append a number \n");
  scanf("%d", &n);
  appendArray(&arr, n);

  displayArrayElements(&arr);

  printf("Enter a index and number do insert into the array\n");
  int i;
  printf("index: ");
  scanf("%d", &i);
  printf("numebr: ");
  scanf("%d", &n);
  insertArray(&arr, i, n);

  displayArrayElements(&arr);

  printf("Enter a index delete this array's element \n");

  printf("index: ");
  scanf("%d", &i);
  int elementDeleted = deleteArrayElement(&arr, 2);
  printf("element deleted:");
  printf("%d\n", elementDeleted);

  displayArrayElements(&arr);

  return 0;
}
