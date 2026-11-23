#!/bin/bash
set -e
cd "$(dirname "$0")"

echo "===== PROGRAM 1: MPI_Send / MPI_Recv ====="
mpicc 1.c -o p1
mpiexec --oversubscribe -n 4 ./p1

echo "===== PROGRAM 3: MPI_Bcast ====="
mpicc 3.c -o p3
printf "25\n" | mpiexec --oversubscribe -n 4 ./p3

echo "===== PROGRAM 4: MPI_Scatter / MPI_Gather ====="
mpicc 4.c -o p4
mpiexec --oversubscribe -n 4 ./p4

echo "===== PROGRAM 5: MPI_Reduce / MPI_Allreduce ====="
mpicc 5.c -o p5
mpiexec --oversubscribe -n 4 ./p5

echo "===== PROGRAM 7: OpenMP static scheduling ====="
gcc -fopenmp 7.c -o p7
printf "8\n" | OMP_NUM_THREADS=4 ./p7

echo "===== PROGRAM 8: OpenMP Fibonacci tasks ====="
gcc -fopenmp 8.c -o p8
printf "10\n" | OMP_NUM_THREADS=4 ./p8

echo "===== PROGRAM 9: OpenMP prime numbers ====="
gcc -fopenmp 9.c -o p9
printf "500\n" | OMP_NUM_THREADS=4 ./p9

echo "===== ALL SELECTED PROGRAMS COMPLETED ====="
