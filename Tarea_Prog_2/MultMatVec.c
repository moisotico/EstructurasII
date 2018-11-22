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
#include<mpi.h>
#include<stdio.h>     // printf
#include <stdlib.h>  // for malloc and free
#include <time.h>    // for execution time

void create_matrix_and_vector(int n_size){
  //create a nxn sized matrix
  double matrix_A[size][size];
  //create 2 n sized vectors: one with the data to multiply,
  //the other stores the results
  double vect_B[size], Results_vect[size];
  int i, j;
  //fill the vector with the element equal to the current position
    vect_B[i] = i;
  }

/**
Generate the matrix elements such that the element equals the sum of the
row and the column number.
**/
  for (i = 0; i < count; i++) {
    for (j = 0; j < count; j++) {
      matrix_A[i][j] = i+j;
    }
  }


  //Code to paralelize

  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      Results_vect[i] += matrix_A[i][j] * vect_B[j]
    }
  }



}



int main(int argc, char const *argv[]) {
  printf("Hello World!\n");
/**
  int i, j, n; //counters
  double in_mat[][]; //input matrix, to be filled  wh data from positions
  double in_vect[]; //pending
  double result_vect[]; //store results
**/
  int myrank, nprocs;
  MPI_Init(&argc, &argv); //start MPI
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  return 0;
}



/**
M P I B a r r i e r (comm) ; use??

**/
