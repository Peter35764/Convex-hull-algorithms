#include "main.h"

#include "linked_list.h"

// Построение минимальной выпуклой оболочки на плоскости.
// Алгоритм Джарвиса.
// Алгоритм Грэхема.
// Рекурсивный алгоритм.

Point_t lower_left = {FIELD_SIZE + 1, FIELD_SIZE + 1, -1};

/*
  Point_t* stack = graham_scan(array, Point_t_AMOUNT);
  printArray(array, Point_t_AMOUNT, "after");
  printStack(stack, Point_t_AMOUNT, "stack");
  arr_free(array, Point_t_AMOUNT);
*/

int main() {
  srand(time(NULL));

  Point_t** array = arr_init(POINT_AMOUNT);

  // array[0]->x = 1;
  // array[0]->y = 1;
  // array[1]->x = 5;
  // array[1]->y = 1;
  // array[2]->x = 1;
  // array[2]->y = 5;
  // array[3]->x = 2;
  // array[3]->y = 2;

  array[0]->x = 6;
  array[0]->y = 0;
  array[1]->x = 6;
  array[1]->y = 2;
  array[2]->x = 0;
  array[2]->y = 1;
  array[3]->x = 8;
  array[3]->y = 0;

  printArray(array, POINT_AMOUNT, "before");

  Point_t* curve_arr = jarvis_scan(array, POINT_AMOUNT);

  printStack(curve_arr, POINT_AMOUNT, "after");

  arr_free(array, POINT_AMOUNT);
  free(curve_arr);
  return 0;
}

int rotate(Point_t* a, const Point_t* b, const Point_t* c) {
  int ab_x = b->x - a->x;
  int ab_y = b->y - a->y;
  int bc_x = c->x - b->x;
  int bc_y = c->y - b->y;
  return (ab_x) * (bc_y) - (ab_y) * (bc_x);
}

void printPoint(Point_t p) {
  printf("i: %d  x: %d  y: %d\n", p.index, p.x, p.y);
}

void printStack(Point_t* ptr, const int length, const char* name) {
  printf("\n=== %s, length: %d ===\n", name, length);
  for (int i = 0; i < length; i++) {
    printPoint(ptr[i]);
  }
  printf("============\n");
}

void printArray(Point_t** ptr, const int length, const char* name) {
  printf("\n=== %s, length: %d ===\n", name, length);
  for (int i = 0; i < length; i++) {
    printPoint(*ptr[i]);
  }
  printf("============\n");
}

Point_t** arr_init(int length) {
  Point_t** ptr = malloc(length * sizeof(Point_t*));
  if (!ptr) {
    perror("ERROR in arr_init()");
    exit(1);
  }
  int index = 0;
  for (int i = 0; i < length; i++) {
    ptr[i] = malloc(sizeof(Point_t));
    if (!ptr[i]) {
      perror("ERROR in arr_init()");
      exit(1);
    }
    // Добавить проверку того, что такой точки еще нет

    int x_ = rand() % (FIELD_SIZE + 1);
    int y_ = rand() % (FIELD_SIZE + 1);

    ptr[i]->x = x_;
    ptr[i]->y = y_;
    ptr[i]->index = index++;

    // ptr[i]->x = i;
    // ptr[i]->y = i;
    // ptr[i]->index = i;
  }

  return ptr;
}

void arr_free(Point_t** ptr, int length) {
  for (int i = 0; i < length; i++) {
    free(ptr[i]);
  }
  free(ptr);
}

int Point_t_compare(const void* p1, const void* p2) {
  int ans = 1;
  int result = rotate(&lower_left, p1, p2);
  if (result < 0) {
    ans = -1;
  } else if (result == 0) {
    ans = 0;
  }
  return ans;
}

Point_t* graham_scan(Point_t** arr, const int length) {
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

  qsort(arr, length, sizeof(Point_t*), Point_t_compare);

  // Этап 3. Добавление точек в оболочку (хранение в "стеке").

  Point_t* curve_arr = calloc(POINT_AMOUNT, sizeof(Point_t));
  if (!curve_arr) {
    perror("ERROR in graham_scan()");
    exit(1);
  }
  Point_t* stack_ptr = curve_arr;

  push(stack_ptr, *arr[0]);
  push(stack_ptr, *arr[1]);

  for (int i = 1; i < length; i++) {
    while (rotate(stack_ptr - 1, stack_ptr, arr[i]) <
           0) {  // попробовать поменять знак
      pop(stack_ptr);
    }
    push(stack_ptr, *arr[i]);
  }

  return curve_arr;
}

Point_t* jarvis_scan(Point_t** arr, const int length) {
  // Этап 1. Найти самую левую нижнюю точку.

  // index (P) - номера точек, их позиция в arr
  Node_t* index = list_init(POINT_AMOUNT, 1);
  // curve (H) - вершины оболочки
  Node_t* curve = list_init(0, 0);

  for (int i = 0; i < length; i++) {
    if (arr[i]->x < lower_left.x) {
      lower_left = *arr[i];
    }
  }

  for (int i = 0; i < length; i++) {
    if (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y) {
      lower_left = *arr[i];
    }
  }

  change_value(index, 0, lower_left.index);
  change_value(index, lower_left.index, 0);

  // Этап 2.

  // index (P) - номера точек, их позиция в arr
  // curve (H) - вершины оболочки
  // H = [P[0]]
  // del P[0]
  // P.append(H[0])

  change_value(curve, 0, remove_first(&index));
  append(&index, get_value(curve, 0));

  // printList(curve, "curve");
  // printList(index, "index");

  // while True:
  //   right = 0
  //   for i in range(1,len(P)):
  //     if rotate(A[H[-1]],A[P[right]],A[P[i]])<0:
  //       right = i
  //   if P[right]==H[0]:
  //     break
  //   else:
  //     H.append(P[right])
  //     del P[right]
  // return H
  // index (P) - номера точек, их позиция в arr
  // curve (H) - вершины оболочки

  while (1) {
    // sleep(3);
    int right = 0;
    for (int i = 1; i < get_length(index); i++) {
      if (rotate(arr[get_last(curve)], arr[get_value(index, right)],
                 arr[get_value(index, i)]) < 0) {
        right = i;
      }
    }
    if (index[right].value == curve[0].value) {
      printf("    !!!break!!!    \n");  ////////////////
      break;
    } else {
      append(&curve, index[right].value);
      remove_index(&index, right);
    }
    printList(curve, "curve in while");  ////////////////////////
    printList(index, "index in while");  ////////////////////////
  }

  // Удобный вывод оболочки.

  Point_t* curve_arr = calloc(POINT_AMOUNT, sizeof(Point_t));
  if (!curve_arr) {
    perror("ERROR in jarvis_scan()");
    exit(1);
  }
  Point_t* stack_ptr = curve_arr;

  for (int i = 0; i < POINT_AMOUNT; i++) {
    push(stack_ptr, *arr[curve[i].value]);
  }

  list_free(index);
  list_free(curve);

  return curve_arr;
}
