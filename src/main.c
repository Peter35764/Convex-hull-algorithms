#include "main.h"

#include "linked_list.h"

// Построение минимальной выпуклой оболочки на плоскости.
// Алгоритм Джарвиса.
// Алгоритм Грэхема.
// Рекурсивный алгоритм.

// Размеры квадрата первой четверти плоскости, в котором могут быть точки.
// (Object-like Macros)
#define FIELD_SIZE 10
// Количество сгенерированных точек.
#define POINT_AMOUNT 10

Point_t lower_left = {FIELD_SIZE + 1, FIELD_SIZE + 1, -1};

int main(void) {
/*
  Point_t* stack = graham_scan(array, Point_t_AMOUNT);
  printArray(array, Point_t_AMOUNT, "after");
  printStack(stack, Point_t_AMOUNT, "stack");
  arr_free(array, Point_t_AMOUNT);
*/

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

  // array[0]->x = 6;
  // array[0]->y = 0;
  // array[1]->x = 6;
  // array[1]->y = 2;
  // array[2]->x = 0;
  // array[2]->y = 1;
  // array[3]->x = 8;
  // array[3]->y = 0;

  printArray(array, POINT_AMOUNT, "before");

  Point_t* curve_arr = jarvis_algorithm(array, POINT_AMOUNT);

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

int printPoint(Point_t p) {
  int status = 0;
  if (!(p.index == p.x && p.x == p.y && p.y == p.index && p.index == -1)) {
    printf("i: %d  x: %d  y: %d\n", p.index, p.x, p.y);
    status++;
  }
  return status;
}

void printStack(Point_t* ptr, const int length, const char* name) {
  int real_length = 0;

  printf("\n=== Stack %s ===\n", name);
  for (int i = 0; i < length; i++) {
    real_length += printPoint(ptr[i]);
  }
  printf("=== length: %i ===\n", real_length);
}

void printArray(Point_t** ptr, const int length, const char* name) {
  int real_length = 0;

  printf("\n=== Array %s ===\n", name);
  for (int i = 0; i < length; i++) {
    real_length += printPoint(*ptr[i]);
  }
  printf("=== length: %i ===\n", length);
  if (real_length != length) {
    printf("declared length: %i\n", length);
    printf("real length: %i\n", real_length);
    printf("===============\n");
  }
}

Point_t** arr_init(int length) {
  Point_t** ptr = malloc(length * sizeof(Point_t*));
  if (!ptr) {
    perror("ERROR in arr_init()");
    exit(1);
  }

  for (int i = 0; i < length; i++) {
    ptr[i] = malloc(sizeof(Point_t));
    if (!ptr[i]) {
      perror("ERROR in arr_init()");
      exit(1);
    }

    Point_t tmp = {rand() % (FIELD_SIZE + 1), rand() % (FIELD_SIZE + 1), -121};
    if (arr_findPoint(ptr, tmp, i)) {
      Point_t tmp __attribute__((unused)) = {rand() % (FIELD_SIZE + 1),
                                             rand() % (FIELD_SIZE + 1), -121};
    }

    ptr[i]->x = tmp.x;
    ptr[i]->y = tmp.y;
    ptr[i]->index = i;
  }
  return ptr;
}

int arr_findPoint(Point_t** arr, Point_t point, int length) {
  int find = 0;
  for (int i = 0; i < length; i++) {
    if (arr[i]->x == point.x && arr[i]->y == point.y) {
      find = 1;
      break;
    }
  }
  return find;
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

Point_t* graham_algorithm(Point_t** arr, const int length) {
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

Point_t* jarvis_algorithm(Point_t** arr, const int length) {
  
  // Этап 1. Найти самую левую нижнюю точку.

  // index - номера точек, их позиция в arr
  Node_t* index = list_init(POINT_AMOUNT, 1);
  // curve - вершины оболочки
  Node_t* curve = list_init(1000000, 0);

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

  change_value(curve, 0, remove_first(&index));
  append(&index, get_value(curve, 0));

  while (1) {
    int right = 0;
    for (int i = 1; i < get_length(index); i++) {
      if (rotate(arr[get_last(curve)], arr[get_value(index, right)],
                 arr[get_value(index, i)]) < 0) {
        right = i;
      }
    }
    if (get_value(index, right) == get_value(curve, 0)) {
      break;
    } else {
      append(&curve, get_value(index, right));
      remove_index(&index, right);
    }
  }

  Point_t* curve_arr = calloc(POINT_AMOUNT, sizeof(Point_t));
  if (!curve_arr) {
    perror("ERROR in jarvis_scan()");
    exit(1);
  }
  Point_t* stack_ptr = curve_arr;

  int curve_length = get_length(curve);
  Point_t zaglushka = {-1, -1, -1};

  for (int i = 0; i < curve_length; i++) {
    push(stack_ptr, *arr[remove_first(&curve)]);  // curve frees here !
  }
  for (int i = curve_length; i < POINT_AMOUNT; i++) {
    push(stack_ptr, zaglushka);
  }

  list_free(index);

  return curve_arr;
}
