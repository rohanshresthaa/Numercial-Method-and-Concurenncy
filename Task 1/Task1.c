#include<stdio.h>
void main(){
    // File names in an array
    char *files[] = {"datasetLR1.txt", "datasetLR2.txt", "datasetLR3.txt", "datasetLR4.txt"};
    //declaring variables
    int i, n = 0, x, y;
    //initializing variables
    float x_sum=0;
    float y_sum=0;
    float x2_sum=0;
    float y2_sum=0;
    float sum_xy=0;
    float tot_a, tot_b, pred_y, x_user;
    //looping through the files
    for(i = 0; i < 4; i++){
        //opening the file
        FILE *fp = fopen(files[i], "r");
        //reading the file
        while(fscanf(fp, "%d,%d", &x, &y) != EOF)
        {
            //calculating the sum of x, y, x^2, y^2, xy
            x_sum += x;
            y_sum += y;
            x2_sum += x*x;
            y2_sum += y*y;
            sum_xy += x*y;
            n++;
        }
        //closing the file
        fclose(fp);
    }
    //printing the results
    printf("Total number of lines: %d\n", n);
    //printing the sum of x, y, x^2, y^2, xy
    printf("Total sum of x: %f \n", x_sum);
    printf("Total sum of y: %f \n", y_sum);

    //using the formula to calculate a and b
    tot_a =((y_sum*x2_sum)-(x_sum*sum_xy))/((n*(x2_sum))-(x_sum*x_sum));
    tot_b =((n*sum_xy)-(x_sum*y_sum))/((n*(x2_sum))-(x_sum*x_sum));
    //printing the results
    printf("Total A: %f \n", tot_a);
    printf("Total B: %f \n", tot_b);
    printf("Equation of line: y = %fx + %f \n", tot_b, tot_a);
    //taking input from user
    printf("Enter value of x: ");
    scanf("%f", &x_user);
    //predicting the value of y using the equation
    pred_y = (tot_b*x_user)+tot_a;
    //printing the predicted value of y
    printf("Predicted value of y: %f\n", pred_y);
}