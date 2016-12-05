#!/bin/bash
#SBATCH --job-name=HeatFlow
#SBATCH -p main
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:9:59

echo 'Number of cpu on node: ' $SLURM_CPUS_ON_NODE
echo ' '


#time ./heatflow_sequential.exe 1000 1000 500 500
#time ./heatflow_red_black_sequential.exe 1000 1000 500 500
time ./heatflow_gauss_sequential.exe 1000 1000 500 500
echo ' '

