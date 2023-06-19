#include<stdio.h> // 代码不规范，图一乐
#pragma warning(disable:4996)
void solve(int size, char* S, char* t) {
	bool** hea = new bool* [size]; int num = 0, i, j, k, l; // 初始化，hea为增广矩阵，num为秩
	for (i = 0; i < size; i++) { hea[i] = new bool[size + 1]; hea[i][size] = (t[i] == '1');	for (j = 0; j < size; j++) hea[i][j] = (S[(i + j) % size] == '1'); }
	for (j = 0; j < size; j++) { // 高斯消元，懂的都懂
		for (k = num; (k < size) && (!hea[k][j]); k++); if (k < size) { // 寻找第j列的第一个1，通过替换行确保第一个1尽量在上面，然后把剩下的给消了
			if (k > num) for (i = 0; i <= size; i++) hea[k][i] = (hea[k][i] = (hea[k][i] ^ hea[num][i])) ^ (hea[num][i] = (hea[num][i] ^ hea[k][i]));
			for (i = 0; i < size; i++) if (hea[i][j] && (i != num)) for (l = 0; l <= size; l++)	hea[i][l] = (hea[i][l] ^ hea[num][l]); num++;
		}
	}
	for (j = size - 1; (j >= num) && (!hea[j][size]); j--); if (j >= num) printf("N"); // 出问题就输出"N"跑路
	else for (i = 0; (i < size); i++) printf("%s", hea[i][size] ? "BA" : "A"); // 没问题就输出操作序列
	printf("\n"); for (int i = 0; i < size; i++) delete[] hea[i]; delete[] hea; // 释放空间，准备下次计算
}
int main() {
	int M, i; scanf("%d", &M); int* ns = new int[M]; char** Ss = new char* [M], ** ts = new char* [M];
	for (i = 0; i < M; i++) { scanf("%d", ns + i); Ss[i] = new char[ns[i] + 1]; ts[i] = new char[ns[i] + 1]; scanf("%s%s", Ss[i], ts[i]); }
	for (i = 0; i < M; i++) { solve(ns[i], Ss[i], ts[i]); delete[] Ss[i], ts[i]; } delete[] ns, Ss, ts; return 0; // 释放空间，结束
}
