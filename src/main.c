#include "main.h"

#include <stdio.h>
#include <stdlib.h>

// Построение минимальной выпуклой оболочки на плоскости.
// Алгоритм Джарвиса.
// Алгоритм Грэхема.
// Рекурсивный алгоритм.

Point lower_left = {FIELD_SIZE + 1, FIELD_SIZE + 1};

int main() {
  srand(time(NULL));

  Point** arr = arr_init(10);

  /*
    printPoint(*arr[0]);
    printPoint(*arr[1]);
    printPoint(*arr[2]);
    printf("Поворот: %d\n", rotate(*arr[0], *arr[1], *arr[2]));
  */

  graham_scan(arr, 10);

  arr_free(arr, 10);
  return 0;
}

int rotate(Point* a, const Point* b, const Point* c) {
  int ab_x = b->x - a->x;
  int ab_y = b->y - a->y;
  int bc_x = c->x - b->x;
  int bc_y = c->y - b->y;
  return (ab_x) * (bc_y) - (ab_y) * (bc_x);
}

void printPoint(Point p) { printf("x: %d   y: %d\n", p.x, p.y); }

Point** arr_init(int length) {
  Point** ptr = malloc(length * sizeof(Point));
  if (!ptr) {
    perror("ERROR in arr_init()");
    exit(1);
  }

  for (int i = 0; i < length; i++) {
    ptr[i] = malloc(sizeof(Point));
    if (!ptr[i]) {
      perror("ERROR in arr_init()");
      exit(1);
    }
    // Добавить проверку того, что такой точки еще нет
    int x_ = rand() % (FIELD_SIZE + 1);
    int y_ = rand() % (FIELD_SIZE + 1);
    ptr[i]->x = x_;
    ptr[i]->y = y_;
  }

  return ptr;
}

void arr_free(Point** ptr, int length) {
  if (ptr == NULL) {
    perror("ERROR in arr_free()");
    exit(1);
  }
  for (int i = 0; i < length; i++) {
    free(ptr[i]);
  }
  free(ptr);
}

/*
comparison function which returns ​a negative integer value if the first
argument is less than the second, a positive integer value if the first argument
is greater than the second and zero if the arguments are equivalent. The
signature of the comparison function should be equivalent to the following: int
cmp(const void *a, const void *b); The function must not modify the objects
passed to it and must return consistent results when called for the same
objects, regardless of their positions in the array.
*/
// rotate(A[P[0]],A[P[j-1]],A[P[j]])<0
int Point_compare(const void* p1, const void* p2) {
  int ans = 1;
  int result = rotate(&lower_left, p1, p2);
  if (result < 0) {
    ans = -1;
  } else if (result == 0) {
    ans = 0;
  }
  return ans;
}

void graham_scan(Point** arr, const int length) {
  /* В статье написано, что надо использовать косвенную адресацию в виде
  отдельного массива, но такой подход требует написания собственной сортировки.
  Попробуем без этого. int index[length]; for (int i = 0; i < length; i++) {
    index[i] = i;
  }
  */

  // Этап 1. Найти самую левую нижнюю точку.

  for (int i = 0; i < length; i++) {
    if (arr[i]->x < lower_left.x) {
      lower_left = *arr[i];
      *arr[i] = *arr[0];
      *arr[0] = lower_left;
    }
  }

  for (int i = 0; i < length; i++) {
    if (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y) {
      lower_left = *arr[i];
      *arr[i] = *arr[0];
      *arr[0] = lower_left;
    }
  }

  // Этап 2. Сортировка точек.

  qsort(arr, length, sizeof(Point), Point_compare);

  for (int i = 0; i < length; i++) {
    printf("%i: array: %i  %i\n", i, arr[i]->x, arr[i]->y);
  }
}
