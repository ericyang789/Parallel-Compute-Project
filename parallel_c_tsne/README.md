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
    
### A note on OpenMP 
     
    We also attempted OpenMP parallelization but did not yield fruitful results as discussed in the main README.
    If you want to test out OpenMP parallelization:
            
            First change the #include "tsne_fns.h" in tsne.c to tsne_fns_omp.h.
            
    Then compile by running
            
            pgcc -mp tsne.c -o tsne
    
    And run using
    
            ./tsne
  
  
      Below are some regions in the t-SNE code we attempted to parallelize:
      
<img width="637" alt="calc_perplexity_omp" src="https://user-images.githubusercontent.com/44482565/117675285-03880000-b1df-11eb-812c-082f64ae1cf7.png">
      
      One region is the for loop within our calc_perplexity_diff() function which calculates the perplexity for a distance matrix row given sigma. We defined a pragma omp parallel directive in order to allow the calculations to be distributed and performed independently by several cores. We also defined which variables are to be shared or private. 

<img width="549" alt="calc_sigmas_omp" src="https://user-images.githubusercontent.com/44482565/117675457-29150980-b1df-11eb-8b0a-28e1460ca5da.png">
      
      Another region was the while loops within our calc_sigmas() function. We defined a pragma omp parallel directive in order to allow both while loops to be performed simulatenously and independently across different cores. 
      

    One can expect these results for the given parameters below if running the OpenMP parallelization with the AWS instance type specified in the main README and with 4 cores.
    
    M: 2500
    N: 784
    Starting to load data 
    Finished loading data 
    Calculating covariance matrix 
    Starting SVD 
    Calculating first K columns of V 
    Projecting onto K dims of V 
    Done 
    Time (PCA): 26.446425 
    Starting tsne 
    Starting gradient descent 
    Time (TSNE): 94.025792 
    Time Total (PCA + TSNE): 120.472271 
      
On the other hand the results from running the same code in serial are given below: 

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
    
As can be seen, there was no improvement in runtime. In fact, employing more cores actually resulted in longer runtimes as a result of communication and synchronizaation overheads. 
      
