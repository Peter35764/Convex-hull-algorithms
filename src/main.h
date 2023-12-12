#ifndef CW_ACT_MAIN_H
#define CW_ACT_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Размеры квадрата первой четверти плоскости, в котором могут быть точки.
#define FIELD_SIZE 10  // Object-like Macros
// Количество сгенерированных точек.
#define POINT_AMOUNT 4

// Функции "стека".
#define push(stack_ptr, element) (*((stack_ptr)++) = (element))
#define pop(stack_ptr) (--(stack_ptr))  // (*(stack_ptr)--)
// Generic для определения типа подаваемой переменной.
#define typeof_Point(x)     \
  _Generic((x),             \
      Point: "Point",       \
      Point *: "Point*",    \
      Point * *: "Point**", \
      default: "?")

typedef struct Point {
  int x, y, index;
} Point_t;

// Вычисление z-компоненты векторного произведения векторов ab и bc.
// Если возвращаемое значение > 0, то точка с находится слева от вектора ab.
// Если возвращаемое значение < 0, то точка с находится справа от вектора ab.
// Если возвращаемое значение = 0, то точка с находится на одной линии с
// вектором ab.
int rotate(Point_t* a, const Point_t* b, const Point_t* c);
// Вывести координаты точки в stdout.
void printPoint(Point_t p);
//
void printStack(Point_t* ptr, const int length, const char* name);
//
void printArray(Point_t** ptr, const int length, const char* name);
// Инициализация (выделение памяти) для массива точек.
Point_t** arr_init(int length);
// Освобождение памяти из под массива точек.
void arr_free(Point_t** ptr, int length);
// Функция-обертка rotate() для qsort.
// Использутся глобальная переменная Point lower_left.
int Point_compare(const void* p1, const void* p2);
// Алгоритм Грэхема.
// Использутся глобальная переменная Point lower_left.
Point_t* graham_scan(Point_t** arr, const int length);
//
Point_t* jarvis_scan(Point_t** arr, const int length);

#endif
