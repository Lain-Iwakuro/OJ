#include<stdio.h> // ���벻�淶��ͼһ��
#pragma warning(disable:4996)
void solve(int size, char* S, char* t) {
	bool** hea = new bool* [size]; int num = 0, i, j, k, l; // ��ʼ����heaΪ�������numΪ��
	for (i = 0; i < size; i++) { hea[i] = new bool[size + 1]; hea[i][size] = (t[i] == '1');	for (j = 0; j < size; j++) hea[i][j] = (S[(i + j) % size] == '1'); }
	for (j = 0; j < size; j++) { // ��˹��Ԫ�����Ķ���
		for (k = num; (k < size) && (!hea[k][j]); k++); if (k < size) { // Ѱ�ҵ�j�еĵ�һ��1��ͨ���滻��ȷ����һ��1���������棬Ȼ���ʣ�µĸ�����
			if (k > num) for (i = 0; i <= size; i++) hea[k][i] = (hea[k][i] = (hea[k][i] ^ hea[num][i])) ^ (hea[num][i] = (hea[num][i] ^ hea[k][i]));
			for (i = 0; i < size; i++) if (hea[i][j] && (i != num)) for (l = 0; l <= size; l++)	hea[i][l] = (hea[i][l] ^ hea[num][l]); num++;
		}
	}
	for (j = size - 1; (j >= num) && (!hea[j][size]); j--); if (j >= num) printf("N"); // ����������"N"��·
	else for (i = 0; (i < size); i++) printf("%s", hea[i][size] ? "BA" : "A"); // û����������������
	printf("\n"); for (int i = 0; i < size; i++) delete[] hea[i]; delete[] hea; // �ͷſռ䣬׼���´μ���
}
int main() {
	int M, i; scanf("%d", &M); int* ns = new int[M]; char** Ss = new char* [M], ** ts = new char* [M];
	for (i = 0; i < M; i++) { scanf("%d", ns + i); Ss[i] = new char[ns[i] + 1]; ts[i] = new char[ns[i] + 1]; scanf("%s%s", Ss[i], ts[i]); }
	for (i = 0; i < M; i++) { solve(ns[i], Ss[i], ts[i]); delete[] Ss[i], ts[i]; } delete[] ns, Ss, ts; return 0; // �ͷſռ䣬����
}
