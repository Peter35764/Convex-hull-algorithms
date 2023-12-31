#ifndef CW_ACT_MAIN_H
#define CW_ACT_MAIN_H

#include "linked_list/linked_list.h"

// Универсальные макросы "стека" для массива точек (Point_t** или Point_t*).
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


// Алгоритм Грэхема.
// Использутся глобальная переменная Point lower_left.
Node_t* graham_algorithm(Point_t** arr, int length);
// Алгоритм Джарвиса.
// Использутся глобальная переменная Point lower_left.
Node_t* jarvis_algorithm(Point_t** arr, int length);
// TODO
Node_t* chan_algorithm(Point_t** arr, int length);
// TODO
void hull_compare();
// Вычисление z-компоненты векторного произведения векторов ab и bc.
// Если возвращаемое значение > 0, то точка с находится слева от вектора ab.
// Если возвращаемое значение < 0, то точка с находится справа от вектора ab.
// Если возвращаемое значение = 0, то точка с находится на одной линии с
// вектором ab.
int rotate(Point_t* a, const Point_t* b, const Point_t* c);
// Функция-обертка rotate() для qsort.
// Использутся глобальная переменная Point lower_left.
int Point_t_compare(const void* p1, const void* p2);



// Инициализация (выделение памяти) для массива точек.
Point_t** arr_init(int length);
//
Point_t** arr_copy(Point_t** arr, int exclude_index);
// Освобождение памяти из под массива точек.
void arr_free(Point_t** ptr, int length);



// Вывести координаты точки в stdout.
void printPoint(Point_t p);
//
void printStack(Point_t* ptr,  int length, const char* name);
//
void printArray(Point_t** ptr,  int length, const char* name);
//
int arr_findPoint(Point_t** arr, Point_t point, int length);
//
int stk_findPoint(Point_t* arr, Point_t point, int length);

// TODO
Point_t** input();
// Представление полученной оболочки в виде набора символов в текстовом файле.
void output(Point_t** arr, Point_t* hull, int hull_length);
// Функция преобразования упорядоченного связного списка точек к массиву для передачи в функции печати и вывода в файл.
Point_t* to_output_stack(Point_t** arr, Node_t* hull);

#endif
