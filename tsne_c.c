#include<stdio.h>
#include "mnist.h"
#include "pca.h"
#include "Hbeta.h"
#include "x2p.h"
/* This is a prototype C implementation of tsne*/

/* Main function conduct the primary execution and is
 * equivalent to if __name__ == "__main__": in python */

void pca(int n, int *d, double X[n][*d], int initial_dims){
	*d=initial_dims;
	printf("n: %d\n", n);
	printf("d: %d\n", *d);

}

void tsne(int n, int d, int no_dims, int initial_dims, double perplexity, double X[n][d], double Y[n][no_dims])
{
     int i;

     /* will update X and change its dim */
     printf("d: %d\n", d);
     pca(n,&d,X, initial_dims);

     printf("d: %d\n", d);



     for (i=0; i<no_dims; i++) {
         Y[0][i]=X[0][i];
     }




     
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

    int n=2500;

    /* initial dims */
    int d=784; 
    int no_dims=2;
    int initial_dims=50;
    double perplexity=30.0;

    double Y[n][no_dims];
    tsne( n, d,no_dims,initial_dims, perplexity,test_image,Y);
    int j;
    for (j=0; j<no_dims; j++) {
        printf("%1.1f ", Y[0][j]);
        if ((j+1) % 28 == 0) putchar('\n');
    }

    return 0;

    
}
