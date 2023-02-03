#!/bin/bash

set -x
export OMP_NUM_THREADS=14

# Baseline
./gemm 20000

# Memory profiling
../emu -m -t 0 ./gemm 20000
../emu -m -t 1 ./gemm 20000

# Numactl
numactl -N 0 -m 0 ./gemm 20000
numactl -N 0 -m 1 ./gemm 20000

# Emulator
../emu -t 1 ./gemm 20000
../emu -l 0 -t 1 ./gemm 20000
