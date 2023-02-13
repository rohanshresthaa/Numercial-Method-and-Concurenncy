#include <stdio.h>
#include "lodepng.h"
#include <stdlib.h>
#include <pthread.h>

//declaring a 3D array to store the image
int ***arr;
//declaring unsigned char pointer to store the image 
unsigned char *Image;
//declaring a structure to store the values of height, width, start height, end height, thread id and kernel size
struct pixels{
    int starth;
    int endh;
    int h;
    int w;
    int tid;
    int kernel_size;
};
//declaring a function to blur the image
void *blur(void *ptr){

    int i, j, k, l, m; 
    struct pixels *arg = (struct pixels*)ptr;
	int sth = arg->starth, fih = arg->endh, kernelsize = arg->kernel_size;
    int thread_id = arg->tid, height = arg->h, width = arg->w;
    //looping through the height of the image
    for(i=sth; i<=fih; i++ ){
    	//looping through the width of the image
        for(j=0; j<width; j++){
        	//initializing the red, green and blue values to 0
            int avgR=0, avgG=0, avgB=0, numOfPixels = 0;
            //looping through the kernel size to get the average of the pixels
            for (l= -kernelsize; l<=kernelsize; l+=kernelsize){
                for (m = -kernelsize; m<=kernelsize; m+=kernelsize){
                    //getting the index of the pixel
                    int xPixel = i+l;
                    int yPixel = j+m;
                   //checking if the pixel is within the image
                    if(xPixel >=0 && yPixel >= 0 && xPixel<height && yPixel<width){
                        avgR += arr[xPixel][yPixel][0];
                        avgG += arr[xPixel][yPixel][1];
                        avgB += arr[xPixel][yPixel][2];
                        numOfPixels++;
                    }          
                }
            }
            //storing the average of the pixels in the image
            Image[4*width*i+4*j+0] = avgR/numOfPixels;
            Image[4*width*i+4*j+1] = avgG/numOfPixels;
            Image[4*width*i+4*j+2] = avgB/numOfPixels;
        }
    }
    //exit the thread
    pthread_exit(0);

}

void main(){
    //declaring variables to store the error, width, height, number of threads and kernel size
	unsigned int error, width, height;
    //declaring variables to store the values of i, j and k
    int i, j, k;
    //declaring variables to store the number of threads and kernel size
    int numOfThreads, ksize;
    //declaring a variable to store the filename
    char filename[20];
    printf("Enter the location of Image: \n");
    scanf("%s", &filename);
    error = lodepng_decode32_file(&Image, &width, &height, filename);
    printf("Enter the number of threads that you want to use:");
    scanf("%d", &numOfThreads);
    //checking if the number of threads is greater than the height of the image
    if(numOfThreads > height){
        //if it is greater than the height of the image, then we set the number of threads to the height of the image
    	numOfThreads = numOfThreads%height + 1;
        //printing the number of threads that we are using
        printf("Invalid number of threads Automatically choosing the appropriate number of threads: %d threads", numOfThreads);
	}
    printf("Enter the size of the kernel \n1. 3x3 \n2. 5x5 \n3. 7x7: "); 
    scanf("%d", &ksize);
    
    //checking if the kernel size is 1, 2 or 3
    arr = (int***)malloc(height * sizeof(int**));
    for (i = 0; i < height; i++) {
        arr[i] = (int**)malloc(width * sizeof(int*));
        for (j = 0; j < width; j++) {
                arr[i][j] = (int*)malloc(3 * sizeof(int));
        }
    }
	//checking if there is an error in decoding the image
    if(error){
        printf("Error in decoding image %d: %s\n", error, lodepng_error_text(error));
    }else{
        //if there is no error, then we store the image in the 3D array
        for(i=0; i<height; i++){
            for(j=0; j<width; j++){
                for(k=0; k<3; k++){
                    arr[i][j][k] = Image[4*width*i+ 4*j + k];
                }
            }
        }
    }

    //declaring variables to store the start height, end height, slice height and reminder height
    int sliceHeight[numOfThreads];
    //calculating the slice height
	for(i=0; i<numOfThreads; i++){
		sliceHeight[i] = height/numOfThreads;			
	}
    //calculating the reminder height
	int reminderHeight = height%numOfThreads;
	for(i=0; i<reminderHeight; i++){
		sliceHeight[i]++;
	}
    //calculating the start height and end height
	int startHeight[numOfThreads], endHeight[numOfThreads];
	for(i=0; i<numOfThreads; i++){
        if (i==0) {
            startHeight[i] = 0;
        } else {
            startHeight[i] = endHeight[i-1] + 1;
        }
        endHeight[i] = startHeight[i] + sliceHeight[i] - 1;
    }
    //array of structures to store the start height, end height, height, width, thread id and kernel size
    struct pixels divider[numOfThreads];
    //declaring an array of threads
    pthread_t threads[numOfThreads];

    int thid = 1;
    //now we create the threads
    for(i=0; i<numOfThreads; i++){
        divider[i].starth = startHeight[i];
        divider[i].endh = endHeight[i];
        divider[i].h = height;
        divider[i].w = width; 
        divider[i].tid = thid;
        divider[i].kernel_size = ksize;
        thid++;
        pthread_create(&threads[i], NULL, blur, &divider[i]);   
    }
    //joining the threads
    for(i=0; i<numOfThreads; i++){
        pthread_join(threads[i], NULL);   
    }
	//declaring a pointer variable to store the png image
    unsigned char *png;
    //declaring a variable to store the name of the output image
    char imagename[20];
    //declaring a variable to store the size of the png image
    size_t pngsize;
    printf("Successfully blurred the image \n Enter the name of the output image:");
    scanf("%s", &imagename);
    //saving the image
    lodepng_encode32(&png, &pngsize, Image, width, height);
    lodepng_save_file(png, pngsize, imagename);
    
}