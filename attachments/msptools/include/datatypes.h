#ifndef DATATYPES_H
#define DATATYPES_H
#include <stdlib.h>

enum storage_order
{
    RowMajor,
    ColMajor
};

typedef struct array /* dynamic array */
{
    size_t len;
    size_t capacity;
    double *val;
} array_t;

typedef struct array2d /* two-dimensional array */
{
    size_t shape[2];
    enum storage_order order;
    double *val;
} array2d_t;

#endif