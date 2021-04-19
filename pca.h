#include <stdio.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_blas.h>

/*Implements pca on X with dims n x d to reduce to n x initial_dims*/

gsl_matrix* pca(int n, int *d, const gsl_matrix * X, int initial_dims){
	*d=initial_dims;
	printf("n: %d\n", n);
	printf("d: %d\n", *d);
	
	unsigned int i;
    	unsigned int rows = X->size1;
   	unsigned int cols = X->size2;

	printf("rows: %d\n", rows);
	printf("cols: %d\n", cols);
	gsl_vector* mean = gsl_vector_alloc(rows);
	
	
	printf("Compute mean.\n\n");
    	for(i = 0; i < rows; i++) {
        gsl_vector_set(mean, i, gsl_stats_mean(X->data + i * cols, 1, cols));
    	}

    	printf("Get mean-substracted data into matrix mean_substracted_data.\n\n");
    	gsl_matrix* mean_substracted_data = gsl_matrix_alloc(rows, cols);
    	gsl_matrix_memcpy(mean_substracted_data, X);
    	for(i = 0; i < cols; i++) {
        gsl_vector_view mean_substracted_point_view = gsl_matrix_column(mean_substracted_data, i);
        gsl_vector_sub(&mean_substracted_point_view.vector, mean);
    	}
    	gsl_vector_free(mean);

    	printf("Compute Covariance matrix.\n\n");
    	gsl_matrix* covariance_matrix = gsl_matrix_alloc(rows, rows);
    	gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0 / (double)(cols - 1), mean_substracted_data, mean_substracted_data, 0.0, covariance_matrix);
    	gsl_matrix_free(mean_substracted_data);

    	printf("Get eigenvectors, sort by eigenvalue.\n\n");
    	gsl_vector* eigenvalues = gsl_vector_alloc(rows);
    	gsl_matrix* eigenvectors = gsl_matrix_alloc(rows, rows);
    	gsl_eigen_symmv_workspace* workspace = gsl_eigen_symmv_alloc(rows);
    	gsl_eigen_symmv(covariance_matrix, eigenvalues, eigenvectors, workspace);
    	gsl_eigen_symmv_free(workspace);
    	gsl_matrix_free(covariance_matrix);

    	printf("Sort the eigenvectors.\n\n");
    	gsl_eigen_symmv_sort(eigenvalues, eigenvectors, GSL_EIGEN_SORT_ABS_DESC);
    	gsl_vector_free(eigenvalues);

    	printf("Project the original dataset.\n\n");
    	gsl_matrix* result = gsl_matrix_alloc(*d, cols);
    	gsl_matrix_view L_eigenvectors = gsl_matrix_submatrix(eigenvectors, 0, 0, rows, *d);
    	gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, &L_eigenvectors.matrix, X, 0.0, result);
    	gsl_matrix_free(eigenvectors);

    	printf("Result is n LxN matrix, each column is the original data vector with reduced dimension from M to L.\n\n");
	unsigned int rows_r = result->size1;
   	unsigned int cols_r = result->size2;

	printf("result rows: %d\n", rows_r);
	printf("result cols: %d\n", cols_r);
	

    	return result;

}
