# Parallel t-SNE implementation

### File structure:
    
    tsne_fns.h/
    
            contains functions for the execution of the core tsne algorithm which are called withing the tsne.c script
            the same sript as serial_c_tsne/tsne_fns.h except with the additions of acc pragmas.
            
    tsne_fns_omp.h/
    
            contains functions for the execution of the core tsne algorithm which are called withing the tsne.c script
            the same sript as serial_c_tsne/tsne_fns.h except with the additions of OpenMP pragmas.
            
    see serial_c_tsne/ folder for the remaining file descriptions here.
    
  ### Running the script
   
    see serial_c_tsne/ folder for the parameter considerations.
                         
                    
  ### Compiling 
  
    For acc parallelization the script can be compiled with pgcc by running
    
            pgcc -acc -Minfo tsne.c -o tsne
            
    And run using
    
            ./tsne
            
    The file prints out statements of the current execution steps as well as the execution times for reference.
    The expected speedups for acc parallelization are discussed in the main README.
    
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
      
      
      
      
      
