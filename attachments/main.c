#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"

array2d_t *array2d_alloc(const size_t shape[2], enum storage_order order);
array2d_t *array2d_from_file(const char *filename);

void array2d_fprint(FILE *stream, const array2d_t *a);

array_t *array_alloc(const size_t capacity);
array_t *array_from_file(const char *filename);

void array_fprint(FILE *stream, const array_t *a);

int call_dgesv(array2d_t *A, array_t *b);
array_t call_dgesv_sol_out(array2d_t *A, array_t *b);

void dgesv_(const int *n,    /* columns/rows in A          */
            const int *nrhs, /* number of right-hand sides */
            double *A,       /* array A                    */
            const int *lda,  /* leading dimension of A     */
            int *ipiv,       /* pivoting array             */
            double *B,       /* array B                    */
            const int *ldb,  /* leading dimension of B     */
            int *info        /* status code                */
);

int main(void)
{
    array2d_t *a = array2d_from_file("./msptools/data/A2.txt");
    array_t *b = array_from_file("./msptools/data/b2.txt");

    array2d_fprint(stdout, a);
    array_fprint(stdout, b);

    int n = a->shape[0];
    int piv[n];
    int info;
    int nrhs = 1;

    dgesv_(&n,     // n
           &nrhs,  // nrhs
           a->val, // A
           &n,     // lda
           piv,    // ipiv
           b->val, // B
           &n,     // ldb
           &info   // info - ??
    );

    array_fprint(stdout, b);

    return 0;
}