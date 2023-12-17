#include "linked_list.h"

Node_t *list_init(unsigned int value, unsigned int flag) {
  Node_t *head = (Node_t *)calloc(1, sizeof(Node_t));

  if (!head) {
    perror("ERROR in list_init()");
    exit(1);
  }

  if (flag == 0) {
    head->value = value;
    head->next = NULL;
  } else {
    head->value = 0;
    for (unsigned int i = 1; i < value; i++) {
      append(&head, i);
    }
  }
  return head;
}

void append(Node_t **head, unsigned int value) {
  Node_t *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = (Node_t *)malloc(sizeof(Node_t));
  if (!current->next) {
    perror("ERROR in append()");
    exit(1);
  }

  current->next->value = value;
  current->next->next = NULL;
}

void list_free(Node_t *head) {
  Node_t *tmp = head;
  Node_t *tmp2 = NULL;

  while (1) {
    if (tmp->next == NULL) {
      free(tmp);
      break;
    } else {
      tmp2 = tmp->next;
      free(tmp);
      tmp = tmp2;
    }
  }
}

void printList(Node_t *head, const char *name) {
  printf("\n=== %s ===\n", name);
  if (head == NULL) {
    printf(" ```empty```\n");
  } else {
    int index = 0;
    Node_t *current = head;
    while (current != NULL) {
      printf("%i: %d\n", index++, current->value);
      current = current->next;
    }
  }
  printf("=== list ===\n");
}

int get_last(Node_t *head) {
  while (head->next != NULL) {
    head = head->next;
  }
  return head->value;
}

int get_penultimate(Node_t *head) {
  while (head->next->next != NULL) {
    head = head->next;
  }
  return head->value;
}

int get_index(Node_t *head, unsigned int value) {
  while (value != head->value) {
    if (head->next == NULL) {
      fprintf(stderr, "ERROR in get_value()\n  No such value.\n");
      exit(1);
    }
    head = head->next;
  }
  return head->value;
}

int get_value(Node_t *head, unsigned int index) {
  for (unsigned int i = 0; i < index; i++) {
    if (head->next == NULL) {
      fprintf(stderr, "ERROR in get_value()\n");
      exit(1);
    }
    head = head->next;
  } /*        tmp           i-1
              head          i
              head->next    i+1        */
  return head->value;
}

int get_length(Node_t *head) {
  int ans = 0;
  while (head != NULL) {
    ans++;
    head = head->next;
  }
  return ans;
}

void change_value(Node_t *head, unsigned int index, unsigned int value) {
  for (unsigned int i = 0; i < index; i++) {
    if (head->next == NULL) {
      fprintf(stderr, "ERROR in change_value()\n");
      exit(1);
    }
    head = head->next;
  }
  head->value = value;
}

int remove_first(Node_t **head) {
  int ans = (*head)->value;
  Node_t *tmp = *head;
  *head = (*head)->next;
  free(tmp);

  return ans;
}

int remove_index(Node_t **head, unsigned int index) {
  int ans;

  if (index == 0) {
    ans = remove_first(head);
  } else {
    Node_t *tmp = *head, *tmp2;

    for (unsigned int i = 0; i < index; i++) {
      if (tmp->next == NULL) {
        fprintf(stderr, "ERROR in remove_index()\n");
        exit(1);
      }
      tmp2 = tmp;
      tmp = tmp->next;
    } /*        tmp2        i-1
                tmp         i
                tmp->next   i+1        */
    tmp2->next = tmp->next;
    ans = tmp->value;
    free(tmp);
  }
  return ans;
}

int remove_value(Node_t **head, unsigned int value) {
  int ans;

  if ((*head)->value == value) {
    ans = remove_first(head);
  } else {
    Node_t *tmp = *head, *tmp2;

    while (value != tmp->value && tmp->next != NULL) {
      tmp2 = tmp;
      tmp = tmp->next;
    } /*        tmp2        i-1
                tmp         i
                tmp->next   i+1        */
    tmp2->next = tmp->next;
    ans = tmp->value;
    free(tmp);
  }
  return ans;
}

int remove_last(Node_t *head) {
  int ans;

  if (head->next == NULL) {
    ans = remove_first(&head);
  } else {
    while (head->next->next != NULL) {
      head = head->next;
    }

    ans = head->next->value;
    free(head->next);
    head->next = NULL;
  }
  return ans;
}