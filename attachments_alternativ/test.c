#include <stdlib.h>
#include <stdio.h>
#include "datatypes.h"

int call_dgesv(array2d_t *A, array_t *b);

array2d_t *array2d_alloc(const size_t shape[2], enum storage_order order);
array2d_t *array2d_from_file(const char *filename);

void array2d_fprint(FILE *stream, const array2d_t *a);

array_t *array_alloc(const size_t capacity);
array_t *array_from_file(const char *filename);

void array_fprint(FILE *stream, const array_t *a);

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
    array2d_t *A = array2d_from_file("./msptools/data/A2.txt");
    array_t *b = array_from_file("./msptools/data/b2.txt");

    array2d_fprint(stdout, A);
    array_fprint(stdout, b);

    int m = (int)A->shape[0], n = (int)A->shape[1];
    int nrhs = 1, ldb = m, lda = m, info;
    int ipiv[n];

    if (A->order == ColMajor)
    {
        dgesv_(&m, &nrhs, A->val, &lda, ipiv, b->val, &ldb, &info);
    }
    else
    {
        int nm = n * m;
        double xrow[nm];

        for (size_t i = 0; i < nm; i++)
        {
            xrow[i] = A->val[i];
        }
        int k = 0;

        for (size_t i = 0; i < n; i++)
        {

            for (size_t j = 0; j < n; j++)
            {
                A->val[k] = xrow[i + n * j];
                k = k + 1;
            }
        }

        dgesv_(&m, &nrhs, A->val, &lda, ipiv, b->val, &ldb, &info);
    }

    array_fprint(stdout, b);

    int call_dgesv_out = 0;

    call_dgesv_out = call_dgesv(A, b);

    printf("%d", call_dgesv_out);

    return 0;
}