#include <stdio.h>
#include <string.h>

#define Mr 100 // Number of rows in data
#define Nr 16384 // Number of columns in data
double data[Mr][Nr];
void load_data() {
  int i=-1;
  int j=-1;
  double d;

  //FILE *the_file = fopen("mnist2500_nohead.csv","r");
  // Location of data
  FILE *the_file = fopen("../single_cell_data/single_cell_data_small_norm.csv","r");


  char line[100000];
  for (i=0; i<Mr; i++){
  //For each row in csv file get the line
    fgets(line,sizeof(line), the_file);
    char *token;

    // Separate each line by comma and for each token add the data point to data array
    token=strtok(line,",");
    for (j=0; j<Nr; j++){
    //while(token!=NULL){ //element N
      //printf("%-12s",token);
      sscanf(token, "%lf", &d);;
      data[i][j]=d;
      //printf("nxt\n");
      token = strtok(NULL,",");
    }

    printf("\n");
  }
}
