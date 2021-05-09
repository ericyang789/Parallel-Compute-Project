# Parallelized t-SNE for Dimension Reduction 

## Model Description: t-SNE
t-Distrubuted Stochastic Neighbor Embedding (t-SNE) is a popular dimensionality reduction technique for visualizing large, high-dimensional datasets in low-dimensional (usually 2) space [1]. The core idea of t-SNE is that each data point is assigned an ordered preference of a small number of other data points in which it will consider as neighbors. The original high-dimensional space is reduced to a low-dimensional embedded space, where the datapoints are mapped to preserve such neighborhood preference and order. The number of points that a data point considers as neighbors is known as perplexity, a concept derived from information theory and is typically passed in as a tunable parameter when utilizing the algorithm. In t-SNE, the probability of a neighborhood of each data point in the original high-dimensional space is a spherical Gaussian. In the embedded low-dimensional space, such probability is a Student t distribution with one degree of freedom, hence the “t” in t-SNE. With this approach, neighborhood relationships are preserved during embedding as opposed to absolute distances. Therefore, t-SNE generates aesthetically pleasing embedding plots and should not be used for clustering as embedded distances are not meaningful.

Diving in more detail of the t-SNE algorithm, we start out with a vector of M data points, where each data point is a vector with N dimensions. In the original implementation of the t-SNE algorithm, principal component analysis (PCA) is used to preliminarily reduce the dimensionality of each data point by prioritizing dimensions (~50 dimensions) that captures the most variance between datapoints. After PCA, tSNE’s novelty begins, were the pairwise Euclidean distances between data points with reduced dimension are calculated. Using the distances in this space, for each data point i, probabilities (p) of point i considering each point j as a neighbor are defined. To do this, a sigma value is fitted for each point i to achieve the global perplexity value that is passed in as a parameter. Conceptually, as sigma approaches zero, only the one closest neighbor is ever considered as a neighbor. Conversely, as sigma approaches infinity, the Gaussian becomes increasingly spread-out, allowing point i to consider more points j as neighbors. In the desired 2-dimensional space, the probabilities (q) of point i considering each point j as a neighbor is also defined. Given the two probability distributions p and q in different dimensional spaces, an objective function known as the Kullback-Leibler (KL) divergence is implemented to maximize the similarity between the p and q probability distributions. To minimize the KL loss function, numerical optimization techniques are used with the gradient information derived. It is of note that the global optimum is not guaranteed to be reached as the optimization is dependent on initial random initializations. In summary, given M data points in N dimensional space and a chosen perplexity, t-SNE returns the original M data points in 2-dimensional t-SNE space. More details and the rationale of the algorithm can be found in the original paper published by Van der Maaten [1].

## Dataset and Application Description: t-SNE in Single-cell Transcriptomics

The t-SNE algorithm is especially popular in the field of single-cell transcriptomics. DNA encodes the instructions to make all the machinery in human bodies. Depending on different stages of life or disease states, different cells carry out these instructions at different rates. This process is known as transcriptomics. In a typical workflow, cells are obtained from tissues, and ribonucleic acid (RNA) molecules are extracted. RNA expression levels are measured via genomic sequencing techniques and serve as biomarkers that capture a cell’s state and function at a particular timepoint [2]. Through understanding the RNA expression profile for each cell, how much the cells are carrying out these DNA instructions can be understood. These experiments result in a large gene expression matrix, where how much each cell has expressed a certain region of our genome is quantified. Typically, these large matrices are represented where the rows contain the genes (on the order of 30,000 for humans) and the columns contain cells (ranging from hundreds to tens of thousands depending on the experiment). Each value in the matrix can be thought of as counts and each cell(M) is considered a data point in the dimensional space defined by the number of genes (N). Because of this large resultant matrix of RNA expression profiles, t-SNE is very beneficial to reduce the dimensionality and help visualize each individual cell in the embedded space. In our particular application, we used a dataset where the RNA expression levels of pancreatic cells of healthy individuals and patients with type 2 diabetes are profiled [3]. The full dataset contains 3229 cells of various types with 27,539 genes profiled for RNA expression. A subset of the matrix is uploaded to GitHub as 'single_cell_data_norm.csv' to fit the GitHub 100 MB storage limit. Here is an example of a visualization of different cell types in t-SNE space based on genomic expression profiles, where each axis represents a t-SNE reduced dimension [3]:

![image](https://user-images.githubusercontent.com/59083836/117291964-35792980-aea2-11eb-9a07-8799e662e656.png)

## C Implementation of t-SNE and the Need for Big Compute:
At its core, t-SNE involved a tremendous amount of matrix operations, especially given the size of gene expression matrices (order of hundreds or thousands of cells by tens of thousands of genes). In the PCA component of t-SNE, the numerical complexity of the sequential part grows by O(N squared), where N is the number of genes in our matrix while the parallel part grows by O(N cubed). The remainder of the t-SNE algorithm additionally grows by (X squared), where X is the number of values in our N by M (number of cells) matrix. It is estimated that for a dataset size X of 25,000 data matrix values, a single core t-SNE run can take around 800 minutes [4]. Given gene expression matrices that can contain millions of values, a parallel implementation of t-SNE can significantly reduce computation time. In addition, currently, there is no C implementation of t-SNE that exists. We hypothesize that a successful parallel implementation of t-SNE in C would benefit the dimensionality reduction community, by providing customizable parallelizability in a compiler programming language with a multitude of parallelization infrastructures available. 

## Previous work
Python sklearn    
CUDA version (+plot), they only allowed for 2-D, we allow any-D    
![image](https://github.com/CannyLab/tsne-cuda/blob/master/docs/simulated_speedup.png)

## Platform, Application and Programming Model
AWS    
OpenACC    
Many-core, GPU accelerated computing    

## Code Profiling and Parallel Implementation
Explain the code/algorithm    
Code profiling    
Decisions taken    
Pragmas etc    
![image](./figures/pca_no_tmp2.png)
![image](./figures/pca_tmp2.png)
![image](./figures/pca_tmp_acc2.png)

## How to run the code
Python details    
AWS details    
AWS installation of libraries (Lab 5)
Commands to run      

Replicability Information: 
Instance type: g3.4xlarge

CUDA Driver Version:           11030
NVRM version:                  NVIDIA UNIX x86_64 Kernel Module  465.19.01  Fri Mar 19 07:44:41 UTC 2021

Device Number:                 0
Device Name:                   NVIDIA Tesla M60
Device Revision Number:        5.2
Global Memory Size:            7988903936
Number of Multiprocessors:     16
Concurrent Copy and Execution: Yes
Total Constant Memory:         65536
Total Shared Memory per Block: 49152
Registers per Block:           65536
Warp Size:                     32
Maximum Threads per Block:     1024
Maximum Block Dimensions:      1024, 1024, 64
Maximum Grid Dimensions:       2147483647 x 65535 x 65535
Maximum Memory Pitch:          2147483647B
Texture Alignment:             512B
Clock Rate:                    873 MHz
Execution Timeout:             No
Integrated Device:             No
Can Map Host Memory:           Yes
Compute Mode:                  default
Concurrent Kernels:            Yes
ECC Enabled:                   Yes
Memory Clock Rate:             2505 MHz
Memory Bus Width:              256 bits
L2 Cache Size:                 2097152 bytes
Max Threads Per SMP:           2048
Async Engines:                 2
Unified Addressing:            Yes
Managed Memory:                Yes
Concurrent Managed Memory:     No
Default Target:                cc50


## Performance evaluation
(speed-up, throughput, weak and strong scaling) and discussion about overheads and optimizations done.    


## Discussion and Future Work
etc    


## References
1.	Van der Maaten, L., & Hinton, G. (2008). Visualizing data using t-SNE. Journal of machine learning research, 9(11).
2.	Kobak, D., & Berens, P. (2019). The art of using t-SNE for single-cell transcriptomics. Nature communications, 10(1), 5416. https://doi.org/10.1038/s41467-019-13056-x
3.	Segerstolpe Å, Palasantza A, Eliasson P, Andersson EM, Andréasson AC et al. (2016) Single-Cell Transcriptome Profiling of Human Pancreatic Islets in Health and Type 2 Diabetes.
4.	Performance Comparison of Dimension Reduction Implementations¶. Performance Comparison of Dimension Reduction Implementations - umap 0.5 documentation. (n.d.). https://umap-learn.readthedocs.io/en/latest/benchmarking.html. 

