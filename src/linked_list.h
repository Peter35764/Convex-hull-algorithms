#ifndef CW_ACT_LINKED_LIST_H
#define CW_ACT_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  unsigned int value;
  struct Node* next;
} Node_t;

// !!! В библиотеке нет контроля за поступающими указателями на списки. !!!
//   !!!     Все функции возвращают только положительные значения.    !!!

// =========================================================================
//   === Инициализация списка, добавление нового элемента, очистка памяти ====
// =========================================================================

// Инициализация первой ноды.
// flag == 0 - создать одну ноду со значением равным value.
// flag == 1 - создать список нод в количестве value.
Node_t* list_init(unsigned int value, unsigned int flag);
// Добавить элемент в конец списка.
void append(Node_t** head, unsigned int value);
// Освобождение памяти от всех нод списка.
// Передавать самую первую ноду!
void list_free(Node_t* head);

// ===========================================================
//   === Печать списка, получение значений элементов списка ====
// ===========================================================

// Печать всего списка.
void printList(Node_t* head, const char* name);
// Получить значение последнего элемента.
int get_last(Node_t* head);
// Получить значение предпоследнего элемента.
int get_penultimate(Node_t* head);
// Получение индекса по значению.
int get_index(Node_t* head, unsigned int value);
// Получение значения по индексу.
int get_value(Node_t* head, unsigned int index);
// Получить длину списка.
int get_length(Node_t* head);

// ================================
//   === Смена значения элемента ====
// ================================

// Смена значения элемента по индексу index на значение value.
void change_value(Node_t* head, unsigned int index, unsigned int value);

// ====================================================
//   === Удаление элементов из разных частей списка ====
// ====================================================

// Удалить элемент из начала списка.
int remove_first(Node_t** head);
// Удалить элемент по индексу.
int remove_index(Node_t** head, unsigned int index);
// Удалить элемент по адресу.
int remove_value(Node_t** head, unsigned int value);
// Удалить элемент с конца списка.
int remove_last(Node_t* head);

#endif