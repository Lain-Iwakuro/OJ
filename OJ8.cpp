#include<stdio.h>
#include<complex>
#include<iostream>
#define PI 3.1415926
#define complex std::complex<double>
#pragma warning(disable:4996)
#pragma warning(disable:6031)
int N;
double** f = new double* [7];
double error = 2e-4;
complex fun(complex c, int floor) {
	complex ans(0, 0);
	for (int i = 0; i <= N; i++) {
		ans += pow(c, i) * f[floor][i];
	}
	return ans;
}
bool judge(complex* w, int N) {
	static int times = 1300;// -(N > 10 && N < 99) * 600;
	if (times-- > 0) return false;
	else return true;
	/*
	for (int i = 0; i < N; i++) {
		if (abs(w[i]) > error) return false; // <<<<<<<<<<<<<<<<<<<<<<
	}
	return true;
	*/
}
int main() {
	// 输入
	scanf("%d", &N);
	if (N > 20) error = 1e-7;
	for (int i = 0; i < 7; i++) {
		f[i] = new double[N + 1];
	}
	for (int i = N; i >= 0; i--) {
		scanf("%lf", f[0] + i);
	}
	// 求导
	for (int i = 1; i < 7; i++) {
		for (int j = 0; j <= N; j++) {
			f[i][j] = (j < N) ? ((double)j + 1) * f[i - 1][j + 1] : 0;
		}
	}
	// 初始化Aberth向量
	complex* aber = new complex[N];
	double r = 2; // ?
	for (int i = 0; i < N; i++) {
		aber[i] = exp(complex(1, (2 * PI * (double)i - 1) / N)) - f[0][N - 1] / f[0][N] / N;
	}
	// 判断重根
	complex* w = new complex[N];
	do
	{
		for (int i = 0; i < N; i++) {
			complex tmp = fun(aber[i], 0) / fun(aber[i], 1);
			complex sum(0, 0);
			for (int j = 0; j < N; j++) {
				if (j != i) sum += (complex)1 / (aber[i] - aber[j]);
			}
			w[i] = tmp / ((complex)1 - tmp * sum );
		}
		for (int i = 0; i < N; i++) {
			aber[i] -= w[i];
		}
	}
	while (!judge(w, N));
	// 判断根的重数
	int* floor = new int[N];
	for (int i = 0; i < N; i++) {
		floor[i] = 0;
		for (int j = 0; j < N; j++) {
			if (j != i && abs(aber[j] - aber[i]) < 5e-2) floor[i]++; // <<<<<<<<<<<<<<<<<<<<<<
		}
	}
	// 牛顿迭代法
	/*
	int num = 0;
	bool* flags = new bool[N];
	for (int i = 0; i < N; i++) {
		if (aber[i].imag() + 1e-1 > 0) {
			flags[i] = true;
			num++;
		}
		else flags[i] = false;
	}
	/*
	int count = 0;
	for (int i = 0; i < N; i++) {
		if (flags[i]) {
			aber[count] = aber[i];
			floor[count] = floor[i];
			count++;
		}
		//if (count == num) break;
	}
	//*/
	/*
	for (int i = 0; i < N; i++) {
		std::cout << aber[i] << std::endl;
	}
	//*/
	int m = 950;
	//int times = 1000 - (N > 10) * m + (N > 99) * m;
	int times = 5;
	do {
		for (int i = 0; i < N; i++) {
			//if(flags[i])	
				aber[i] -= fun(aber[i], floor[i]) / fun(aber[i], floor[i] + 1);
		}
	} while (times-- > 0); // <<<<<<<<<<<<<<<<<<<<<<

	// 排序 有更改！
	for (int i = 0; i <= N; i++) {
		for (int j = i + 1; j < N; j++) {
			if ((aber[j].real() > aber[i].real() + 1e-7) || ((fabs(aber[j].real() - aber[i].real()) < 1e-7) && (fabs(aber[j].imag()) > fabs(aber[i].imag()) + 1e-7))) {
				complex temp = aber[j];
				aber[j] = aber[i];
				aber[i] = temp;
			}
		}
	}
	/*
	for (int i = 0; i < N; i++) {
		std::cout << aber[i] << std::endl;
	}
	//*/
	// 输出
	for (int i = 0; i < N; i++) {
		if (fabs(aber[i].imag()) < 1e-6)	printf("%lf %lf\n", aber[i].real(), aber[i].imag());
		else if (aber[i].imag() > 1e-6)	printf("%lf %lf\n%lf %lf\n", aber[i].real(), aber[i].imag(), aber[i].real(), -aber[i].imag());
	}
	return 0;
}