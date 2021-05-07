#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "read_in_data.h"
//#include "read_non_binary.h"
#include "singular_value_decomposition.c"
#include "pca_fns.h"
#include "tsne_fns.h"



//#define M 100
#define M 2500
//#define N 21413
#define N 784
#define K 50
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
double X[M][K];
//double col_mean[N];
double* dummy_array;
int err;

// For pji
double D[M][M];
double pji[M][M];
double sigmas[M];
double P[M][M];

// For q

double Y[M][2];
double Q[M][M];
double grad[M][2];
double target_perplexity=5.0;
double grad_err=1000;
double grad_current_err=0;
double Yold[M][2];
double alpha=0.1;
int grad_iter=0;



// Initialise A - need to take away mean and calculate the covariance matrix
int main(){
	clock_t start, end;

	printf(" Starting to load data \n");

	load_data();

	printf(" Finished loading data \n");

	// Mean vector of size N - collects sum of each of N columns of test_image


	start = clock();

	subtract_col_means(M, N, data);


	printf(" Calculating covariance matrix \n");

	// Calculating the covariance matrix S= np.dot(A.T, A)/M
	for (int i=0; i<N; i++) {
		for (int j=0; j<N; j++) {
			S[i][j]=0;}
		}

	for (int i=0; i<N; i++) {
		for (int j=0; j<M; j++) {
			At[i][j]=0;}
		}

	//printf(" Calculating covariance matrix: getting A.T \n");
	// Get A.T
	//transpose(M, N, A, At);

	//printf(" Calculating covariance matrix: getting A.T * A \n");
	// Calculate S
	//mat_multiply(N, M, M, N, At, A, S);



	calculate_covariance(M, N, data, At, S);


	//multiply(N, M, M , N, At, A, S);

	printf(" Starting SVD \n");
	dummy_array = (double*) malloc(N * sizeof(double));
	if (dummy_array == NULL) {printf(" No memory available\n"); exit(0); }

	err = Singular_Value_Decomposition((double*) data, N, N, (double*) U,
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

	//mat_multiply(M, N, N, K, A, Vtk, R);
	multiply(M, N, N, K, data, Vtk, X);
	printf(" Done \n");

	end = clock()-start;

	double time_taken=((double)end)/CLOCKS_PER_SEC;
	printf("Time taken: %f \n",time_taken);

	// Working with MxD matrix R

	// pji

	for (int i=0; i<M; i++) {
		for (int j=0; j<M; j++) {
			pji[i][j]=0;}
		}

	for (int i=0; i<M; i++) {
		for (int j=0; j<M; j++) {
			D[i][j]=0;
			P[i][j]=0;}
		}

	for (int i=0; i<M; i++) {
		sigmas[i]=0;}

	for (int i=0; i<M; i++) {
		for (int j=0; j<2; j++){
			grad[i][j]=0;
		}
	}

	printf("Starting tsne \n");

	printf("Calc P \n");


	//calc_P(int d1, int k1, double D[][d1], double X[][k1], double sigmas[d1], double pji[][d1], double target_perplexity)
	calc_P(M, K, D, X, sigmas, pji, target_perplexity, P);

	for (int i=0; i<M; i++) {
		for (int j=0; j<2; j++){
			Y[i][j]=normalRandom()*0.0001;
		}
	}

	// Gradient descent

	//KL_dist(int d1, double Y[][2], double P[][d1], double Q[][d1], double grad[][2])

	// update Y using learning rate alpha and absoluted difference for convergence

	printf("Starting gradient descent \n");
	while (grad_err>0.1 && grad_iter<4){
		// updates gradient
		KL_dist(M, Y, P, Q, grad);
		printf("Interation: %d \n",grad_iter);

		for (int i=0; i<5; i++) {
			for (int j=0; j<2; j++){
				printf("%f  ",Y[i][j]);
			}

		}

		printf("\n");

		for (int i=0; i<5; i++) {
			for (int j=0; j<2; j++){
				printf("%f  ",grad[i][j]);
			}

		}

		printf("\n");


		//Ynew=Y-alpha*grad;
		for (int i=0; i<M; i++) {
			for (int j=0; j<2; j++){
				//Updates Y
				Yold[i][j]=Y[i][j];
				Y[i][j]=Y[i][j]-alpha*grad[i][j];
				grad_current_err=fabs(Yold[i][j]-Y[i][j]);

				if (grad_current_err>grad_err){
					grad_err=grad_current_err;}

			}
		}
		grad_iter+=1;

	}

	printf("Interation's taken: %d \n",grad_iter-1);
/*
	for (int i=0; i<M; i++) {
		for (int j=0; j<2; j++){
			printf("%f",Y[i][j]);
		}
		printf("\n");
	}
*/

	end = clock()-start;
	time_taken=((double)end)/CLOCKS_PER_SEC;
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
	return 0;
}
