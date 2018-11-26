# Programmed Homework No.2 - IE0521
This repository consists of 3 different problems:
* Problema1: A cache using the MESI protocol.
* Problema2: Multiplication of matrices and vectors in MPI.
* Problema3: Calculation of the constant pi via pthreads.

## Getting Started
(Pending)

## Installing
Once downloaded, use the following commands to make and compile the code
```
make build
```

## Running
To run Problema2 copy the following command in the terminal, with representing the size of the
matrix and vector to multiply.
```
make "proc = n" run_Problema2
```

To run Problema3, copy the next command with n being the Number of decimals
and m being the number of threads.
```
 make "num_Dec = n" "num_Threads = m" run_Problema3
```

## Cleaning
To remove the executables run:
```
make clean
```
