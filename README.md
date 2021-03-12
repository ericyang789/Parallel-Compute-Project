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

In a few sentences, what do you plan for the CS205 final project?
- Data: Sequencing, genomics, RNA-seq...
- Algorithms: ML algorithms, ie Kmeans, PCA...
- Application: Cell type classification, or disease vs normal, etc 
- Different parallelization techniques
- Different initializations...

What course concepts will be used in the project? (Note that some concepts may not be covered yet) [3 - 5 sentences] *
- Some approaches: Big data stuff, different levels of parallelism (many core, multi core,
- Diagnosis in the beginning, understand what part is taking the longest, biggest areas for max performance gain, scalability
- Performance optimization
- Loop unrolling, matrix blocking, row major vs column major, loop interchange...
- Directives, openacc, openmp...

What resources will you need to accomplish this task (AWS / compute cluster / etc.) Please let us know if you need help getting access to resources. We may be able to work with research computing for specific cases. [3-5 sentences] *
- AWS credits, not sure how much, GPU computing, clusters if we do MPI, Cannon cluster
- Potential EBS storage (potenntially hundreds of GB maybe more if raw sequencing data, but most likely we'll do expression data ie counts/TPM), S3 might be cheap
- We might want to compare performance between cloud computing vs supercomputer
- not sure what exact techniques but thinking about a multitude of possibilities, leaving doors open to do CPU-GPU, and clusters


Q for TA: 
- Novelty
- Programming language
