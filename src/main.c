#include "main.h"

// Построение минимальной выпуклой оболочки на плоскости.
// Алгоритм Джарвиса.
// Алгоритм Грэхема.
// Рекурсивный алгоритм.

int main() {
  srand(time(NULL));

  Point** arr = NULL;

  arr = arr_init(10);

  printPoint(*arr[0]);
  printPoint(*arr[1]);
  printPoint(*arr[2]);

  printf("Поворот: %d\n", rotate(*arr[0], *arr[1], *arr[2]));

  // graham_scan(arr, 10);
  arr_free(arr, 10);
  return 0;
}

int rotate(Point a, Point b, Point c) {
  int ab_x = b.x - a.x;
  int ab_y = b.y - a.y;
  int bc_x = c.x - b.x;
  int bc_y = c.y - b.y;
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

void graham_scan(Point** arr, int length) {
  int index[length];
  for (int i = 0; i < length; i++) {
    index[i] = i;
  }
  
  
  
  /*
  for (int i = 0; i < length; i++) {
    printf("%d\n", index[i]);
  }
  arr[5]->x = 0;*/
}
