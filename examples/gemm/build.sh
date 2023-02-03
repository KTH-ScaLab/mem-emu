icpc -O3 -o gemm gemm.cpp  -DMKL_ILP64  -I"${MKLROOT}/include"  -L${MKLROOT}/lib/intel64 -lmkl_intel_ilp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl
