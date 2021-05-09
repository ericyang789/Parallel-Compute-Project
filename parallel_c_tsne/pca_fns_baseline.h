#include<stdio.h>
#include <stdlib.h>

// Matrix multiplication
int print=0;

void mat_multiply(int r1, int c1, int r2, int c2,
		     double a[r1][c1],
                      double b[r2][c2],
                      double c[r1][c2]
                      ) {

   // Initializing elements of c to 0.
   for (int i = 0; i < r1; ++i) {
      for (int j = 0; j < c2; ++j) {
         c[i][j] = 0;
      }
   }	
	
   // Multiplying first and second matrices and storing it in c
   //#pragma acc kernels copyin(a,b) copy(c)
//   #pragma omp parallel for default(none) shared(a,b,c) private(i,j,k)
   for (int i = 0; i < r1; ++i) {
      for (int j = 0; j < c2; ++j) {
		float tmp = 0;
         for (int k = 0; k < c1; ++k) {
            tmp += a[i][k] * b[k][j];
         }
		c[i][j] = tmp;
      }
   }
}

//void multiply(int m1, int m2, int mat1[][m2], int n1,
//              int n2, int mat2[][n2])
void multiply(int m1, int m2, int n1, int n2,
		     double mat1[m1][m2], double mat2[n1][n2], double res[m1][n2]
                      )
{
    int x, i, j;
    for (i = 0; i < m1; i++)
    {
        for (j = 0; j < n2; j++)
        {
            res[i][j] = 0;
            for (x = 0; x < m2; x++)
            {
                *(*(res + i) + j) += *(*(mat1 + i) + x)
                                     * *(*(mat2 + x) + j);
            }
        }
    }
	}

// Transposes matrix A and outputs the transpose B
void transpose(int n, int d, double A[n][d], double B[d][n])
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < d; j++)
            B[i][j] = A[j][i];
}

void reduce_to_k(int new_dim, int n, double Vt[n][n], double Vtk[n][new_dim])
{
	for (int i=0; i<n; i++){
		for (int j=0; j<new_dim; j++){
			Vtk[i][j]=Vt[i][j];
	}
	}

}






void subtract_col_means(int d1, int d2, double data[][d2]){
	double col_mean[d2];
	for (int i = 0; i < d2; ++i) {
				col_mean[i] = 0;

	}

	if (print==1) printf(" Calculating means \n");
	for (int i=0; i<d2; i++) {
		for (int j=0; j<d1; j++) {
			col_mean[i]=col_mean[i]+data[j][i];
			}

		col_mean[i]=col_mean[i]/d1;
	}

	if (print==1) printf(" Taking away means \n");
	// Taking mean away from data test_image
	for (int i=0; i<d1; i++) {
		for (int j=0; j<d2; j++) {
			data[i][j]=data[i][j]-col_mean[j];
			}
		}

	}

void calculate_covariance(int d1, int d2, double data[][d2], double data_transpose[][d1], double cov[][d2]){


	// Calculating the covariance matrix S= np.dot(A.T, A)/M


	if (print==1) printf(" Calculating covariance matrix: getting A.T \n");
	// Get A.T

	for (int i = 0; i < d1; i++)
		for (int j = 0; j < d2; j++)
			data_transpose[i][j] = data[j][i];

	
	if (print==1) printf(" Calculating covariance matrix: getting A.T * A \n");
	// Calculate S
	//mat_multiply(N, M, M, N, At, A, S);
	
/*12.23
	#pragma acc data copyin(data_transpose[0:d2][0:d1], data[0:d1][0:d2]) copy(cov[0:d2][0:d2]) 
	#pragma acc kernels
	#pragma acc loop tile(64,64)
	for (int i = 0; i < d2; ++i) {
		for (int j = 0; j < d2; ++j) {
			double tmp = 0;
	#pragma acc loop reduction(+:tmp)
			for (int k = 0; k < d1; ++k) {
				//cov[i][j] += data_transpose[i][k] * data[k][j];
				tmp += data_transpose[i][k] * data[k][j];
			}
		cov[i][j] = tmp;
		}
	}
*/	

/*
	#pragma acc data copyin(data_transpose[0:d2][0:d1], data[0:d1][0:d2]) copy(cov[0:d2][0:d2])
	#pragma acc kernels
	#pragma acc loop tile(4,4)
	for (int i = 0; i < d2; ++i) {
		for (int j = 0; j < d2; ++j) {
			double tmp = 0;
	#pragma acc loop reduction(+:tmp)
			for (int k = 0; k < d1; ++k) {
				//cov[i][j] += data_transpose[i][k] * data[k][j];
				tmp += data_transpose[i][k] * data[k][j];
			}
		cov[i][j] = tmp;
		}
	}
*/


	//#pragma acc kernels copyin(data_transpose[0:d2][0:d1], data[0:d1][0:d2]) copy(cov[0:d2][0:d2])
	//#pragma acc kernels
	//#pragma acc loop independent vector(32)
	for (int i = 0; i < d2; ++i) {
//	#pragma acc loop independent
		for (int j = 0; j < d2; ++j) {
			double tmp = 0;
//	#pragma acc loop reduction(+:tmp)
			for (int k = 0; k < d1; ++k) {
				tmp += data_transpose[i][k] * data[k][j];
			}
		cov[i][j] = tmp;
		}
	}
	if (print==1) printf("d1,d2 = %d, %d", d1, d2);



}
