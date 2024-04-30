/**
 * In my case, my hashtable needs tobe fast in write
 */

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <optional>

namespace hashTable
{
  template <typename T>
  struct Node
  {
    T data;
  };

  template <typename T>
  struct List
  {
    Node<T> **data;
    unsigned int len;
    int count;
  };

  // my table will be a list off pointers to nodes
  // every insert will recreate the list with one more block
  template <typename T>
  List<T> *createList(int cappacity)
  {
    List<T> *list = (List<T> *)malloc(sizeof(List<T> *) * cappacity);

    list->data = (Node<T> **)malloc(sizeof(Node<T> *) * cappacity);
    list->len = cappacity;
    list->count = 0;

    return list;
  }

  inline int createHash(const char *chave, unsigned int limit)
  {
    unsigned long i = 0;

    for (int j = 0; chave[j]; j++)
      i += chave[j];

    return i % limit;
  }

  template <typename T>
  Node<T> *createNode(T data)
  {
    Node<T> *node = (Node<T> *)malloc(sizeof(Node<T>));
    node->data = data;
    return node;
  }

  template <typename T>
  void insertElement(List<T> *list, const char *key, T value)
  {
    // pensar depois

    if (list->count >= list->len)
    {
      printf("error in insert\n");
      exit(1);
    }
    unsigned int id;

    id = createHash(key, list->len);

    Node<T> *new_node = (Node<T> *)malloc(sizeof(Node<T> *));
    new_node->data = value;
    list->data[id] = new_node;

    list->count++;
  }

  template <typename T>
  std::optional<T> getValueInList(List<T> *l, const char *key)
  {

    int id = createHash(key, l->len);

    if (l->data[id] == NULL)
    {
      return std::nullopt;
    }
    Node<T> *node = l->data[id];

    return node->data;
  }

};