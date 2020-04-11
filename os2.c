#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#define handle_error_en(en, msg) 
do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
volatile int running_threads = 0;
pthread_t thread[3]; 
int numOfElements;
struct Results{ 

int min;
int max;
int average;

}Results;
void *findMin(void *array_ptr){
int i;
int *elements = (int*)array_ptr;  
Results.min = elements[0];
for(i = 0; i < numOfElements; i++){
	if(elements[i] < Results.min){
		Results.min = elements[i];

	}

}

running_threads -= 1;

return NULL;

}
void *findMax(void *array_ptr){

int i;	/*counter*/

int *elements = (int*)array_ptr; 

for(i = 0; i < numOfElements; i++){	

	if(elements[i] > Results.max){	

		Results.max = elements[i];

	}
}

running_threads -= 1;	

return NULL;

}
void *findAverage(void *array_ptr){
int i;

int *elements = (int*)array_ptr; 	

for(i = 0; i < numOfElements; i++){	

	Results.average += elements[i];		
}
Results.average = Results.average/numOfElements;
running_threads -= 1;	
return NULL;
}
int getArrayInput(int n, int *array_ptr){
	int input;
	int numberOfElements = 0;
printf("Creating Dynamic Array...\n-\n");
for(;;){  
printf("Enter a positive value:\nNegative Number to Stop\n-\n");

if (scanf("%d",&input) != 1){

printf("\nOops that wasn't an Integer\nlets try filling the array again\nRemember INTEGERS only!\n");

			exit(EXIT_FAILURE);
		}
	if (input >= 0){ 
	 	if (numberOfElements == n){
	  	  n += 1;
		  array_ptr = realloc(array_ptr, n * sizeof(int));
		 }
	array_ptr[numberOfElements++] = input;
	} else {
	 printf("\nNumber of Integers: %d\n", numberOfElements);
	 break;
			 }
		}
return numberOfElements;
	}
void joinThreads(int numberOfThreads){

int i; 
int s;
while(numberOfThreads >= 0){
	s = pthread_join(thread[numberOfThreads], NULL);
	 if (s != 0){
		handle_error_en(s, "pthread_create");
	 }
	 numberOfThreads--;
}
}
void createThreads(int *array_ptr){
int s; 
s = pthread_create(&thread[0], NULL, findMin, (void *)array_ptr);
 if (s != 0){
		handle_error_en(s, "pthread_create");
	 }
	 	running_threads += 1;
 s = pthread_create(&thread[1], NULL, findMax, (void *)array_ptr);
	 if (s != 0){
  handle_error_en(s, "pthread_create");
 }
 running_threads += 1; 
 s = pthread_create(&thread[2], NULL, findAverage, (void *)array_ptr);
	 if (s != 0){
 handle_error_en(s, "pthread_create");
 }
		running_threads += 1;
}
int main(){
int n = 1; 
int *array_ptr = malloc(n * sizeof(int));
	 numOfElements = getArrayInput(n, array_ptr);
	 createThreads(array_ptr);
    	while(running_threads>0){	
			sleep(1);
		}
	joinThreads(2);
	printf("\nThe average is %d\nThe maximum is %d\nThe minimum is %d\n",Results.average, Results.max, Results.min);
return(0);
} 
