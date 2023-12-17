#include "main.h"

#include "linked_list.h"

// Построение минимальной выпуклой оболочки на плоскости.
// Алгоритм Джарвиса.
// Алгоритм Грэхема.
// Алгоритм Чена.

// Размеры квадрата первой четверти плоскости, в котором могут быть точки.
// (Object-like Macros)
#define FIELD_SIZE 50
// Количество сгенерированных точек.
#define POINT_AMOUNT 8

Point_t lower_left = {FIELD_SIZE + 1, FIELD_SIZE + 1, -1};

int main(void) {
  srand(time(NULL));

  Point_t** array = arr_init(POINT_AMOUNT);

  printArray(array, POINT_AMOUNT, "Array of points");

  Point_t* curve = graham_algorithm(array);

  printStack(curve, POINT_AMOUNT, "curve");
  output(array, curve);

  arr_free(array, POINT_AMOUNT);
  free(curve);

  return 0;
}

void output(Point_t** arr, Point_t* curve) {
  const char* filename = "output.txt";
  FILE* file = fopen(filename, "w");

  for (int i = 0; i <= FIELD_SIZE + 2; i++) {
    fprintf(file, "-");
  }
  fprintf(file, "\n");

  for (int i = 0; i <= FIELD_SIZE; i++) {
    fprintf(file, "|");
    for (int j = 0; j <= FIELD_SIZE; j++) {
      Point_t tmp = {i, j, -100};
      if (stk_findPoint(curve, tmp, POINT_AMOUNT)) {
        fputc('@', file);
      } else if (arr_findPoint(arr, tmp, POINT_AMOUNT)) {
        fputc('*', file);
      } else {
        fputc(' ', file);
      }
    }
    fprintf(file, "|\n");
  }
  for (int i = 0; i <= FIELD_SIZE + 2; i++) {
    fprintf(file, "-");
  }
  fprintf(file, "\n");

  fclose(file);
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

  printf("\n=== %s ===\n", name);
  for (int i = 0; i < length; i++) {
    real_length += printPoint(ptr[i]);
  }
  printf("=== stack length: %i ===\n", real_length);
}

void printArray(Point_t** ptr, const int length, const char* name) {
  int real_length = 0;

  printf("\n=== %s ===\n", name);
  for (int i = 0; i < length; i++) {
    real_length += printPoint(*ptr[i]);
  }
  printf("=== array length: %i ===\n", length);
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

Point_t** arr_copy(Point_t** arr, int exclude_index) {
  Point_t** ptr = malloc((POINT_AMOUNT - 1) * sizeof(Point_t*));
  if (!ptr) {
    perror("ERROR in arr_copy()");
    exit(1);
  }

  int i = 0;
  for (; i < POINT_AMOUNT - 1; i++) {
    if (arr[i]->index == exclude_index) {
      break;
    }

    ptr[i] = malloc(sizeof(Point_t));
    if (!ptr[i]) {
      perror("ERROR in arr_copy()");
      exit(1);
    }

    ptr[i]->x = arr[i]->x;
    ptr[i]->y = arr[i]->y;
    ptr[i]->index = arr[i]->index;
  }
  for (; i < POINT_AMOUNT - 1; i++) {
    ptr[i] = malloc(sizeof(Point_t));
    if (!ptr[i]) {
      perror("ERROR in arr_copy()");
      exit(1);
    }

    ptr[i]->x = arr[i + 1]->x;
    ptr[i]->y = arr[i + 1]->y;
    ptr[i]->index = arr[i + 1]->index;
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

int stk_findPoint(Point_t* arr, Point_t point, int length) {
  int find = 0;
  for (int i = 0; i < length; i++) {
    if (arr[i].x == point.x && arr[i].y == point.y) {
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

// comparison function which returns ​a negative integer value if the first
// argument is less than the second, a positive integer value if the first
// argument is greater than the second and zero if the arguments are equivalent.
int Point_t_compare(const void* p1, const void* p2) {
  const Point_t* point1 = *((const Point_t**)p1);
  const Point_t* point2 = *((const Point_t**)p2);
  return -1 * rotate(&lower_left, point1, point2);
}

Point_t* graham_algorithm(Point_t** arr) {
  // index - номера точек, их позиция в arr
  Node_t* index = list_init(POINT_AMOUNT, 1);

  // Этап 1. Найти самую левую нижнюю точку.

  for (int i = 0; i < POINT_AMOUNT; i++) {
    if (arr[i]->x < lower_left.x) {
      lower_left = *arr[i];
    }
  }

  for (int i = 0; i < POINT_AMOUNT; i++) {
    if (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y) {
      lower_left = *arr[i];
    }
  }

  change_value(index, 0, lower_left.index);
  change_value(index, lower_left.index, 0);

  // Этап 2. Сортировка точек, кроме нулевой (самой левой).

  // создание копии без элемента с индексом lower_left.index
  Point_t** copy = arr_copy(arr, lower_left.index);

  qsort(copy, POINT_AMOUNT - 1, sizeof(Point_t*), Point_t_compare);

  for (int i = 1; i < POINT_AMOUNT; i++) {
    change_value(index, i, copy[i - 1]->index);
  }

  // Этап 3. Добавление точек в оболочку (хранение в "стеке").

  Node_t* curve = list_init(10000, 0);

  change_value(curve, 0, get_value(index, 0));
  append(&curve, get_value(index, 1));

  for (int i = 2; i < POINT_AMOUNT; i++) {
    while (rotate(arr[get_penultimate(curve)], arr[get_last(curve)], arr[get_value(index, i)]) < 0) {
      //int tmp = remove_last(curve);
      //remove_last(curve);
      //append(&curve, tmp);
      remove_last(curve);
    }
    append(&curve, get_value(index, i));
  }

  // Вывод

  Point_t* curve_arr = calloc(POINT_AMOUNT, sizeof(Point_t));
  if (!curve_arr) {
    perror("ERROR in graham_algorithm()");
    exit(1);
  }

  Point_t* stack_ptr = curve_arr;
  int curve_length = get_length(curve);
  Point_t plug = {-1, -1, -1};

  for (int i = 0; i < curve_length; i++) {
    push(stack_ptr, *arr[remove_first(&curve)]);  // curve frees here !
  }
  for (int i = curve_length; i < POINT_AMOUNT; i++) {
    push(stack_ptr, plug);
  }

  // Освобождение памяти (curve освобождается в предпоследнем for)
  list_free(index);
  arr_free(copy, POINT_AMOUNT - 1);

  return curve_arr;
}

Point_t* jarvis_algorithm(Point_t** arr, const int length) {
  // index - номера точек, их позиция в arr
  Node_t* index = list_init(POINT_AMOUNT, 1);
  // curve - вершины оболочки
  Node_t* curve = list_init(1000000, 0);

  // Этап 1. Найти самую левую нижнюю точку.

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

  // Вывод

  Point_t* curve_arr = calloc(POINT_AMOUNT, sizeof(Point_t));
  if (!curve_arr) {
    perror("ERROR in jarvis_scan()");
    exit(1);
  }

  Point_t* stack_ptr = curve_arr;
  int curve_length = get_length(curve);
  Point_t plug = {-1, -1, -1};

  for (int i = 0; i < curve_length; i++) {
    push(stack_ptr, *arr[remove_first(&curve)]);  // curve frees here !
  }
  for (int i = curve_length; i < POINT_AMOUNT; i++) {
    push(stack_ptr, plug);
  }

  // Освобождение памяти (curve освобождается в предпоследнем for)
  list_free(index);

  return curve_arr;
}
