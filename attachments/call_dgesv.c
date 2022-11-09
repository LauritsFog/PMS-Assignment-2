#include "datatypes.h"

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

/* call_dgesv : wrapper for LAPACK's DGESV routine

Purpose:
Solves system of equations A*x=b using LAPACK's DGESV routine
Upon exit, the input vector b is overwriten by the solution x.

Return value:
The function returns the output `info` from DGESV with the
following exceptions: the return value is

   -9 if the input A is NULL and/or the input B is NULL
   -10 if A is not a square matrix
   -11 if the dimensions of A and b are incompatible
   -12 in case of memory allocation errors.
*/
int call_dgesv(array2d_t * A, array_t * b) {
  
  if (A == NULL || b == NULL){
    return -9;
  }

  if (A->shape[0] != A->shape[1]){
    return -10;
  }

  if (A->shape[0] != b->len){
   return -11;
  }


  int n = A->shape[0];
  int piv[n];
  int info;
  int nrhs = 1;

  

  dgesv_( &n,            // n 
          &nrhs,        // nrhs 
          A->val,       // A   
          &n,           // lda 
          piv,         // ipiv 
          b->val,       // B 
          &n,           // ldb 
          &info         // info - ??
  );

// Ingen ide om hvorfor det her lort ik virker...

return info;

}