# Parallel-Compute-Project
Temp description: Harvard CS205 Final Project

Brainstorming...
- Parallelize deep learning, other ML techniques on medical-related prediction (maybe diagnostics, imaging or other data)
-  https://web.stanford.edu/~rezab/classes/cme323/S16/projects_reports/hedge_usmani.pdf
-  https://arxiv.org/pdf/1802.09941.pdf
-  https://arxiv.org/pdf/1706.02677.pdf
- Parallelizing k-means with OpenMP: https://cse.buffalo.edu/faculty/miller/Courses/CSE633/Chandramohan-Fall-2012-CSE633.pdf
-   Kmeans (or weighted) for cell type classification with RNA-Seq data would be interesting

Vague ideas:
- There's quite a lot of genomics data available, could be interesting to see if we can do some classification on this i.e. whether the individuals have certain medical conditions
- Off-topic but there have been some intereting publications of watermarking of audio files. The process is notoriously slow and some speed up is achieved through randomised SVD but could lend itself very well to this project
- Kmeans: different starting points, could be another parallelization dimension, communicate best result in the end. Also resource allocation if one node converges early.


Q for TA: 
- Novelty
- Programming language


Milestone 2


In a few sentences, what do you plan for the CS205 final project?

For the project we will be working on a classification problem using genomics data, specifically RNA-seq or gene-expression data. We are currently considering sample type clustering or classification (i.e. determining whether individuals have come from healthy control cohorts or have a health condition). Our goal is to implement machine learning algorithms such as K-means clustering and PCA to achieve this whilst also using various parallelisation techniques in order to improve the performance of the chosen algorithms. Some potential areas we can parallelize for a k-means algorithm is in the computation of distances between vectors of samples, calculating cluster means, the assignment step, trying runs with different "k's", and using different initialization points to ensure the algorithm finds the true global optima.  

- Data: Sequencing, genomics, RNA-seq...
- Algorithms: ML algorithms, ie Kmeans, PCA...
- Application: Cell type classification, or disease vs normal, etc
- Different parallelization techniques
- Different initializations...

What course concepts will be used in the project? (Note that some concepts may not be covered yet) [3 - 5 sentences] *

Initially, we will run diagnostics on the performance of the standard chosen algorithms in order to understand what sections of the code would benefit the most from parallelisation. Likewise, we will look into the scalability of the code. We plan on using a variety of different performance optimization techniques e.g.  loop unrolling, matrix blocking, loop interchange, etc. We also currently plan on using a compiler language and so will be looking at OpenACC and OpenMP. Specific details on which computing frameworks will be implemented (many core, multi core, shared memory, distributed memory, etc) will be decided on as the project matures. 


- Some approaches: Big data stuff, different levels of parallelism (many core, multi core,
- Diagnosis in the beginning, understand what part is taking the longest, biggest areas for max performance gain, scalability
- Performance optimization
- Loop unrolling, matrix blocking, row major vs column major, loop interchange...
- Directives, openacc, openmp...

What resources will you need to accomplish this task (AWS / compute cluster / etc.) Please let us know if you need help getting access to resources. We may be able to work with research computing for specific cases. [3-5 sentences] *

The exact techniques we will use will depend on the outcome of the diagnostics tests so we are currently considering a multitude of possibilities leaving the doors open to the use of GPU computing and the use of clusters. We are likely to be using AWS EC2 and depending on whether we are working with raw sequencing data (which can be 100s of GB in size) we may require the use of S3. We are also looking at the potential for heterogenous CPU-GPU computing and potentially comparing algoithmic performance on different platforms e.g. cloud computing vs supercomputer. 

- AWS credits, not sure how much, GPU computing, clusters if we do MPI, Cannon cluster
- Potential EBS storage (potentially hundreds of GB maybe more if raw sequencing data, but most likely we'll do expression data ie counts/TPM), S3 might be cheap
- We might want to compare performance between cloud computing vs supercomputer
- not sure what exact techniques but thinking about a multitude of possibilities, leaving doors open to do CPU-GPU, and clusters
