Selected experiments from the new lab index:
1, 3, 4, 5, 7, 8, 9.
Excluded: 2 (deadlock) and 6 (mergesort).

Files: 1.c, 3.c, 4.c, 5.c, 7.c, 8.c, 9.c, run_all.sh

Arch WSL setup:
  sudo pacman -Syu
  sudo pacman -S --needed base-devel gcc openmpi unzip

Run all:
  chmod +x run_all.sh
  ./run_all.sh

The script uses four MPI processes and four OpenMP threads.
Inputs: Program 3 = 25; Program 7 = 8; Program 8 = 10; Program 9 = 500.
MPI output order and timing values may vary.
