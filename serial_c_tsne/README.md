# Serial t-SNE implementation

### File structure:

    tsne.c/

            core tsne executable file
            calls pca_fns.h, tsne_fns.h, read_in_data.h or read_csv.h and singular_value_decomposition.c
            works with an M x N array read in from the read_in_data.h file (for ubyte format data) or read_csv.h (for CSV data)
            file calculates an M x L array Y containing the projection of the data onto L dimensions
            uploaded file does so for L=2 dimensions however this can be easily changed to any L<=K where K is reduced PCA dimension
            
    pca_fns.h/
    
            contains the helper function for initial PCA dimensionality reduction step called from the tsne.c file
            the function include:
                    
                    multiply(): computes matrix multiplication of two input arrays
                    transpose(): transposes an input array
                    subtract_col_means(): calculates and subtracts column means for all elements in input array
                    calculate_covariance(): calculates the covariance matrix for an input array
                    reduce_to_k(): reduced the SVD output to K components
                    
    singular_value_decomposition.c/
    
            script to compute SVD of the covariance matrix called withing PCA implementation in tsne.c file
            source: http://www.mymathlib.com/matrices/linearsystems/singular_value.html
         
    
    tsne_fns.h/
    
            contains functions for the execution of the core tsne algorithm which are called withing the tsne.c script
            
    read_in_data.h/
    
            script to read in an M x N array of data saved in the ubyte format called within the tsne.c file
            this was addapted from the following implementation https://github.com/takafumihoriuchi/MNIST_for_C
            path to data file and the dimension (M and N) must be defined at the top of the scrips
            the script is called withing tsne.c using the load_data() function
            data is read into the M x N 'data' array
            
    read_csv.h/
    
            script to read in an M x N array of data saved in a CSV format called within the tsne.c file
            the CSV file must not have any headers
            path to data file and the dimension (M and N) must be defined at the top of the scrips
            the script is called withing tsne.c using the load_data() function
            data is read into the M x N 'data' array
            
                    
