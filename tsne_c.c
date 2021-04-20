#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mnist.h"
#include "pca.h"
#include "Hbeta.h"
#include "x2p.h"

/* This is a prototype C implementation of tsne*/

/* Main function conduct the primary execution and is
 * equivalent to if __name__ == "__main__": in python */

/*void pca(int n, int *d, double X[n][*d], int initial_dims){
	*d=initial_dims;
	printf("n: %d\n", n);
	printf("d: %d\n", *d);

}*/

void tsne(int n, int d, int no_dims, int initial_dims, double X[n][d])
{

     /* Initialize variables */

     double perplexity=30.0;
     int max_iter = 1000;
     double initial_momentum = 0.5;
     double final_momentum = 0.8;
     int eta = 500;
     double min_gain = 0.01;
     double tol = 0.00001;

     double Y[n][no_dims];
     double dY[n][no_dims];
     double iY[n][no_dims];
     double gains[n][no_dims];

     /* Initialise Y, dY, iY, gains */

     int i, j;
     

     for (i = 0; i<n; i++){

          for (j = 0; j<no_dims; j++){
             Y[i][j] = (float)rand() / (float)RAND_MAX;
             dY[i][j] = 0;
             iY[i][j] = 0;
             gains[i][j] = 1;}
     }



     /* will update X and project it onto initial_dims dimensions*/  
     /* n won't be updated (no samples), d will be updated to initial_dims */

     printf("(n,d): (%d , %d) \n", n,d);

     pca(n,&d,X, initial_dims);

     printf("(n,d): (%d , %d) \n", n,d);


     for (i = 0; i<n; i++)
     {
          for (j = 0; j<no_dims; j++)
               printf("%1.1f ", Y[i][j]);
               if ((j+1) % 28 == 0) putchar('\n');
     }


     /*Compute P-values*/

     x2p(n,d,X, tol, perplexity);

     /*P = x2p(n,d,X, tol, perplexity)
     P = P + np.transpose(P)
     P = P / np.sum(P)
     P = P * 4.									
     P = np.maximum(P, 1e-12)*/




     
}



int main()
{
    printf("\nPerforming t-SNE on your dataset\n\n");
    printf("\nRunning example of 2,500 MNIST digits...\n");

    // call to store mnist in array - give test_image size 2500 by 784
    // and test_label size 2500
    load_mnist();

    // print pixels of first data in test dataset
    int i;
    for (i=0; i<784; i++) {
        printf("%1.1f ", test_image[0][i]);
        if ((i+1) % 28 == 0) putchar('\n');
    }

    // print first label in test dataset
    printf("label: %d\n", test_label[0]);

    /* initial dims of X */
    int n=2500;
    int d=784; 

    /* tsne dim */
    int no_dims=2;

    /* initial PCA reduction dim */
    int initial_dims=50;



    tsne( n, d, no_dims, initial_dims, test_image);

    /*tsne( n, d, no_dims, initial_dims, perplexity, test_image, Y);
    int j;
    for (j=0; j<no_dims; j++) {
        printf("%1.1f ", Y[0][j]);
        if ((j+1) % 28 == 0) putchar('\n');
    }*/

    return 0;

    
}

