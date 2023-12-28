#include "main.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linked_list/linked_list.h"

// ==================================================================
//   === Построение минимальной выпуклой оболочки на плоскости. =======
// ==================================================================

// Алгоритм Джарвиса.       JARVIS
// Алгоритм Грэхема.        GRAHAM
// Алгоритм Чана.           CHAN

#define JARVIS

// TEST PROD
#define PROD

// Размеры квадрата первой четверти плоскости, в котором могут быть точки.
#define FIELD_SIZE 20000
// Количество сгенерированных точек.
#define POINT_AMOUNT 10000

//   ==================================================================
// =================================================================
//   ==================================================================

// Глобальная переменная для нахождения левой-нижней точки.
Point_t lower_left = {FIELD_SIZE + 1, FIELD_SIZE + 1, -1};

int main(void) {
  Point_t** array = arr_init(POINT_AMOUNT);

#ifdef PROD
  clock_t start_time = clock();

  printArray(array, POINT_AMOUNT, "array afrer intitalisation");

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
  Point_t* convex_hull =
      to_output_stack(array, POINT_AMOUNT, hull);  // Point_t* hull frees here !
  printStack(convex_hull, hull_length, "Convex hull");

  printf("\n=== Execution time: %f secs.\n",
         ((double)(end_time - start_time)) / CLOCKS_PER_SEC);

  if (FIELD_SIZE > 500) {
    printf(
        "\n\tWARNING: field is too big for writing it to the output.txt "
        "file!\n\tFile output.txt not created!\n\n");
  } else {
    output(array, POINT_AMOUNT, convex_hull, hull_length);
  }
  
  arr_free(array, POINT_AMOUNT);
  free(convex_hull);
#endif

#ifdef TEST

  array[0]->index = 0; array[0]->x = 1; array[0]->y = 1;
  array[1]->index = 1; array[1]->x = 1; array[1]->y = 4;
  array[2]->index = 2; array[2]->x = 4; array[2]->y = 1;
  array[3]->index = 3; array[3]->x = 2; array[3]->y = 2;

#endif

  return 0;
}

Node_t* graham_algorithm(Point_t** arr, int length) {
  // index - номера точек, их позиция в arr.
  Node_t* index = list_init(length, arr, 2);

  // Этап 1. Найти самую левую нижнюю точку.

  lower_left.x = FIELD_SIZE + 1;
  lower_left.y = FIELD_SIZE + 1;
  lower_left.index = -1;
  for (int i = 0; i < length; i++) {
    if (arr[i]->x < lower_left.x ||
        (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y)) {
      lower_left = *arr[i];
    }
  }

  // Поменять местами нулевой элемент с элементом индекса lower_left.index
  int tmp = get_value(index, 0);
  change_value(index, get_index(index, lower_left.index), tmp);
  change_value(index, 0, lower_left.index);

  // Этап 2. Сортировка точек, кроме нулевой (самой левой).

  // создание копии без элемента с индексом lower_left.index
  Point_t** copy = arr_copy(arr, lower_left.index, length);

  qsort(copy, length - 1, sizeof(Point_t*), Point_t_compare);

  for (int i = 1; i < length; i++) {
    change_value(index, i, copy[i - 1]->index);
  }

  // Этап 3. Добавление точек в оболочку (хранение в "стеке").

  Node_t* hull = list_init(10000, arr, 0);
  change_value(hull, 0, get_value(index, 0));
  append(&hull, get_value(index, 1));

  for (int i = 2; i < length; i++) {
    while (rotate(arr[find_by_index(arr, length, get_penultimate(hull))],
                  arr[find_by_index(arr, length, get_last(hull))],
                  arr[find_by_index(arr, length, get_value(index, i))]) < 0) {
      remove_last(hull);
    }
    append(&hull, get_value(index, i));
  }

  // Освобождение памяти (hull освобождается в предпоследнем for)

  list_free(index);
  arr_free(copy, length - 1);

  return hull;
}

Node_t* jarvis_algorithm(Point_t** arr, int length) {
  // index - номера точек, их позиция в arr
  Node_t* index = list_init(length, arr, 1);
  // hull - вершины оболочки
  Node_t* hull = list_init(1000000, arr, 0);

  // Этап 1. Найти самую левую нижнюю точку.

  for (int i = 0; i < length; i++) {
    if (arr[i]->x < lower_left.x ||
        (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y)) {
      lower_left = *arr[i];
    }
  }

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

Node_t* chan_algorithm(Point_t** arr, int length) {
  int success = 0;
  Node_t* hull = NULL;

  Point_t p0 = {
      -1, 0,
      POINT_AMOUNT};  // p0 хранится под самым большим индексом POINT_AMOUNT.

  // Этап 1. Найти самую левую нижнюю точку.
  for (int i = 0; i < length; i++) {
    if (arr[i]->x < lower_left.x ||
        (arr[i]->x == lower_left.x && arr[i]->y < lower_left.y)) {
      lower_left = *arr[i];
    }
  }

  for (int t = 0; t < (int)ceil(log(log(length))); t++) {
    int m = pow(2, pow(2, t));  // Количество точек в подмножестве точек.

    // Создание списка для хранения оболочки
    hull = list_init(
        POINT_AMOUNT, arr,
        0);  // p0 хранится под самым большим индексом POINT_AMOUNT.
    //change_value(hull, 0,
    //             lower_left.index);  // Добавить первый элемент в оболочку.
    append(&hull, lower_left.index);

    Point_t*** subsets = split(arr, length, m);
    int subset_amount = ceil((double)length / m);
    int last_subset_length = length % m;

    Node_t** subset_hulls = malloc(subset_amount * sizeof(Node_t*));

    for (int k = 0; k < subset_amount; k++) {
      if (last_subset_length == 0) {
        subset_hulls[k] = graham_algorithm(subsets[k], m);
      } else { 
        while (k < subset_amount - 1) {
          subset_hulls[k] = graham_algorithm(subsets[k], m);
        }
        subset_hulls[k] = graham_algorithm(subsets[k], last_subset_length);
      }
    }

    // Модифицированный алгоритм Джарвиса.

    // Создание массива потенциальных точек.
    int potentially_new_point_arr[subset_amount];
    memset(potentially_new_point_arr, 0, subset_amount*sizeof(int));
    
    //   //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // for (int j = 0; j < subset_amount; j++) printf("%i\n", potentially_new_point_arr[j]);
    // printf("\n\n\n\n\n");
    //   //////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < m; i++) {
      Point_t* penultimate = NULL;
        if (get_penultimate(hull)==POINT_AMOUNT)  {
          penultimate = &p0;
        } else {
          penultimate = arr[find_by_index(arr, length, get_penultimate(hull))];
        }
      for (int k = 0;k < subset_amount; k++) {
        potentially_new_point_arr[k] = binary_search(
            arr, length, penultimate, arr[find_by_index(arr, length, get_last(hull))],
            subset_hulls[k]);
      }
//       //////////////////////////////////////////////////////////////////////////////////////////////////////////
//       for (int j = 0; j < subset_amount; j++) printf("%i\n", potentially_new_point_arr[j]);
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      int aboba = next_hull_point(arr, penultimate, arr[find_by_index(arr, length, get_last(hull))],
                          potentially_new_point_arr, subset_amount);
      if (get_value(hull, 0) == aboba) {
        success = 1;
        break;
      } else {
        append(&hull, aboba);
      }
    }
      split_free(subsets, length, m);
      for (int i = subset_amount-1; i >= 0; i--) {
        free(subset_hulls[i]);
      }
      free(subset_hulls);
    if (!success) {
      list_free(hull);
    } else {
      break;
    }
  }
  return hull;
}
// возвращает индекс внутри arr (то есть абсолютный индекс, прямо из точки")
int binary_search(Point_t** arr, int arr_length, Point_t* penultimate,
                  Point_t* last, Node_t* subhull) {

  /*int potential_winner = -1;

  def binary_search(list, key):
    low = 0
    high = len(list) - 1

    while low <= high:
        mid = (low + high) // 2
        midVal = list[mid]
        if midVal == key:
            return mid
        if midVal > key:
            high = mid - 1
        else:
            low = mid + 1
    return 'not found :('
  */
  /*
  // сделать так, чтобы независимо от подаваемого массива и оболочки алгоритм
  отрабатывал int low = 0; int high = get_length(subhull), potential_winner =
  -1; int mid, compare_result; printf("high %i\n", high); while (low < high) {
    mid = (low + high) / 2;
    //    while (rotate(arr[find_by_index(arr, length, get_penultimate(hull))],
    printf("mid %i\n", mid);

    //     potential_winner = get_value(subhull, find_by_index(arr, arr_length,
  get_value(subhull, mid))); printf("\taboba1\n"); potential_winner =
  get_value(subhull, mid); printf("potential winner %i i %i x %i y %i\n",
  potential_winner, arr[find_by_index(arr, arr_length,
  potential_winner)]->index, arr[find_by_index(arr, arr_length,
  potential_winner)]->x, arr[find_by_index(arr, arr_length,
  potential_winner)]->y);

    printf("\taboba2\n");
    compare_result = rotate(penultimate, last,  arr[potential_winner]); //
  arr[find_by_index(arr, arr_length, potential_winner)] if (compare_result == 0)
  { break; } else if (compare_result > 0) { if (high == mid) break; high = mid;
    } else {
      if (low == mid) break;
      low = mid;
    }
  }
*/

  int subhull_length = get_length(subhull);
  int ans = 0;
  for (int i = 0; i < subhull_length; i++) {
    double first = angle(penultimate, last,
              arr[find_by_index(arr, arr_length, get_value(subhull, i))]);
    double second = angle(penultimate, last,
              arr[find_by_index(arr, arr_length, get_value(subhull, ans))]);
    if (first < second) {
      ans = i;
    }
  }
  return get_value(subhull, ans);
}

int next_hull_point(Point_t** arr, Point_t* penultimate, Point_t* last,
                    int* pnp, int subset_amount) {
      

                      
  int ans = 0;
  for (int i = 1; i < subset_amount; i++) {
    if (angle(penultimate, last, arr[pnp[i]])<
        angle(penultimate, last, arr[ans])) {
      ans = pnp[i];
    }
  }
  //printf("\nans%i\n", ans);
  return ans;
}

Point_t*** split(Point_t** arr, int length, int m) {
  if (m > length) {
    fprintf(stderr, "\nERROR in split(): m > length\n\n");
    exit(1);
  } else if (m == 0) {
    fprintf(stderr, "\nERROR in split(): m == 0\n\n");
    exit(1);
  }
  int subset_number = ceil((double)length / m);
  Point_t*** subsets = calloc(subset_number, sizeof(Point_t**));
  int index = 0;
  for (int i = 0; i < subset_number; i++) {
    subsets[i] = calloc(m, sizeof(Point_t*));
    for (int j = 0; j < m; j++) {
      subsets[i][j] = arr[index++];
      if (index == length) break;
    }
  }
  return subsets;
}

int rotate(Point_t* a, const Point_t* b, const Point_t* c) {
  int ab_x = b->x - a->x;
  int ab_y = b->y - a->y;
  int bc_x = c->x - b->x;
  int bc_y = c->y - b->y;
  return (ab_x) * (bc_y) - (ab_y) * (bc_x);
}

// Угол между ab и bc
double angle(Point_t* a, const Point_t* b, const Point_t* c) {
  int ab_x = b->x - a->x;
  int ab_y = b->y - a->y;
  int bc_x = c->x - b->x;
  int bc_y = c->y - b->y;
  return acos(
      (double)(ab_x * bc_x + ab_y * bc_y) /
      (sqrt(ab_x * ab_x + ab_y * ab_y) * sqrt(bc_x * bc_x + bc_y + bc_y)));
}

// comparison function which returns ​a negative integer value if the first
// argument is less than the second, a positive integer value if the first
// argument is greater than the second and zero if the arguments are equivalent.
int Point_t_compare(const void* p1, const void* p2) {
  const Point_t* point1 = *((const Point_t**)p1);
  const Point_t* point2 = *((const Point_t**)p2);
  return -1 * rotate(&lower_left, point1, point2);
}

int find_by_index(Point_t** arr, int length, int index) {
  int ans = -1;
  for (int i = 0; i < length; i++) {
    if (arr[i]->index == index) {
      ans = i;
      break;
    }
  }
  return ans;
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

Point_t** arr_copy(Point_t** arr, int exclude_index, int length) {
  Point_t** ptr = malloc((length - 1) * sizeof(Point_t*));
  if (!ptr) {
    perror("ERROR in arr_copy()");
    exit(1);
  }

  int i = 0;
  for (; i < length - 1; i++) {
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
  for (; i < length - 1; i++) {

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

void split_free(Point_t*** arr, int length, int m) {
  int subset_number = ceil((double)length / m);
  for (int i = 0; i < subset_number; i++) {
    free(arr[i]);
  }
  free(arr);
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

void output(Point_t** arr, int arr_length, Point_t* hull, int hull_length) {
#ifdef JARVIS
  const char* filename = "../output/jarvis_output.txt";
#endif
#ifdef GRAHAM
  const char* filename = "../output/graham_output.txt";
#endif
#ifdef CHAN
  const char* filename = "../output/chan_output.txt";
#endif
  FILE* file = fopen(filename, "w");

  for (int i = 0; i <= FIELD_SIZE + 2; i++) {
    fprintf(file, "-");
  }
  fprintf(file, "\n");

  for (int j = FIELD_SIZE; j >= 0; j--) {
    fprintf(file, "|");
    for (int i = 0; i <= FIELD_SIZE; i++) {
      Point_t tmp = {i, j, -100};
      if (stk_findPoint(hull, tmp, hull_length)) {
        fputc('@', file);
      } else if (arr_findPoint(arr, tmp, arr_length)) {
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

Point_t* to_output_stack(Point_t** arr, int arr_length, Node_t* hull) {
  int hull_length = get_length(hull);
  Point_t* hull_arr = calloc(hull_length, sizeof(Point_t));

  if (!hull_arr) {
    perror("ERROR in graham_algorithm()");
    exit(1);
  }

  Point_t* stack_ptr = hull_arr;

  for (int i = 0; i < hull_length; i++) {
    int tmp = remove_first(&hull);  // hull frees here !
    // printPoint(*arr[find_by_index(arr, arr_length, tmp)]);
    push(stack_ptr, *arr[find_by_index(arr, arr_length, tmp)]);
  }

  return hull_arr;
}