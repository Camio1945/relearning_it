#include <stdio.h>

struct list_item {
  int value;
  struct list_item *next;
};
typedef struct list_item list_item;

struct list {
  struct list_item *head;
};
typedef struct list list;

#define N 10
static list_item items[N];
static list l;

void remove_entry_from_list(list *l, list_item *target) {
  while (l->head != target) {
    l = &(l->head)->next;
  }
  l->head = target->next;
}

void init_list() {
  items[1].value = 7;
  items[1].next = &items[3];

  items[3].value = 8;
  items[3].next = &items[5];

  items[5].value = 9;
  items[5].next = NULL;

  l.head = &items[1];
}

void print_list() {
  list_item *cur = l.head;
  while (cur) {
    printf("%d ", cur->value);
    cur = cur->next;
  }
  printf("\n");
}

void test_remove_item(int item_index)  {
  init_list();
  print_list();
  remove_entry_from_list(&l, &items[item_index]);
  print_list();
  printf("\n");
}

int main() {
  test_remove_item(1);
  test_remove_item(3);
  test_remove_item(5);
  return 0;
}

