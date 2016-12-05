#!/bin/bash
#SBATCH --job-name=HeatFlow
#SBATCH -p main
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=00:9:59

export OMP_NUM_THREAD=$SLURM_CPUS_ON_NODE
echo Threads: $OMP_NUM_THREAD
echo ' '
echo heatflow_red_black_omp.exe 1000 1000 500 500

#time ./heatflow_omp.exe 1000 1000 500 500
time ./heatflow_red_black_omp.exe 1000 1000 500 500
echo ' '

