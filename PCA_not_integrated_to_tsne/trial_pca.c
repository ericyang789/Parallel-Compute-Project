#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include "read_non_binary.h"
#include "singular_value_decomposition.c"



#define M 2500                                                            
#define N 784
#define K 784                                                           
double A[M][N];
double Recon[M][N];
double At[N][M];
double S[N][N];                                                          
double U[N][N];                                                        
double V[N][N];                                                        
double singular_values[N];       
double Vt[N][N];  
double Vtk[N][K];   
double Vtkt[K][N];   
double R[M][K];  
double col_mean[N];                                 
double* dummy_array; 
int err;

// Matrix multiplication
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




  
// Initialise A - need to take away mean and calculate the covariance matrix                                               
int main(){
clock_t start, end;

printf(" Starting to load data \n");

load_mnist();

printf(" Finished loading data \n");

// Mean vector of size N - collects sum of each of N columns of test_image


start = clock();

printf(" Calculating means \n");
for (int i=0; i<N; i++) {
	for (int j=0; j<M; j++) {
		col_mean[i]=col_mean[i]+test_image[j][i];
		}

	col_mean[i]=col_mean[i]/M;

}    


printf(" Taking away means \n");
// Taking mean away from data test_image
for (int i=0; i<M; i++) {
	for (int j=0; j<N; j++) {
		A[i][j]=test_image[i][j]-col_mean[j];
		
	}

}   

 
printf(" Calculating covariance matrix \n");
// Calculating the covariance matrix S= np.dot(A.T, A)/M

// Get A.T
transpose(M, N, A, At);

// Calculate S
mat_multiply(N, M, M, N, At, A, S);




printf(" Starting SVD \n");                                                                                             
dummy_array = (double*) malloc(N * sizeof(double));                    
if (dummy_array == NULL) {printf(" No memory available\n"); exit(0); } 
                                                                          
err = Singular_Value_Decomposition((double*) A, N, N, (double*) U,     
                              singular_values, (double*) V, dummy_array);   
                                                                           
free(dummy_array);

printf(" Finished SVD \n");    
                                                     
if (err < 0) printf(" Failed to converge\n");                          
	else { printf(" The first 20 singular values of A are \n");   }


for (int i = 0; i<20; i++){
               printf("%1.1f ", singular_values[i]);
	       printf("\n");

}




// Want first k singular values and first k columns of V

// Reduced V = N X N to K cols with N rows in Vtk Vtk = N X K

printf(" Calculating first K columns of V \n");   

reduce_to_k(K, N, V, Vtk);

// Want to project A onto K dims ie. R=A*V.T[:,0:k] R = M X K

printf(" Projecting onto K dims of V \n");   

mat_multiply(M, N, N, K, A, Vtk, R);

printf(" Done \n");   

end = clock()-start;

double time_taken=((double)end)/CLOCKS_PER_SEC;
printf("Time taken: %f \n",time_taken);

/* Uncomment to produce the reconstruction Recon and intermediate Vtkt K X N
transpose(N, K, Vtk, Vtkt);
mat_multiply(M, K, K, N, R, Vtkt, Recon);*/



/*FILE *fp;
fp=fopen("test", "rb");
for(int i=0;i<10;i++) {
	for(int j=0;j<10;j++) {
    		fprintf(fp,"%f ",R[i][j]);
	}
	fprintf(fp,"\n");}*/

/*for(int i=0;i<10;i++) {
	for(int j=0;j<10;j++) {
    		printf("%f ",R[i][j]);
	}
	printf("\n");}

for(int j=0;j<784;j++) {
    		printf("%f ",Recon[100][j]);
	}*/
} 

