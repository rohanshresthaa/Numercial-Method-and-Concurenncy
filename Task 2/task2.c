#include<stdio.h>
#include<pthread.h>
#include<math.h>

//intializing the mutex lock
pthread_mutex_t lock;

//structure to store the start and end of the iteration
struct limit{
    int start;
    int end;
};

//global variable to store the value of pi
double pi = 1;

//function to calculate the value of pi
void* calculation(void *p){

    //typecasting the void pointer to the structure pointer
    struct limit *l = (struct limit *)p;
    int i;
    //locking the mutex lock
    pthread_mutex_lock (&lock);
    //calculating the value of pi
    for(i=l->start;i<=l->end;i++){
        //using the formula
        pi += pow(-1,i)/(2*i+1);
    }
    //unlocking the mutex lock
   pthread_mutex_unlock (&lock);
}
void main(){
    //initializing the mutex lock
    pthread_mutex_init(&lock, NULL);

    //taking the number of threads and iterations as input
    int numThread, iteration;
    printf("Enter the number of threads: ");
    scanf("%d", &numThread);
    printf("Enter the number of iterations: ");
    scanf("%d", &iteration);
    //calculating the number of iterations per thread
    int slice = iteration/numThread;
    //creating the threads
    pthread_t id[numThread];
    int i;
    //creating the structure array to store the start and end of the iteration
    struct limit l[numThread];
    //initializing the start and end of the iteration
    l[0].start = 1;
    l[0].end = slice;
    //calculating the start and end of the iteration
    for(i=1;i<numThread;i++){
        l[i].start = l[i-1].end + 1;
        l[i].end = l[i-1].end + slice;
    }
    //creating the threads
    for(i=0;i<numThread;i++){
        pthread_create(&id[i], NULL, calculation,&l[i]);
    }
    //joining the threads
    for(i=0;i<numThread;i++){
        pthread_join(id[i], NULL);
    }
    //destroying the mutex lock
    pthread_mutex_destroy(&lock);
    //printing the value of pi
    printf("The value of pi is: %lf", pi*4);

}
