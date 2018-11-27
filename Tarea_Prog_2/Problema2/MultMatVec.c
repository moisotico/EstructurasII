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
#include<mpi.h>       // to paralelize processes
#include<stdio.h>     // printf
#include<stdlib.h>  // for malloc and free
#include<time.h>    // for execution time
#include<string.h>
#include<math.h>
//Definitions
#define MASTER 0
#define M_Size 5
void create_matrix_and_vector(
            unsigned int n_size,
            double matrix_A[n_size][n_size],
            double vect_B[n_size])
{
  int i,j;
  /**
  -Fill the vector and  the matrix  with the element equal to the current position (the sum of current row and column in the matrix)  **/
  printf("Input matrix: \n");
  for( i = 0; i < n_size; i++) {
    for(j = 0; j < n_size; j++) {
      matrix_A[i][j] = i+j;
      printf("%.0lf    ", matrix_A[i][j]);
    }
    printf("\n");
  }

  printf("Input vector: \n");
  for (i = 0; i < n_size; i++) {
    vect_B[i] = n_size - (i+1);
    printf("%.0lf\n",vect_B[i]);
  }
}

  //Code to paralelize with MPI using buffers with MPI_Bcast

void multiply_row(
    int i,
    unsigned int n_size,
    double matrix_A[n_size][n_size],
    double vect_B[n_size],
    double Results_vect[n_size] )
{
  for (int j = 0; j < n_size; j++) {
    Results_vect[i] += matrix_A[i][j] * vect_B[j];
  }
}


void print_Results(int n_size, double Results_vect[n_size])
{
  printf("Results vector: \n");
  for (int i = 0; i < n_size; i++)
    printf("%.0lf\n",Results_vect[i]);
}
  /**
  printf("Results vector: \n");
  for (i = 0; i < n_size; i++) {
    for (j = 0; j < n_size; j++) {
      Results_vect[i] += matrix_A[i][j] * vect_B[j];
    }
    printf("%.0lf\n",Results_vect[i]);
  }
}
  **/

int main(int argc, char *argv[]){
  clock_t begin = clock();
  int myrank, nprocs;
  MPI_Init(&argc, &argv); //start MPI
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs); //nproc means the number of processes
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  //  printf("rank: %d\n",myrank);
  MPI_Status status;

  /**Master process**/
  if(myrank == MASTER) { // this is the code for the master

    printf("This is a program to multiply a vector and a matrix using the MPI library \n \n");
    unsigned int size = 0;
    int finished;
    double matrix_A[size][size];
    /**create 2 n sized vectors: one with the data to multiply,
    the other stores the results**/
    double vect_B[size], Results_vect[size];
    int i, j;

    while(size == 0){
    //printf("Enter a valid size for the square matrix and vector size:");
    //scanf(" %5d", &size); //user input
    size = M_Size;
    printf("\nEntered size for the square matrix and vector: %d\n", size);
    }
    //create values
    create_matrix_and_vector(size, matrix_A,vect_B);
    time_t start_time;
    time_t end_time;
    start_time = time(NULL);
    printf("Master process sending data");
    MPI_Send(&size, 1, MPI_INT, myrank, 5, MPI_COMM_WORLD); // the process with sends its data in buffer to all other procs, to store it in their buffer.
    int to_do = (int)round(size/(nprocs-1)); // there number of rows to paralelize equals size
    //to_do += 1; // increase to_do by 1 to avoid rounding errors in division
    MPI_Bcast(&to_do, 1, MPI_INT, 0, MPI_COMM_WORLD); // send number of multiplications each slave must complete

    MPI_Bcast(&matrix_A, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&vect_B, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Results_vect, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //srand(time(NULL));
    int count = 0;
    while(count < size) { // listen for slaves until the slaves have reported that enough work is done
      int done_by_slave;
      MPI_Status status;
      //blocking
      MPI_Recv(&Results_vect, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Recv(&done_by_slave, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      count += done_by_slave;
    }

    print_Results(size, Results_vect);
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution time: %d\n",time_spent);
  }

  /**Slave Process**/
  else { // this is the code for the slaves to run
    int size;
    int to_do, offset;
    int done; //counter
    int tot_rows; //total rows
    printf("Proceso esclavo %d\n por recibir dato", myrank);
    MPI_Recv(&size, 1, MPI_INT, 0,  5, MPI_COMM_WORLD, &status); // receive the size of the matrices to be multiplied
    MPI_Bcast(&to_do, 1, MPI_INT, 0, MPI_COMM_WORLD); // receive how many multiplications to be done

    double matrix_A[size][size];
    double vect_B[size], Results_vect[size];
    MPI_Bcast(&matrix_A, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&vect_B, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Results_vect, 1, MPI_INT, 0, MPI_COMM_WORLD);
    print_Results(size, vect_B);

    offset = to_do*myrank-1 + done;
    while(to_do > done || size > offset) { // run until we have done an adaquate number of operations
      multiply_row(done, size, matrix_A, vect_B, Results_vect);
      ++done;
      offset= to_do*myrank-1 + done;
    }
    MPI_Send(&Results_vect, 1, MPI_INT, 0,myrank, MPI_COMM_WORLD); //report changes in matrix
    MPI_Send(&done, 1, MPI_INT, 0, myrank, MPI_COMM_WORLD); // report how many multiplications we have done
  }

  MPI_Finalize();
  return 0;
}
