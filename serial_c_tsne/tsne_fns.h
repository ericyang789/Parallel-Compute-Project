#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
			Q[j][i]=Q[i][j];
			Z+=2*Q[i][j];
		}
		}

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
