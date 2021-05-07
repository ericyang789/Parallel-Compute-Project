#include <stdio.h>
#include <string.h>

#define Mr 2500
#define Nr 784
double data[Mr][Nr];
void load_data() {
  int i=-1;
  int j=-1;
  double d;

  FILE *the_file = fopen("mnist2500_nohead.csv","r");

  char line[100000];
  for (i=0; i<Mr; i++){
  //while (fgets(line,sizeof(line), the_file)){ //row M
    fgets(line,sizeof(line), the_file);
    char *token;

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
