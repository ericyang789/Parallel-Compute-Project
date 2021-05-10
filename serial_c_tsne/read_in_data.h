#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// set appropriate path for data
#define DATA "../data/t10k-images.idx3-ubyte"
#define DATA_LABEL "../data/t10k-labels.idx1-ubyte"

#define Ns 784 // 28*28
#define Ms 2500 //NUM_TEST
#define LEN_INFO_IMAGE 4
#define LEN_INFO_LABEL 2

#define MAX_IMAGESIZE 1280
#define MAX_BRIGHTNESS 255
#define MAX_FILENAME 256
#define MAX_NUM_OF_IMAGES 1


unsigned char image[MAX_NUM_OF_IMAGES][MAX_IMAGESIZE][MAX_IMAGESIZE];
int width[MAX_NUM_OF_IMAGES], height[MAX_NUM_OF_IMAGES];

int info_image[LEN_INFO_IMAGE];
int info_label[LEN_INFO_LABEL];

unsigned char test_image_char[Ms][Ns];
unsigned char test_label_char[Ms][1];


double data[Ms][Ns]; //test_image
int data_label[Ms]; //test_label




void FlipLong(unsigned char * ptr)
{
    register unsigned char val;

    // Swap 1st and 4th bytes
    val = *(ptr);
    *(ptr) = *(ptr+3);
    *(ptr+3) = val;

    // Swap 2nd and 3rd bytes
    ptr += 1;
    val = *(ptr);
    *(ptr) = *(ptr+1);
    *(ptr+1) = val;
}


void read_mnist_char(char *file_path, int num_data, int len_info, int arr_n, unsigned char data_char[][arr_n], int info_arr[])
{
    int i, j, k, fd;
    unsigned char *ptr;

    if ((fd = open(file_path, O_RDONLY)) == -1) {
        fprintf(stderr, "couldn't open image file");
        //printf(" File wasn't opened \n");
        exit(-1);
    }

    read(fd, info_arr, len_info * sizeof(int));



    // read-in information about size of data
    for (i=0; i<len_info; i++) {
        ptr = (unsigned char *)(info_arr + i);
        FlipLong(ptr);
        ptr = ptr + sizeof(int);
    }

    // read-in mnist numbers (pixels|labels)
    for (i=0; i<num_data; i++) {
        read(fd, data_char[i], arr_n * sizeof(unsigned char));
    }

    close(fd);
}


void image_char2double(int num_data, unsigned char data_image_char[][Ns], double data_image[][Ns])
{
    int i, j;
    for (i=0; i<num_data; i++)
        for (j=0; j<Ns; j++)
            data_image[i][j]  = (double)data_image_char[i][j] / 255.0;
}


void label_char2int(int num_data, unsigned char data_label_char[][1], int data_label[])
{
    int i;
    for (i=0; i<num_data; i++)
        data_label[i]  = (int)data_label_char[i][0];
}


void load_data()
{

    read_mnist_char(DATA, Ms, LEN_INFO_IMAGE, Ns, test_image_char, info_image);
    image_char2double(Ms, test_image_char, data);

    read_mnist_char(DATA_LABEL, Ms, LEN_INFO_LABEL, 1, test_label_char, info_label);
    label_char2int(Ms, test_label_char, data_label);
}
