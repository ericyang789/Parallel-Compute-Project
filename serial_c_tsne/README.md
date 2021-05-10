# Serial t-SNE implementation

### File structure:

  tsne.c/
 
          core tsne executable file
          
          calls pca_fns.h, tsne_fns.h, read_in_data.h or read_csv.h and singular_value_decomposition.c
          
          works with an M x N array read in from the read_in_data.h file (for ubyte format data) or read_csv.h (for CSV data)
          
          file calculates an M x L array Y containing the projection of the data onto L dimensions
          
          uploaded file does so for L=2 dimensions however this can be easily changed to any L<=K where K is reduced PCA dimension

