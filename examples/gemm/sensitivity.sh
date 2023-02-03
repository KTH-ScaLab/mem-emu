#!/bin/bash

PROG="./gemm 20000"

set -x
export OMP_NUM_THREADS=14

# 100% local memory
../emu $PROG

# 0% local memory
../emu -l 0 $PROG

# 25%, 50%, 75% local memory
for n in 2.6575g 4.905g 7.1525g; do
	../emu -l $n $PROG
done
