/***
To consider:
- # of rows in the matrix is not necessarily a multiple of the # of processors
- Each processor works one row at the time
- Master process asigns another job to slaves only if these have finished their
asignments
- The program must receive as inputs the dimensions of the (square) matrix
- Print the result, as well as the array and matrix used for the calulation.
***/

//Libraries
//#include<mpi.h>
#include<stdio.h>     // printf
#include<stdlib.h>  // for malloc and free
#include<time.h>    // for execution time
#include<string.h>
//Definitions
#define MASTER 0
#define TOTAL_OPS 1000 //number of processes neded for control


void create_matrix_and_vector(unsigned int n_size){
  //create a nxn sized matrix
  double matrix_A[n_size][n_size];

  /**create 2 n sized vectors: one with the data to multiply,
  the other stores the results**/
  double vect_B[n_size], Results_vect[n_size];
  int i, j;
  /**
  -Fill the vector with the element equal to the current position.
  -Generate the matrix elements such that the element equals the sum of the
  row and the column number.
  **/
  printf("Input matrix: \n");
  for( i = 0; i < n_size; i++) {
    for(j = 0; j < n_size; j++) {
      matrix_A[i][j] = i+j;
      printf("%f    ", matrix_A[i][j]);
    }
    printf("\n");
  }

  printf("Input vector: \n");
  for (i = 0; i < n_size; i++) {
    vect_B[i] = i;
    printf("%f\n",vect_B[i]);
  }


  //Code to paralelize with MPI using buffers with MPI_Bcast
  printf("Results vector: \n");
  for (i = 0; i < n_size; i++) {
    for (j = 0; j < n_size; j++) {
      Results_vect[i] += matrix_A[i][j] * vect_B[j];
    }
    printf("%f\n",Results_vect[i]);
  }

}


int main(int argc, char *argv[]){
  printf("This is a program to multiply a vector and a matrix using the MPI library \n \n");
/**
  int myrank, nprocs;
  MPI_Init(&argc, &argv); //start MPI
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Status status;

  if(myrank == MASTER) { // this is the code for the master to run
**/ //char userInput[64];
    unsigned int size = 0;
    //int finished;
    printf("Enter the square matrix and vector size:");
    scanf(" %5d", &size);
    fgets(userInput, 64, stdin);
    size = (unsigned int)(userInput);
    printf("\nEntered size: %d\n", size);
/**
    time_t start_time;
    time_t end_time;
    start_time = time(NULL);
    **/
/**
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD); // the process with sends its data in buffer to all other procs, to store it in their buffer.
    int to_do = TOTAL_OPS/(nprocs - 1); // there are nprocs - 1 slaves
    to_do += 1; // increase to_do by 1 to avoid rounding errors in division
    MPI_Bcast(&to_do, 1, MPI_INT, 0, MPI_COMM_WORLD); // send number of multiplications each slave must complete

    srand(time(NULL)); // seed the random number generator using the current system time
    int count = 0;
    while(count < TOTAL_OPS) { // listen for slaves until the slaves have reported that enough work is done
      int done_by_slave;
      MPI_Status status;
      MPI_Recv(&done_by_slave, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      count += done_by_slave;
    }
    end_time = time(NULL) - start_time; // get the time the program took
    printf("Seconds Taken %d\n", ((int)end_time));
  }

  else { // this is the code for the slaves to run
    int size;
    int to_do;
    int done = 0;
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD); // receive the size of the matrices to be multiplied
    MPI_Bcast(&to_do, 1, MPI_INT, 0, MPI_COMM_WORLD); // receive how many multiplications to be done
    while(to_do > done) { // run until we have done an adaquate number of operations
**/
      create_matrix_and_vector(size);
/**
      ++done;

    }
    MPI_Send(&done, 1, MPI_INT, 0, myrank, MPI_COMM_WORLD); // report how many multiplications we have done
  }

  MPI_Finalize();
  **/
  return 0;
}



/**
M P I B a r r i e r (comm) ; use??

**/
