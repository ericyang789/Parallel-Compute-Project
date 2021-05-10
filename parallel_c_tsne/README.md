# Parallel t-SNE implementation

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
            the same sript as serial_c_tsne/tsne_fns.h except with the additions of acc pragmas.
            
    tsne_fns_omp.h/
    
            contains functions for the execution of the core tsne algorithm which are called withing the tsne.c script
            the same sript as serial_c_tsne/tsne_fns.h except with the additions of OpenMP pragmas.
            
    read_in_data.h/
    
            script to read in an Ms x Ns array of data saved in the ubyte format called within the tsne.c file
            this was addapted from the following implementation https://github.com/takafumihoriuchi/MNIST_for_C
            path to data file and the dimension (Ms and Ns) must be defined at the top of the scrips
            the script is called withing tsne.c using the load_data() function
            data is read into the Ms x Ns 'data' array
            
    read_csv.h/
    
            script to read in an Mr x Nr array of data saved in a CSV format called within the tsne.c file
            the CSV file must not have any headers
            path to data file and the dimension (Mr and Nr) must be defined at the top of the scrips
            the script is called withing tsne.c using the load_data() function
            data is read into the Mr x Nr 'data' array
            
                    
                    
   ### Running the script
   
    Adaptations to be made to the files prior to running:
    
            Read in file: 
                    
                    depending on the format the data was saved in either the read_csv.h or read_in_data.h file must be included at the top of tsne.c 
                    both read in files have been defined in such a way that no other parts of tsne.c need to be changed
                    
                    within the chosen read in file you must change the path to the data location (default data folder one directory up)
                    dimensions of the data to be read in must also be defined (Mr and Ms =number of rows, Nr and Ns =number of columns)
    
            Define dimensions: 
            
                    dimensions of the input data array must be defined in both the chosen read in file and the tsne.c script
                    M = number of rows
                    N = number of columns (features)
                    K: dimensions of PCA used must also be defined
                    
            Parameter tunning:
            
                    The script implements both the bisection method and gradient descent
                    The error and maximum number of iterations can be set for these in the tsne.c file
                    The learning rate alpha for gradient descent can also be set 
                    The target_perplexity variable for tSNE can modified at the top of the tsne.c script
                    
              
                    
  ### Compiling and running code
  
    For acc parallelization the script can be compiled with pgcc by running
    
            pgcc -acc -Minfo tsne.c -o tsne
            
    And run using
    
            ./tsne
            
    The file prints out statements of the current execution steps as well as the execution times for reference.
    The speedups for acc parallelization are discussed in the main README.
    
    We also attempted OpenMP parallelization but did not yield fruitful results as discussed in the main README.
    If you want to test out OpenMP parallelization:
            
            First change the #include "tsne_fns.h" in tsne.c to tsne_fns_omp.h.
            
    Then compile by running
            
            pgcc -mp tsne.c -o tsne
    
    And run using
    
            ./tsne
  
    One can expect these results if running the OpenMP parallelization with the AWS instance type specified in the main README.
      M: 2500
      N: 784
      Starting to load data 
      Finished loading data 
      Calculating covariance matrix 
      Starting SVD 
      Calculating first K columns of V 
      Projecting onto K dims of V 
      Done 
      Time (PCA): 26.118728 
      Starting tsne 
      Starting gradient descent 
      Interation's taken: 274 
      Time (TSNE): 91.534762 
      Time Total (PCA + TSNE): 117.653525
      
      
      
      
      
