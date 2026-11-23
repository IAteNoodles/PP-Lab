# Parallel Programming Lab — Quick Cheatsheet

## Your selected experiments (new lab-index numbering)

| Lab no. | Topic | Source file |
|---:|---|---|
| 1 | MPI_Send and MPI_Recv | `1.c` |
| 3 | MPI_Bcast | `3.c` |
| 4 | MPI_Scatter and MPI_Gather | `4.c` |
| 5 | MPI_Reduce and MPI_Allreduce | `5.c` |
| 7 | OpenMP static scheduling, chunk size 2 | `7.c` |
| 8 | Fibonacci using OpenMP tasks | `8.c` |
| 9 | Prime numbers: serial and parallel | `9.c` |

Excluded: lab 2 (deadlock) and lab 6 (mergesort).

---

# 1. MPI essentials

MPI = Message Passing Interface. Each MPI process has its own rank and memory.
The rank is the process ID within a communicator.

## Setup and process information

```c
MPI_Init(&argc, &argv);                    // Start MPI
MPI_Comm_rank(MPI_COMM_WORLD, &rank);      // This process's rank
MPI_Comm_size(MPI_COMM_WORLD, &size);      // Total process count
MPI_Finalize();                            // End MPI
```

- `MPI_COMM_WORLD`: communicator containing all launched processes.
- `rank`: process number, from `0` to `size - 1`.
- `size`: total number of processes.
- Usually rank 0 acts as the root/coordinator in these lab examples.

## MPI_Send

```c
MPI_Send(buf, count, datatype, dest, tag, comm);
```

| Argument | Meaning |
|---|---|
| `buf` | Address of data to send |
| `count` | Number of data elements |
| `datatype` | Type, e.g. `MPI_INT`, `MPI_CHAR`, `MPI_DOUBLE` |
| `dest` | Destination process rank |
| `tag` | Message label; sender and receiver must match |
| `comm` | Communicator, usually `MPI_COMM_WORLD` |

Example:

```c
MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
```

Sends one integer `x` to process 1 with tag 0.

## MPI_Recv

```c
MPI_Recv(buf, count, datatype, source, tag, comm, status);
```

| Argument | Meaning |
|---|---|
| `buf` | Address where received data is stored |
| `count` | Maximum number of elements to receive |
| `datatype` | Expected data type |
| `source` | Sender's rank |
| `tag` | Message tag to match |
| `comm` | Communicator |
| `status` | Information about received message |

Example:

```c
MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
```

Receives one integer from process 0. `MPI_STATUS_IGNORE` means the program does not need the message status details.

Remember: the sender's `dest` must match the receiver's `source`; datatype, tag and communicator must also match.

## MPI_Bcast — broadcast

```c
MPI_Bcast(buffer, count, datatype, root, comm);
```

- Sends the same data from one root process to every process in the communicator.
- `buffer`: data location; root starts with the value, all processes receive it.
- `count`: number of elements.
- `datatype`: data type.
- `root`: rank of broadcasting process.
- `comm`: communicator.

Example:

```c
MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
```

All processes get the integer `n` from process 0. Every process must call this collective operation.

## MPI_Scatter — distribute pieces

```c
MPI_Scatter(sendbuf, sendcount, sendtype,
            recvbuf, recvcount, recvtype, root, comm);
```

- Root splits its send array into equal-sized pieces.
- Each process receives one piece.
- `sendbuf`: root's source array.
- `sendcount`: number of elements sent to each process.
- `sendtype`: source element type.
- `recvbuf`: local receive variable/array for each process.
- `recvcount`: number of elements received by each process.
- `recvtype`: receive element type.
- `root`: sending root rank.
- `comm`: communicator.

Example:

```c
MPI_Scatter(data, 1, MPI_INT, &x, 1, MPI_INT, 0, MPI_COMM_WORLD);
```

Each process receives one integer into `x`.

## MPI_Gather — collect pieces

```c
MPI_Gather(sendbuf, sendcount, sendtype,
           recvbuf, recvcount, recvtype, root, comm);
```

- Opposite direction to Scatter: every process sends its local data to root.
- `sendbuf`: local data to send.
- `sendcount`: elements sent by each process.
- `sendtype`: local data type.
- `recvbuf`: root's array for collected data.
- `recvcount`: elements received from each process.
- `recvtype`: receive data type.
- `root`: receiving root rank.
- `comm`: communicator.

Example:

```c
MPI_Gather(&x, 1, MPI_INT, result, 1, MPI_INT, 0, MPI_COMM_WORLD);
```

Root collects one integer from every process into `result`.

## MPI_Reduce — combine values at root

```c
MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
```

- Combines values from all processes using an operation.
- Result is delivered only to `root`.
- `sendbuf`: each process's input.
- `recvbuf`: root's result storage.
- `count`: number of elements.
- `datatype`: data type.
- `op`: operation.
- `root`: process receiving final result.
- `comm`: communicator.

Common operations:
- `MPI_SUM`: sum
- `MPI_PROD`: product
- `MPI_MAX`: maximum
- `MPI_MIN`: minimum

Example:

```c
MPI_Reduce(&x, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
```

Adds all processes' `x` values and stores the result at process 0.

## MPI_Allreduce — combine values at every process

```c
MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
```

Same idea as Reduce, but the final result is delivered to every process. There is no `root` argument.

Example:

```c
MPI_Allreduce(&x, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
```

Every process gets the total sum.

### Collective vs point-to-point

| Function | What it does |
|---|---|
| `MPI_Send` | One process sends to one destination |
| `MPI_Recv` | One process receives from a source |
| `MPI_Bcast` | One root sends same data to everyone |
| `MPI_Scatter` | Root distributes different pieces |
| `MPI_Gather` | Root collects pieces from everyone |
| `MPI_Reduce` | Combines values; result at root |
| `MPI_Allreduce` | Combines values; result at everyone |

Collective calls (`Bcast`, `Scatter`, `Gather`, `Reduce`, `Allreduce`) must be called by all processes in the communicator in a matching order.

---

# 2. OpenMP essentials

OpenMP = shared-memory parallel programming. Threads run inside one process and share the program's memory.

## Parallel region

```c
#pragma omp parallel
{
    // This block runs on multiple threads
}
```

Creates a team of threads. Each thread runs the block.

## Parallel for

```c
#pragma omp parallel for
for (int i = 0; i < n; i++) {
    // loop iterations are divided among threads
}
```

Splits loop iterations across threads.

## Static scheduling with chunk size 2

```c
#pragma omp parallel for schedule(static, 2)
for (int i = 0; i < n; i++) {
    // each assigned chunk contains up to 2 iterations
}
```

- `static`: assign loop chunks in a planned, round-robin way.
- `2`: chunk size is two iterations.
- Example with 2 threads and 4 iterations: thread 0 gets 0–1; thread 1 gets 2–3.
- With more chunks, a thread can receive more than one chunk.

## Thread ID

```c
int tid = omp_get_thread_num();
```

Returns the current thread's ID. IDs start at 0.

## Number of threads

Set from the shell:

```bash
OMP_NUM_THREADS=4 ./program
```

Requests four OpenMP threads for that run.

## Tasks

```c
#pragma omp task
x = work1();

#pragma omp task
y = work2();

#pragma omp taskwait
```

- `task`: creates a task that the OpenMP runtime can schedule on a thread.
- `taskwait`: waits for child tasks created by the current task to finish.
- `shared(x)`: tells OpenMP that tasks use the shared variable `x`.

A common pattern for task code:

```c
#pragma omp parallel
{
    #pragma omp single
    result = fib(n);
}
```

- `parallel`: creates the thread team.
- `single`: only one thread starts the task-generating function.
- Other threads can help execute tasks.

## Reduction

```c
#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < n; i++)
    sum += a[i];
```

Each thread gets a private partial `sum`; OpenMP combines them using `+` at the end. This avoids a race when summing.

Common reduction operators: `+`, `*`, `-`, `&`, `|`, `^`, `&&`, `||`, `min`, `max` (support depends on OpenMP version/compiler).

## Timing

```c
double start = omp_get_wtime();
// code to time
double elapsed = omp_get_wtime() - start;
```

`omp_get_wtime()` returns wall-clock time in seconds.

---

# 3. Compile and run on Arch Linux / Arch WSL

Install tools once:

```bash
sudo pacman -Syu
sudo pacman -S --needed base-devel gcc openmpi unzip
```

Check tools:

```bash
gcc --version
mpicc --version
mpiexec --version
```

## Compile and run one OpenMP program

```bash
gcc -fopenmp 7.c -o p7
OMP_NUM_THREADS=4 ./p7
```

For Program 8:

```bash
gcc -fopenmp 8.c -o p8
OMP_NUM_THREADS=4 ./p8
```

For Program 9:

```bash
gcc -fopenmp 9.c -o p9
OMP_NUM_THREADS=4 ./p9
```

`-fopenmp` enables OpenMP during compilation and links its runtime.

## Compile and run one MPI program

```bash
mpicc 1.c -o p1
mpiexec --oversubscribe -n 4 ./p1
```

Other MPI programs:

```bash
mpicc 3.c -o p3 && printf "25\n" | mpiexec --oversubscribe -n 4 ./p3
mpicc 4.c -o p4 && mpiexec --oversubscribe -n 4 ./p4
mpicc 5.c -o p5 && mpiexec --oversubscribe -n 4 ./p5
```

- `mpicc`: MPI compiler wrapper; it supplies MPI include and link flags.
- `mpiexec`: launches MPI processes.
- `-n 4`: launch four processes.
- `--oversubscribe`: allows more processes than OpenMPI's detected available slots if needed.

## Run every selected program

From the folder containing the files:

```bash
chmod +x run_all.sh
./run_all.sh
```

The script compiles and runs the selected programs in order: 1, 3, 4, 5, 7, 8, 9.

## If you write a new program

1. Save it as a `.c` file, for example `myprog.c`.
2. For OpenMP:

   ```bash
   gcc -fopenmp myprog.c -o myprog
   OMP_NUM_THREADS=4 ./myprog
   ```

3. For MPI:

   ```bash
   mpicc myprog.c -o myprog
   mpiexec --oversubscribe -n 4 ./myprog
   ```

4. For a hybrid MPI + OpenMP program:

   ```bash
   mpicc -fopenmp myprog.c -o myprog
   OMP_NUM_THREADS=2 mpiexec --oversubscribe -n 4 ./myprog
   ```

   This launches 4 MPI processes, each allowed 2 OpenMP threads (up to 8 threads total).

Remember: use `gcc -fopenmp` for OpenMP, `mpicc` for MPI, and `mpiexec -n N` to launch MPI processes.
