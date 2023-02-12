#include<stdio.h>
#include "lodepng.h"
#include<stdlib.h>
#include<pthread.h>

unsigned char *image, *blurImage;
unsigned int width, height;
int r, g, b, a;

struct img{
    int start;
    int end;
};

void* imgBlur(void* args){
    struct img *ptr = (struct img*) args;
    int x, y;
    for(x = ptr->start; x<ptr->end;x++){
        for(y = 0; y < width; y++){
            //storing the pixel value to new variable
            r = image[4 * width * height + 4 * y + 0];
            g = image[4 * width * height + 4 * y + 1];
            b = image[4 * width * height + 4 * y + 2];
            a = image[4 * width * height + 4 * y + 3];
            //calculating the average of the pixel value
            r = (r + image[4 * width * (x-1) + 4 * y + 0] + image[4 * width * (x+1) + 4 * y + 0] + image[4 * width * x + 4 * (y-1) + 0] + image[4 * width * x + 4 * (y+1) + 0])/5;
            g = (g + image[4 * width * (x-1) + 4 * y + 1] + image[4 * width * (x+1) + 4 * y + 1] + image[4 * width * x + 4 * (y-1) + 1] + image[4 * width * x + 4 * (y+1) + 1])/5;
            b = (b + image[4 * width * (x-1) + 4 * y + 2] + image[4 * width * (x+1) + 4 * y + 2] + image[4 * width * x + 4 * (y-1) + 2] + image[4 * width * x + 4 * (y+1) + 2])/5;
            a = (a + image[4 * width * (x-1) + 4 * y + 3] + image[4 * width * (x+1) + 4 * y + 3] + image[4 * width * x + 4 * (y-1) + 3] + image[4 * width * x + 4 * (y+1) + 3])/5;
            //storing the average value to the new image
            blurImage[4 * width * x + 4 * y + 0] = r;
            blurImage[4 * width * x + 4 * y + 1] = g;
            blurImage[4 * width * x + 4 * y + 2] = b;
            blurImage[4 * width * x + 4 * y + 3] = a;
            
        }
    }
}
