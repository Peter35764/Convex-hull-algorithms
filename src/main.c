#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "linked_list/linked_list.h"

// ==================================================================
//   === Построение минимальной выпуклой оболочки на плоскости. =======
// ==================================================================

// Алгоритм Джарвиса.       JARVIS
// Алгоритм Грэхема.        GRAHAM
// Алгоритм Чана.           CHAN TODO!!!!

#define JARVIS

// Размеры квадрата первой четверти плоскости, в котором могут быть точки.
#define FIELD_SIZE 1000
// Количество сгенерированных точек.
#define POINT_AMOUNT 2000

//   ==================================================================
// =================================================================
//   ==================================================================

// Глобальная переменная для нахождения левой-нижней точки.
Point_t lower_left = {FIELD_SIZE + 1, FIELD_SIZE + 1, -1};

int main(void) {
  Point_t** array = arr_init(POINT_AMOUNT);
  printArray(array, POINT_AMOUNT, "Array of points");

  clock_t start_time = clock();

  #ifdef GRAHAM
  Node_t* hull = graham_algorithm(array, POINT_AMOUNT);
  #endif
  #ifdef JARVIS
  Node_t* hull = jarvis_algorithm(array, POINT_AMOUNT);
  #endif
  #ifdef CHAN
  Node_t* hull = chan_algorithm(array, POINT_AMOUNT);
  #endif

  clock_t end_time = clock();

  int hull_length = get_length(hull);
  Point_t* convex_hull = to_output_stack(array, hull);  // Point_t* hull frees here !
  printStack(convex_hull, hull_length, "Convex hull");

  printf("\n=== Execution time: %f secs.\n", ((double)(end_time - start_time)) / CLOCKS_PER_SEC);

  if (FIELD_SIZE > 500) {
    printf("\n\tWARNING: field is too big for writing it to the output.txt file!\n\tFile output.txt not created!\n\n");
  } else {
    output(array, convex_hull, hull_length);
  }
  
  arr_free(array, POINT_AMOUNT);
  free(convex_hull);

  return 0;
}

Node_t* graham_algorithm(Point_t** arr, int length) {
  // index - номера точек, их позиция в arr.
  Node_t* index = list_init(length, 1);

  // Этап 1. Найти самую левую нижнюю точку.

  for (int i = 0; i < length; i++) {
    if (arr[i]->x < lower_left.x || (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y)) {
      lower_left = *arr[i];
    }
  }

  // for (int i = 0; i < POINT_AMOUNT; i++) {
  //   if (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y) {
  //     lower_left = *arr[i];
  //   }
  // }

  change_value(index, 0, lower_left.index);
  change_value(index, lower_left.index, 0);

  // Этап 2. Сортировка точек, кроме нулевой (самой левой).

  // создание копии без элемента с индексом lower_left.index
  Point_t** copy = arr_copy(arr, lower_left.index);

  qsort(copy, length - 1, sizeof(Point_t*), Point_t_compare);

  for (int i = 1; i < length; i++) {
    change_value(index, i, copy[i - 1]->index);
  }

  // Этап 3. Добавление точек в оболочку (хранение в "стеке").

  Node_t* hull = list_init(10000, 0);

  change_value(hull, 0, get_value(index, 0));
  append(&hull, get_value(index, 1));

  for (int i = 2; i < length; i++) {
    while (rotate(arr[get_penultimate(hull)], arr[get_last(hull)], arr[get_value(index, i)]) < 0) {
      remove_last(hull);
    }
    append(&hull, get_value(index, i));
  }

  // Вывод (переехал в отдельную функцию)

  // Point_t* hull_arr = calloc(length, sizeof(Point_t));
  // if (!hull_arr) {
  //   perror("ERROR in graham_algorithm()");
  //   exit(1);
  // }

  // Point_t* stack_ptr = hull_arr;
  // int hull_length = get_length(hull);
  // Point_t plug = {-1, -1, -1};

  // for (int i = 0; i < hull_length; i++) {
  //   push(stack_ptr, *arr[remove_first(&hull)]);  // hull frees here !
  // }
  // for (int i = hull_length; i < length; i++) {
  //   push(stack_ptr, plug);
  // }

  // Освобождение памяти (hull освобождается в предпоследнем for)
  list_free(index);
  arr_free(copy, length - 1);

  return hull;
}

Node_t* jarvis_algorithm(Point_t** arr, int length) {
  // index - номера точек, их позиция в arr
  Node_t* index = list_init(length, 1);
  // hull - вершины оболочки
  Node_t* hull = list_init(1000000, 0);

  // Этап 1. Найти самую левую нижнюю точку.

  for (int i = 0; i < length; i++) {
    if (arr[i]->x < lower_left.x || (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y)) {
      lower_left = *arr[i];
    }
  }

  // for (int i = 0; i < POINT_AMOUNT; i++) {
  //   if (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y) {
  //     lower_left = *arr[i];
  //   }
  // }

  change_value(index, 0, lower_left.index);
  change_value(index, lower_left.index, 0);

  // Этап 2.

  change_value(hull, 0, remove_first(&index));
  append(&index, get_value(hull, 0));

  while (1) {
    int right = 0;
    for (int i = 1; i < get_length(index); i++) {
      if (rotate(arr[get_last(hull)], arr[get_value(index, right)],
                 arr[get_value(index, i)]) < 0) {
        right = i;
      }
    }
    if (get_value(index, right) == get_value(hull, 0)) {
      break;
    } else {
      append(&hull, get_value(index, right));
      remove_index(&index, right);
    }
  }

  list_free(index);

  return hull;
}

Node_t* chan_algorithm(Point_t** arr __attribute__((unused)), int length  __attribute__((unused))) {
  return 0;  
}

int rotate(Point_t* a, const Point_t* b, const Point_t* c) {
  int ab_x = b->x - a->x;
  int ab_y = b->y - a->y;
  int bc_x = c->x - b->x;
  int bc_y = c->y - b->y;
  return (ab_x) * (bc_y) - (ab_y) * (bc_x);
}

// comparison function which returns ​a negative integer value if the first
// argument is less than the second, a positive integer value if the first
// argument is greater than the second and zero if the arguments are equivalent.
int Point_t_compare(const void* p1, const void* p2) {
  const Point_t* point1 = *((const Point_t**)p1);
  const Point_t* point2 = *((const Point_t**)p2);
  return -1 * rotate(&lower_left, point1, point2);
}



Point_t** arr_init(int length) {
  srand(time(NULL));

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

void arr_free(Point_t** ptr, int length) {
  for (int i = 0; i < length; i++) {
    free(ptr[i]);
  }
  free(ptr);
}


void printPoint(Point_t p) {
    printf("i: %d  x: %d  y: %d\n", p.index, p.x, p.y);
}

void printStack(Point_t* ptr, int length, const char* name) {

  printf("\n=== %s ===\n", name);
  for (int i = 0; i < length; i++) {
    printPoint(ptr[i]);
  }
  printf("=== stack length: %i ===\n", length);
}

void printArray(Point_t** ptr, int length, const char* name) {
  printf("\n=== %s ===\n", name);
  for (int i = 0; i < length; i++) {
    printPoint(*ptr[i]);
  }
  printf("=== array length: %i ===\n", length);
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




void output(Point_t** arr, Point_t* hull, int hull_length) {
  #ifdef JARVIS
  const char* filename = "../output/jarvis_output.txt";
  #endif
  #ifdef GRAHAM
  const char* filename = "../output/graham_output.txt";
  #endif
  #ifdef CHEN
  const char* filename = "../output/chen_output.txt";
  #endif
  FILE* file = fopen(filename, "w");

  for (int i = 0; i <= FIELD_SIZE + 2; i++) {
    fprintf(file, "-");
  }
  fprintf(file, "\n");

  for (int i = 0; i <= FIELD_SIZE; i++) {
    fprintf(file, "|");
    for (int j = 0; j <= FIELD_SIZE; j++) {
      Point_t tmp = {i, j, -100};
      if (stk_findPoint(hull, tmp, hull_length)) {
        fputc('@', file);
      } else if (arr_findPoint(arr, tmp, hull_length)) {
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

Point_t* to_output_stack(Point_t** arr, Node_t* hull) {
    int hull_length = get_length(hull);

  Point_t* hull_arr = calloc(hull_length, sizeof(Point_t));
  if (!hull_arr) {
    perror("ERROR in graham_algorithm()");
    exit(1);
  }

  Point_t* stack_ptr = hull_arr;

  for (int i = 0; i < hull_length; i++) {
    push(stack_ptr, *arr[remove_first(&hull)]);  // hull frees here !
  }

  return hull_arr;
  }