#include<stdio.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)
int n, sum = 0;
inline int max(int a, int b) { return a > b ? a : b; }
int main() {
	// ���룬��ʼ��
	scanf("%d", &n);
	int* a = new int[n];
	for (int i = 0; i < n; i++) {
		scanf("%d", a + i);
		sum += a[i];
	}
	// �ж��������
	int count = 0;
	for (int i = 0; i < n; i++) { // ����������(>0)��������
		if (a[i] >= 0) count++;
	}
	if (count < 3) { // ������������ѡ����������õ�
		int ms[3] = { -1001, -1001, -1001 }, mx[3] = { 0, 0, 0 };
		for (int i = 0; i < n; i++) {
			if (-a[i] > ms[0]) {
				ms[0] = -a[i];
				mx[0] = i;
			}
		}
		for (int i = 0; i < n; i++) {
			if (-a[i] > ms[1] && i != mx[0]) {
				ms[1] = -a[i];
				mx[1] = i;
			}
		}
		for (int i = 0; i < n; i++) {
			if (-a[i] > ms[2] && i != mx[0] && i != mx[1]) {
				ms[2] = -a[i];
				mx[2] = i;
			}
		}
		printf("%d", ms[0] + ms[1] + ms[2]);
		delete[] a;
		return 0;
	}
	// ������һ��
	int* b1 = new int[n];
	int* b2 = new int[n];
	int* c1 = new int[n];
	b1[0] = 0;
	c1[0] = a[0];
	for (int i = 1; i < n; i++) { // һ��
		b1[i] = max(b1[i - 1], c1[i - 1]); // ǰi��Ԫ�أ���ȡ��i��������j�ε����ֵ
		c1[i] = max(c1[i - 1], 0) + a[i]; // ǰi��Ԫ�أ�ȡ��i��������j�ε����ֵ
	}
	for (int j = 0; j < 2; j++) { // ���Ρ�����
		for (int i = 0; i < n; i++) {
			b2[i] = b1[i];
		}
		for (int i = 1; i < n; i++) {
			b1[i] = max(b1[i - 1], c1[i - 1]); // ǰi��Ԫ�أ���ȡ��i��������j�ε����ֵ
			c1[i] = max(c1[i - 1], b2[i - 1]) + a[i]; // ǰi��Ԫ�أ�ȡ��i��������j�ε����ֵ
		}
	}
	int max1 = max(b1[n - 1], c1[n - 1]);
	// ������һ��
	for (int i = 0; i < n; i++) {
		a[i] = -a[i];
	}
	c1[0] = a[0];
	for (int i = 1; i < n; i++) { // һ��
		b1[i] = max(b1[i - 1], c1[i - 1]); // ǰi��Ԫ�أ���ȡ��i��������j�ε����ֵ
		c1[i] = max(c1[i - 1], 0) + a[i]; // ǰi��Ԫ�أ�ȡ��i��������j�ε����ֵ
	}
	for (int j = 0; j < 2; j++) { // ���Ρ�����
		for (int i = 0; i < n; i++) {
			b2[i] = b1[i];
		}
		for (int i = 1; i < n; i++) {
			b1[i] = max(b1[i - 1], c1[i - 1]); // ǰi��Ԫ�أ���ȡ��i��������j�ε����ֵ
			c1[i] = max(c1[i - 1], b2[i - 1]) + a[i]; // ǰi��Ԫ�أ�ȡ��i��������j�ε����ֵ
		}
	}
	int max2 = sum + max(b1[n - 1], c1[n - 1]);
	printf("%d", max(max1, max2)); // ���
	delete[] a, b1, b2, c1;
	return 0;
}