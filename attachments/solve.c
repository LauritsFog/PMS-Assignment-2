#include <stdlib.h>
#include <stdio.h>
#include "msptools.h"

/* C prototype for LAPACK routine DGESV */
void dgesv_(int *n,    /* columns/rows in A          */
            int *nrhs, /* number of right-hand sides */
            double *A,       /* array A                    */
            int *lda,  /* leading dimension of A     */
            int *ipiv,       /* pivoting array             */
            double *B,       /* array B                    */
            int *ldb,  /* leading dimension of B     */
            int *info        /* status code                */
);


int call_dgesv(array2d_t * A, array_t * b);

int main(int argc, char *argv[]) {

  
    if (argc != 4) {
        fprintf(stderr,"Usage: %s A b x\n", argv[0]);
        return EXIT_FAILURE;
    }

    array2d_t *A = array2d_from_file(argv[1]);
    array_t *b = array_from_file(argv[2]);

    if (!A) {
        fprintf(stderr,"Error reading file %s\n",argv[1]);
        return EXIT_FAILURE;
    }
    if (!b) {
        fprintf(stderr,"Error reading file %s\n",argv[2]);
        return EXIT_FAILURE;
    }

    if (A->shape[0] != A->shape[1]){
         fprintf(stderr,"Error matrix A is not square");
         return EXIT_FAILURE;
    }

    if (A->shape[0] != b->len){
        fprintf(stderr,"Error matrix A and vector b not compatible");
        return EXIT_FAILURE;
    }

    int info = call_dgesv(A, b);

    if (info != 0)
    {
      return EXIT_FAILURE;
    }
    
    array_to_file(argv[3], b);

  return EXIT_SUCCESS;
}


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