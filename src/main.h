#ifndef CW_ACT_MAIN_H
#define CW_ACT_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Размеры квадрата первой четверти плоскости, в котором могут быть точки.
#define FIELD_SIZE 100

typedef struct Point {
  int x, y;
} Point;

// Вычисление z-компоненты векторного произведения векторов ab и bc.
// Если возвращаемое значение > 0, то точка с находится слева от вектора ab.
// Если возвращаемое значение < 0, то точка с находится справа от вектора ab.
// Если возвращаемое значение = 0, то точка с находится на одной линии с
// вектором ab.
int rotate(Point* a, const Point* b, const Point* c);
// Вывести координаты точки в stdout.
void printPoint(Point p);
// Инициализация (выделение памяти) для массива точек.
Point** arr_init(int length);
// Освобождение памяти из под массива точек.
void arr_free(Point** ptr, int length);
//
void graham_scan(Point** arr, const int length);
#endif
