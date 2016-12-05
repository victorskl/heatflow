#!/bin/bash
#SBATCH --job-name=HeatFlow
#SBATCH -p main
#SBATCH --nodes=8
#SBATCH --ntasks=8
#SBATCH --time=00:1:59

echo 'Number of cpu on node: ' $SLURM_CPUS_ON_NODE
echo ' '
echo 8 node 8 task

time srun heatflow_gauss_mpi.exe 1000 1000 500 500
echo ' '

