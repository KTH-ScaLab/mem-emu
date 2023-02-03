#include <random>
#include <mkl.h>
#include <time.h>
#include <signal.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: ./gemm N\n");
		return 1;
	}

	int n = atoi(argv[1]);

	printf("Initalizing...\n");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1.0, 1.0);

	double *a = new double[n*n];
	double *b = new double[n*n];
	double *c = new double[n*n];

	for (int i = 0; i < n*n; i++) {
		a[i] = dis(gen);
		b[i] = dis(gen);
		c[i] = dis(gen);
	}


	int iter = 1;

	printf("Warmup...\n");

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, a, n, b, n, 1.0, c, n);

	struct timespec t0, t1;
	printf("Timing...\n");

	//raise(SIGSTOP);

	clock_gettime(CLOCK_MONOTONIC, &t0);
	for (int i = 0; i < iter; i++)
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, a, n, b, n, 1.0, c, n);
	clock_gettime(CLOCK_MONOTONIC, &t1);

	//raise(SIGSTOP);

	double t = ((t1.tv_sec - t0.tv_sec) + 1e-9*(t1.tv_nsec - t0.tv_nsec)) / iter;

	printf("%lf\n", t);

	return 0;
}
