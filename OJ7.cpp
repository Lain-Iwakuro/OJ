#include<stdio.h>
#include<math.h>
#include<iostream>
#include<iomanip>
#pragma warning(disable:4996)
#pragma warning(disable:6031)
int main() {
	// ����
	int N;
	scanf("%d", &N);
	double** M = new double* [N], ** M1 = new double* [N];
	double* x = new double[N], * y = new double[N];
	for (int i = 0; i < N; i++) {
		M[i] = new double[N];
		M1[i] = new double[N];
		y[i] = x[i] = 0;
		for (int j = 0; j < N; j++) {
			scanf("%lf", &M[i][j]);
			M1[i][j] = i == j ? 1 : 0;
		}
	}
	y[0] = x[0] = 1;
	// �����������ֵ
	double rx, ry, lambda[2] = {0, 0};
	while (true) {
		rx = 0;
		ry = 0;
		for (int i = 0; i < N; i++) { // ����ģ��ƽ��
			rx += x[i] * x[i];
			ry += y[i] * y[i];
		}
		for (int i = 0; i < N; i++)	y[i] = x[i] / sqrt(rx); // ������һ��
		lambda[1] = lambda[0];
		lambda[0] = sqrt(rx / ry);
		if (fabs(lambda[0] - lambda[1]) < 1e-8)	break; // �ȽϽ�������ֵ��������������
		for (int i = 0; i < N; i++) { // ���������
			x[i] = 0;
			for (int j = 0; j < N; j++) {
				x[i] += y[j] * M[i][j];
			}
		}
	}
	std::cout << std::setprecision(4) << lambda[0] << " "; // ����������ֵ
	// ��������
	int i, j, k, l, num = 0;
	for (j = 0; j < N; j++) {
		for (k = num; (k < N) && (fabs(M[k][j]) < 1e-10); k++); // �ҵ�j�е�һ������Ԫ��
		if (k < N) { // �ҵ���
			if (k > num) for (i = 0; i <= N; i++) { // �������ԣ��򽻻���
				double* tmp = new double[N];
				tmp[i] = M[k][i];
				M[k][i] = M[num][i];
				M[num][i] = tmp[i];
				tmp[i] = M1[k][i];
				M1[k][i] = M1[num][i];
				M1[num][i] = tmp[i];
				delete[] tmp;
			}
			double c0 = M[num][j];
			for (i = 0; i < N; i++) { // ����num�й�һ��
				M[num][i] /= c0;
				M1[num][i] /= c0;
			}
			for (i = 0; i < N; i++) if (fabs(M[i][j]) > 1e-11 && (i != num)) // ��������
			{
				double c = M[i][j] / M[num][j];
				for (l = 0; l <= N; l++) {
					M[i][l] -= c * M[num][l];
					M1[i][l] -= c * M1[num][l];
				}
			}
			num++;
		}
	}
	// ������С����ֵ
	for (int i = 0; i < N; i++) {
		y[i] = x[i] = 0;
	}
	y[0] = x[0] = 1; lambda[0] = 0; lambda[1] = 0; // ���³�ʼ��
	while (true) {
		rx = 0;
		ry = 0;
		for (int i = 0; i < N; i++) { // ����ģ��ƽ��
			rx += x[i] * x[i];
			ry += y[i] * y[i];
		}
		for (int i = 0; i < N; i++)	y[i] = x[i] / sqrt(rx); // ������һ��
		lambda[1] = lambda[0];
		lambda[0] = sqrt(rx / ry);
		if (fabs(lambda[0] - lambda[1]) < 1e-8)	break; // �ȽϽ�������ֵ��������������
		for (int i = 0; i < N; i++) { // ���������
			x[i] = 0;
			for (int j = 0; j < N; j++) {
				x[i] += y[j] * M1[i][j];
			}
		}
	}
	std::cout << std::setprecision(4) << 1 / lambda[0]; // �����С����ֵ
	return 0;
}