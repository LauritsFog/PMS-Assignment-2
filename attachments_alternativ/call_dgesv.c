#include "call_dgesv.h"
#include "datatypes.h"
#include <stdlib.h>
#include <stdio.h>

/* C prototype for LAPACK routine DGESV */
void dgesv_(int *n,    /* columns/rows in A          */
            int *nrhs, /* number of right-hand sides */
            double *A, /* array A                    */
            int *lda,  /* leading dimension of A     */
            int *ipiv, /* pivoting array             */
            double *B, /* array B                    */
            int *ldb,  /* leading dimension of B     */
            int *info  /* status code                */
);

// Function for solving the linear system
int call_dgesv(array2d_t *A, array_t *b)
{

  // Checks if both Matrix A and vector b are not empty
  if (A == NULL || b == NULL)
  {
    return -9;
  }

  // Checks if the Matrix A is square
  if (A->shape[0] != A->shape[1])
  {
    return -10;
  }

  // Checks if vector b is of the same dim as matrix A so the problem can be solved
  if (A->shape[0] != b->len)
  {
    return -11;
  }

  // Initializing
  int m = (int)A->shape[0], n = (int)A->shape[1];
  int nrhs = 1, ldb = m, lda = m, info;
  int ipiv[n];

  if (A->order == ColMajor)
  {
    // dgesv assumes colmajor so we do nothing to the values of A and call the function
    dgesv_(&m, &nrhs, A->val, &lda, ipiv, b->val, &ldb, &info);
  }
  else
  {
    // Initializing vector and dim of vector
    int nm = n * m;
    double xrow[nm];

    // Saving the values of A in a new vector
    for (size_t i = 0; i < nm; i++)
    {
      xrow[i] = A->val[i];
    }
    int k = 0;

    // Rearranges the values of A so its in ColMajor
    for (size_t i = 0; i < n; i++)
    {

      for (size_t j = 0; j < n; j++)
      {
        A->val[k] = xrow[i + n * j];
        k = k + 1;
      }
    }

    // Calling the function
    dgesv_(&m, &nrhs, A->val, &lda, ipiv, b->val, &ldb, &info);
  }

  return info;
}
