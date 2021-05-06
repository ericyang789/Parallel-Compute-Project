
#include <math.h>
/* Performs a binary search to get P-values in such a way that each
conditional Gaussian has the same perplexity. */

void mat_multiply(int r1, int c1, int r2, int c2,
		     double a[r1][c1],
                      double b[r2][c2],
                      double c[c1][r2]
                      ) {

   // Initializing elements of c to 0.
   for (int i = 0; i < r1; ++i) {
      for (int j = 0; j < c2; ++j) {
         c[i][j] = 0;
      }
   }

   // Multiplying first and second matrices and storing it in c
   for (int i = 0; i < r1; ++i) {
      for (int j = 0; j < c2; ++j) {
         for (int k = 0; k < c1; ++k) {
            c[i][j] += a[i][k] * b[k][j];
         }
      }
   }
}

// Transposes matrix A and outputs the transpose B
void transpose(int n, int d, double A[n][d], double B[d][n])
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < d; j++)
            B[i][j] = A[j][i];
}
void x2p(int n, int d, double X[n][d], double tol, double perplexity){

	printf("Computing pairwise distances\n");

	/*Sum element wise square of X by columns*/
	double sum_X[n][1];
	double result[n][n];
/*
	int i, j;

	for (i = 0; i<n; i++){

		for (j = 0; j<d; j++){
			sum_X[i][0] = sum_X[i][0]+X[i][j]*X[i][j];}

		}

	double Xt[d][n];

	// Calculates transpose of X, populates Xt
	transpose(n, d, X, Xt);

	// Calculates matrix multiplication of X and Xt
	// Populates result with the multiplication

	mat_multiply(n, d, d, n,X, Xt,result);

	// Add sum_X to each row of result

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			result[i][j]=-2*result[i][j]+ sum_X[j][0];
      		}
   	}

	// Transpose result

	double resultT[n][n];

	transpose(n, n, result, resultT);

	// Add sum_X to each row of resultT

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			resultT[i][j]=resultT[i][j]+ sum_X[j][0];
      		}
   	}

	double P[n][n];
	double beta[n][1];

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			P[i][j]=0;
      		}
   	}

	for (int i = 0; i < n; ++i) {
		beta[i][0]=1;
   	}

	double logU=log(perplexity);*/








}
