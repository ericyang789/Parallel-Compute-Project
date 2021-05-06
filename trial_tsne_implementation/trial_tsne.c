#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "read_in_data.h"
#include "singular_value_decomposition.c"
#include "pca_fns.h"



#define M 2500
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

void calc_pji(int d1, double pji[][d1], double D[][d1], double sigmas[d1]){
	// d1=M, pji is MXM, D is MXM, sigmas is M
	double Z=0;
	for (int i=0; i<d1; i++) {
		Z=0;
		for (int j=0; j<d1; j++) {
			if (i!=j){
				pji[i][j]=exp(pow(D[i][j], 2))/ (2 * pow(sigmas[i],2));
				Z+=exp(pow(D[i][j], 2)) / (2 * pow(sigmas[i],2));}}

		for (int k=0; k<d1; k++){
			pji[i][k]=pji[i][k]/Z;
		}
}}

void calc_D(int d1, int k1, double X[][k1], double D[][d1]){
	// X is M X K
	// D is M X M and all 0's
	// Updates D

	double norm=0;
	for (int i=0; i<d1; i++) {
		for (int j=0; j<i; j++) {
			norm=0;
			for (int k=0; k<k1; k++){
				norm+=pow(X[i][k]-X[j][k],2);
			}
			D[i][j]=sqrt(norm);
			D[j][i]=D[i][j];

		}
	}


}



float calc_perplexity_diff(double sigmai, double target_perplexity, int d1, double Di[d1]){
	// d1=main
	// Di is row i of D of size M
	double Z=0;
	double denom=0;
	double pji_var=0;
	double H=0;
	double diff=0;

	denom=2*pow(sigmai,2);

	for (int i=0; i<d1; i++){
		Z+=exp(-pow(Di[i],2)/denom);
	}

	for (int j=0; j<d1; j++){
		pji_var=exp(-pow(Di[j],2)/denom)/Z;
		if (pji_var>0) {
			H-=pji_var*log2(pji_var);
		}
	}

	diff=pow(2,H)-target_perplexity;

	return diff;

}




void calc_sigmas(int d1, double D[][d1], double sigmas[d1], double target_perplexity){
	// sigmas should all be set to 0
	double a=1.0;
	double b=1.0;
	double diff_a;
	double diff_b;
	double Di[d1];
	double iter=0;
	double max_iter=100;
	double max_err=0.001;
	double err=1000;
	double c;
	double cnew;


	for (int i=0; i<d1; i++){
		a=1.0;b=1.0;

		for (int k=0; k<d1; k++){
			Di[k]=D[i][k];
		}

		//calc_perplexity_diff(double sigmai, double target_perplexity, int d1, double Di[d1])
		diff_a=calc_perplexity_diff(a, target_perplexity, d1, Di);
		diff_b=calc_perplexity_diff(b, target_perplexity, d1, Di);

		while (diff_a>=0){
			a=a/2;
			diff_a=calc_perplexity_diff(a, target_perplexity, d1, Di);
		}

		while (diff_b<=0){
			b=b*2;
			diff_b=calc_perplexity_diff(b, target_perplexity, d1, Di);
		}

		c=(a+b)/2;

		while (err>max_err && iter < max_iter){
			if (calc_perplexity_diff(a, target_perplexity, d1, Di)*calc_perplexity_diff(c, target_perplexity, d1, Di) < 0)
      	{
				b=c;}
      else{
				a=c;}

			cnew=(a+b)/2;
			err=fabs(c-cnew);
			c=cnew;
			iter+=1;

		}

		sigmas[i]=c;
	}


}



void calc_P(int d1, int k1, double D[][d1], double X[][k1], double sigmas[d1], double pji[][d1], double target_perplexity, double P[][d1]){
	// Calculate P which is input as all zeros

	// X is M X K
	// D is M X M and all 0's

	// Updates D
	calc_D(d1, k1, X, D);

	// sigmas is size M and should all be set to 0
	// Updates sigmas

	calc_sigmas(d1, D, sigmas, target_perplexity);

	// Update pji - M X M - initialised to 0's
	calc_pji(d1, pji, D, sigmas);

	// Update P - M X M - initialised to 0's

	for (int i=0; i<d1; i++) {
		for (int j=0; j<d1; j++) {
			P[i][j]=(pji[i][j]+pji[j][i])/(2*d1);}
		}



}


void calc_Q(int d1, double Y[][2], double Q[][d1]){
	//Y Mx2 initialised to 0's (TODO: change to random init)
	//Q MXM init to 0's and is updated
	double Z=0.0;
	double norm=0.0;

	for (int i=0; i<d1; i++) {
		for (int j=0; j<i; j++) {
			norm=0.0;
			for (int k=0; k<2; k++){
				norm+=pow(Y[i][k]-Y[j][k],2);}
			Q[i][j]=1.0/(1.0+norm);
			if (i<3 && j<2){printf("In calc_Q Qij: %f \n", Q[i][j]);}
			Q[j][i]=Q[i][j];
			Z+=2*Q[i][j];
		}
		}
	printf("Z: %f \n", Z);
	for (int i=0; i<d1; i++) {
		for (int j=0; j<d1; j++) {
			Q[i][j]=Q[i][j]/Z;

		}
	}



}


void KL_dist(int d1, double Y[][2], double P[][d1], double Q[][d1], double grad[][2]){
	// Y: Mx2
	// P: MxM
	// Q: MxM set to 0's
	// grad: Mx2 set to 0's

	double kldist=0.0;
	double norm=0.0;
	// Updates Q
	calc_Q(d1, Y, Q);

	for (int i=0; i<d1; i++) {
		for (int j=0; j<d1; j++) {
			if (P[i][j]>0){
				kldist+=P[i][j]+log(P[i][j]/Q[i][j]);
			}
		}
	}


	for (int i=0; i<d1; i++) {
		for (int j=0; j<d1; j++) {
			norm=0;

			for (int kk=0; kk<2; kk++){
				norm+=pow(Y[i][kk]-Y[j][kk],2);}


			for (int k=0; k<2; k++){
				grad[i][k]+=(P[i][j]-Q[i][j])*(Y[i][k]-Y[j][k])*(1.0/(1.0+norm));}

			if (i<3 && j<2){printf("Pij: %f \n",P[i][j]);printf("Qij: %f \n",Q[i][j]);}

			} // end of j loop

		for (int kkk=0; kkk<2; kkk++){
			grad[i][kkk]=4.0*grad[i][kkk];}


	}





}


double rand_gen() {
	return (float)rand() / (float)RAND_MAX;
}
double normalRandom() {
   // return a normally distributed random value
   double v1=rand_gen();
   double v2=rand_gen();
   return cos(2.0*3.14*v2)*sqrt(-2.0*log(v1));
}

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
				printf("%f",Y[i][j]);
			}

		}

		printf("\n");

		for (int i=0; i<5; i++) {
			for (int j=0; j<2; j++){
				printf("%f",grad[i][j]);
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

	printf("Interation: %d \n",grad_iter);
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
