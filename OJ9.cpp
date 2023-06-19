#include<stdio.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)
int n, sum = 0;
inline int max(int a, int b) { return a > b ? a : b; }
int main() {
	// 输入，初始化
	scanf("%d", &n);
	int* a = new int[n];
	for (int i = 0; i < n; i++) {
		scanf("%d", a + i);
		sum += a[i];
	}
	// 判断特殊情况
	int count = 0;
	for (int i = 0; i < n; i++) { // 计数技术好(>0)的铁匠数
		if (a[i] >= 0) count++;
	}
	if (count < 3) { // 若少于三个，选三个技术最不烂的
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
	// 正着算一遍
	int* b1 = new int[n];
	int* b2 = new int[n];
	int* c1 = new int[n];
	b1[0] = 0;
	c1[0] = a[0];
	for (int i = 1; i < n; i++) { // 一段
		b1[i] = max(b1[i - 1], c1[i - 1]); // 前i个元素，不取第i个，至多j段的最大值
		c1[i] = max(c1[i - 1], 0) + a[i]; // 前i个元素，取第i个，至多j段的最大值
	}
	for (int j = 0; j < 2; j++) { // 两段、三段
		for (int i = 0; i < n; i++) {
			b2[i] = b1[i];
		}
		for (int i = 1; i < n; i++) {
			b1[i] = max(b1[i - 1], c1[i - 1]); // 前i个元素，不取第i个，至多j段的最大值
			c1[i] = max(c1[i - 1], b2[i - 1]) + a[i]; // 前i个元素，取第i个，至多j段的最大值
		}
	}
	int max1 = max(b1[n - 1], c1[n - 1]);
	// 反着算一遍
	for (int i = 0; i < n; i++) {
		a[i] = -a[i];
	}
	c1[0] = a[0];
	for (int i = 1; i < n; i++) { // 一段
		b1[i] = max(b1[i - 1], c1[i - 1]); // 前i个元素，不取第i个，至多j段的最大值
		c1[i] = max(c1[i - 1], 0) + a[i]; // 前i个元素，取第i个，至多j段的最大值
	}
	for (int j = 0; j < 2; j++) { // 两段、三段
		for (int i = 0; i < n; i++) {
			b2[i] = b1[i];
		}
		for (int i = 1; i < n; i++) {
			b1[i] = max(b1[i - 1], c1[i - 1]); // 前i个元素，不取第i个，至多j段的最大值
			c1[i] = max(c1[i - 1], b2[i - 1]) + a[i]; // 前i个元素，取第i个，至多j段的最大值
		}
	}
	int max2 = sum + max(b1[n - 1], c1[n - 1]);
	printf("%d", max(max1, max2)); // 输出
	delete[] a, b1, b2, c1;
	return 0;
}