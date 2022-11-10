#include <stdlib.h>
#include <stdio.h>
#include "datatypes.h"

/* C prototype for LAPACK routine DGELS */
void dgels_(
	const char * trans,  /* 'N' or 'T'             */
	const int * m,       /* rows in A              */
	const int * n,       /* cols in A              */
	const int * nrhs,    /* cols in B              */
	double * A,          /* array A                */
	const int * lda,     /* leading dimension of A */
	double * B,          /* array B                */
	const int * ldb,     /* leading dimension of B */
	double * work,       /* workspace array        */
	int * lwork,         /* workspace size         */
	int * info           /* status code            */
);
/* call_dgels : wrapper for LAPACK's DGELS routine

Purpose:
Solves the least-squares problem

   minimize  || A*x-b ||_2^2

using LAPACK's DGELS routine. Upon exit, the input vector b is overwriten, i.e.,
the leading n elements of b contain the solution x.

Return value:
The function returns the output `info` from DGELS with the
following exceptions: the return value is

	-12 if the input A is NULL and/or the input b is NULL
	-13 if A->shape[0]<A->shape[1]
	-14 if the dimensions of $A$ and $b$ are incompatible
	-15 in case of memory allocation errors.
*/

int call_dgels(array2d_t * A, array_t * b) {
	//Validation of input
	if(!A|!b) {
		fprintf(stderr, "Error: A or b is a NULL pointer \n");
		return -12;
	}

	if(A->shape[0] < A->shape[1]) {
		fprintf(stderr, "Error: The number of rows in A must be "
		"greater than or equal to the number of columns in A \n");
		return -13;
	}

	if(A->shape[0]!=b->len) {
		fprintf(stderr, "Error: The number of rows in A must be"
		"equal to the number of columns in A \n");
		return -14;
	}
	
	// initialisation
	int m = (int) A->shape[0], n = (int) A->shape[1];
	int nrhs = 1, ldb=m, lda=m, info, lwork;
	char trans = 'N';
	double*work;
	double wkopt;

	// calculate optimal work size
  	lwork = -1;
	dgels_(&trans,&m,&n,&nrhs,A->val,&lda,b->val,&ldb,&wkopt,&lwork,&info);
	lwork = (int)wkopt;
    
	// alocate array for work
	work = (double*)malloc( lwork*sizeof(double) );
	if(!work) {
		fprintf(stderr, "Error: Failed to allocate memory\n");
		return -14;
	}
	
	// solve system depending on whether A is row- or colmajor
	if(A->order == ColMajor){
		dgels_(&trans,&m,&n,&nrhs,A->val,&lda,b->val,&ldb,work,&lwork,&info);
	}
	else {
		char trans = 'T'; lda=n;
		dgels_(&trans,&n,&m,&nrhs,A->val,&lda,b->val,&ldb,work,&lwork,&info);
	}

	//deallocate memory in 'work' 
	free( work );
	
	//if dgels_ was succesful the length of b is adjusted to n
	if (info == 0){
		b->len = n;
	}
	else if (info > 0){
		fprintf(stderr, "Error: A does not have full rank\n");
	}
	else {
		fprintf(stderr, "Error: Parameter numper %d of dgels_ had an illigal value\n", -info);
	}

	return info;	
}