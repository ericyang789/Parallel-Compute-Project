#include<stdio.h>
#include <stdlib.h>
#include "read_in_data.h"
#include "pca.h"
//#define M 2500 //number of samples
#define nsamp 2500
//#define N 784 //number of cells
#define ncell 784
//#define K 50 // no reduced dimensions
// Input matrix will be M x N

int const no_dims_reduced=50;
int const d1=2500;
int d2=784;
int pca_res;
double reduced_data[d1][no_dims_reduced];
//
/* This is a prototype C implementation of tsne*/
int main(){
  printf(" Loading data \n");
  // Step 1: Read in data

  //Load data into data
  load_data();


  // Step 2: Perform initial PCA step
  printf(" Starting PCA \n");
  pca_res=pca(d1, d2, no_dims_reduced, data, reduced_data);





  return 0;
}
