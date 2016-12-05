#!/bin/bash

rm *.exe

gcc heatflow_red_black_sequential.cpp -o heatflow_red_black_sequential.exe
gcc heatflow_sequential.cpp -o heatflow_sequential.exe
gcc heatflow_gauss_sequential.cpp -o heatflow_gauss_sequential.exe
gcc -fopenmp heatflow_omp.cpp -o heatflow_omp.exe
gcc -fopenmp heatflow_red_black_omp.cpp -o heatflow_red_black_omp.exe
mpic++ heatflow_gauss_mpi.cpp -o heatflow_gauss_mpi.exe
