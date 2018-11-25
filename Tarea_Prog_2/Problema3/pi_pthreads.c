/**
-The program should take the number of threads as input parameters.
-The minimum number of decimal places for pi is 5.
**/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //to handle POSIX threads
#include <math.h>

double gPi = 0.0;  //  global accumulator for value computed
pthread_mutex_t theMutex;
int num_Dec, num_Threads;


//Thread function
void *pi_sum_runner(void* pi_Arg) {
  int thread_number = *((int*)pi_Arg);
  double h = 2.0 / num_Dec;
  double partSum = 0.0, x;  // local to each thread

  //adds value to partSum, increasing based on the number of threads
  for (int i = thread_number; i < num_Dec; i+=num_Threads) {
    x = -1 + (i + 0.5f) * h;
    partSum += sqrt(1.0 - x*x)*h;
  }

  //locks and unlocks the thread until sum is done
  pthread_mutex_lock(&theMutex);
  gPi +=partSum; // add to global variable
  pthread_mutex_unlock(&theMutex);
  //cancel and cleanup
  pthread_exit(0);
}

int main(int argc, char const *argv[]) {
  //error handling
  if(argc != 3) {
    printf("Error: Sent a wrong number of parameters:%d", argc-1);
    printf( "Please send 2 parameters in the format: <executable> <N. decimals> <N. threads>");
    exit(1);
  }

  num_Threads = atoi(argv[1]);
  num_Dec= atoi(argv[2]);

  pthread_t tHandles[num_Threads];
  int tNum[num_Threads], i;
  pthread_mutex_init(&theMutex, NULL);

  for ( i = 0; i <num_Threads ; ++i ) {
    tNum[i] = i;
    pthread_create(
      &tHandles[i],       // Returned thread handle
      NULL,               // Thread Attributes
      pi_sum_runner,      // Thread function
      (void*)&tNum[i]);   // Data for pi_sum_runner
  }
  for (i = 0; i < num_Threads; ++i ) {
    pthread_join(tHandles[i], NULL);
  }

  printf("Computed value for pi: %f\n", 2 * gPi);

  //clean mutex ptr
  pthread_mutex_destroy(&theMutex);
  return 0;
}
