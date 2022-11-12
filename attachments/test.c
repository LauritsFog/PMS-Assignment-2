#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "array2d.h"
#include "array.h"

/* C prototype for LAPACK routine DGESV */
void dgesv_(const int *n,    /* columns/rows in A          */
            const int *nrhs, /* number of right-hand sides */
            double *A,       /* array A                    */
            const int *lda,  /* leading dimension of A     */
            int *ipiv,       /* pivoting array             */
            double *B,       /* array B                    */
            const int *ldb,  /* leading dimension of B     */
            int *info        /* status code                */
);

// functions for 1-dim array
array_t *array_alloc(const size_t capacity);
void array_fprint(FILE *stream, const array_t *a);
int array_resize(array_t *a, size_t new_capacity);
void array_print(const array_t *a);
int array_push_back(array_t *a, double value);

// functions for 2-dim array
array2d_t *array2d_alloc(const size_t shape[2], enum storage_order order);
void array2d_fprint(FILE *stream, const array2d_t *a);
void array2d_print(const array2d_t *a);

int call_dgesv(array2d_t * A, array_t * b);

int main(void){

    double xcol[4] = {
            1,7,-2,-3
    };
    double xrow[4] = {
            1,-2,7,-3
    };
    double y[2] = {-2,19};



    array_t *b = array_alloc(2); // Allocate dynamic array with capacity 8
    assert(b != NULL);

    for (size_t k = 0; k < 2; k++){
        array_push_back(b,y[k]);
    }
    // array_print(b);

   //  printf("\n Now on to 2d arrays \n");

    array2d_t *A = array2d_alloc((size_t[]){2, 2}, ColMajor);
    assert(A != NULL);

    for (size_t k = 0; k < 4; k++){
        A->val[k] = xcol[k];
    }
    // array2d_print(A);

    call_dgesv(A,b);

    printf("\n Solution for Ax=b is then: \n");
    array_print(b);

    return 0;
}



int call_dgesv(array2d_t * A, array_t * b){

    int m = (int) A->shape[0], n = (int) A->shape[1];
    int nrhs = 1, ldb=m, lda=m, info;
    int ipiv[n];
    
    if (A->order == ColMajor){
        dgesv_( &m, &nrhs, A->val, &lda, ipiv, b->val, &ldb, &info );
    } else{
        int nm = n*m;
        double xrow[nm];

        for (size_t i = 0; i < nm; i++){
           xrow[i] = A->val[i];
        }
        int k = 0;

        
        for (size_t i = 0; i < n; i++){
            
            for (size_t j = 0; j < n; j++)
            {
               A->val[k] = xrow[i+n*j];
               k = k+1;
            }
        }
        
        dgesv_( &m, &nrhs, A->val, &lda, ipiv, b->val, &ldb, &info );

    }
    
    
    
        
    return info;
}

array_t *array_alloc(const size_t capacity)
{
  array_t *a = malloc(sizeof(*a));
  if (a == NULL)
  {
#ifndef NDEBUG
    MEM_ERR;
#endif
    return NULL;
  }
  a->capacity = (capacity > 0 ? capacity : 1); // Minimum capacity is 1
  a->len = 0;
  a->val = malloc((a->capacity) * sizeof(*(a->val)));
  if (a->val == NULL)
  {
#ifndef NDEBUG
    MEM_ERR;
#endif
    free(a);
    return NULL;
  }
  return a;
}

int array_push_back(array_t *a, double x)
/*
  Purpose:

    Appends an element to an array and, if necessary, increases the capacity
    by a factor of two.

  Example:

    ```c
    array_t *a = array_alloc(50);
    if (a==NULL) exit(EXIT_FAILURE);    
    for (size_t k=0;k<500;k++) array_push_back(a, 2.0*k);
    // .. do something with array ..
    array_dealloc(a);    
    ```

  Arguments:
    arr        a pointer to an array_t
    value      the value to be appended

  Return value:
    MSP_SUCCESS if successful, MSP_FAILURE if an error occurs, 
    and MSP_ILLEGAL_INPUT if the input is a NULL pointer.
*/
{
  if (!a)
    return MSP_ILLEGAL_INPUT;
  if (a->capacity <= a->len)
  {
    int ret = array_resize(a, 2 * (a->capacity));
    if (ret != MSP_SUCCESS)
      return ret;
  }
  a->val[a->len++] = x;
  return MSP_SUCCESS;
}

int array_resize(array_t *a, size_t new_capacity)
/* 
  Purpose:

    Resizes dynamic array. The length of the array is reduced if 
    the new capacity is smaller than the length, and otherwise 
    the length is unchanged.

  Arguments:
    new_capacity    capacity after resizing

  Return value:
    Returns MSP_SUCCESS if resizing is successful, MSP_MEM_ERR if 
    reallocation fails, and MSP_ILLEGAL_INPUT if the input is a 
    NULL pointer.

  See also: array_alloc, array_dealloc

*/
{
  if (!a)
    return MSP_ILLEGAL_INPUT; // Received null pointer
  double *tmp = realloc(a->val, new_capacity * sizeof(double));
  if (!tmp)
    return MSP_MEM_ERR; // Reallocation failed
  a->val = tmp;
  a->capacity = new_capacity;
  a->len = (a->len <= new_capacity) ? a->len : new_capacity;
  return MSP_SUCCESS; // Reallocation successful
}

void array_fprint(FILE *stream, const array_t *a)
/*
  Purpose:

    Prints an array_t.

  Arguments:
    stream       a pointer to a file stream
    a            a pointer to an array_t

  Return value:
    None
*/
{
  if (a == NULL)
    return;
  size_t len = a->len;
  fprintf(stream, "<array_t len=%zu capacity=%zu>\n", len, a->capacity);
  for (size_t i = 0; i < len; i++)
  {
    fprintf(stream, " % .3g\n", a->val[i]);
  }
  return;
}

void array_print(const array_t *a) { array_fprint(stdout, a); }

array2d_t *array2d_alloc(const size_t shape[2], enum storage_order order)

/*
  Purpose:

    Allocates a two-dimensional array with a given shape. The array 
    elements are initialized as zero.

  Example:

    ```c
    array2d_t *a = array2d_alloc(size_t nrows, size_t ncols, RowMajor);
    if (a==NULL) exit(EXIT_FAILURE);    
    // .. do something with array ..
    array2d_dealloc(a);
    ```

  Arguments:
    shape        array of length 2 (number of rows and columns)
    order        RowMajor or ColMajor

  Return value:
    A pointer to an array2d_t, or NULL if an error occurs.
*/
{
  array2d_t *a = malloc(sizeof(*a));
  if (a == NULL)
  {
#ifndef NDEBUG
    MEM_ERR;
#endif
    return NULL;
  }
  a->shape[0] = shape[0];
  a->shape[1] = shape[1];
  a->order = order;
  a->val = calloc(shape[0] * shape[1], sizeof(*(a->val)));
  if (a->val == NULL)
  {
#ifndef NDEBUG
    MEM_ERR;
#endif
    free(a);
    a = NULL;
  }
  return a;
}

void array2d_fprint(FILE *stream, const array2d_t *a)
/*
  Purpose:

    Prints an two-dimensional array.

  Arguments:
    stream       a pointer to a file stream
    a            a pointer to an array2d_t

  Return value:
    None
*/
{
  {

    if (a == NULL)
      return;

    /* Print array header */
    fprintf(stream, "<array2d_t shape=(%zu,%zu) order=%s>\n", a->shape[0], a->shape[1], a->order == RowMajor ? "RowMajor" : "ColMajor");

    /* Print routine for one- and two-dimensional arrays */
    size_t m = a->shape[0];
    size_t n = a->shape[1];
    size_t st0 = (a->order == RowMajor) ? a->shape[1] : 1;
    size_t st1 = (a->order == RowMajor) ? 1 : a->shape[0];
    for (size_t i = 0; i < m; i++)
    {
      for (size_t j = 0; j < n; j++)
      {
        fprintf(stream, "% 8.3g ", a->val[i * st0 + j * st1]);
      }
      fprintf(stream, "\n");
    }
    return;
  }
}

void array2d_print(const array2d_t *a) { array2d_fprint(stdout, a); }
