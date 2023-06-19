#include<stdio.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)
int main()
{
	bool flag = true;
	int N, m, i, j;
	int max(int a, int b);
	int max(int a, int b, int c);
	int* w = new int[30000];
	int* w_t[2];
	w_t[0] = new int[30000];
	w_t[1] = new int[30000];
	scanf("%d%d", &N, w);
	for (i = 1; i < N; i++)
	{
		scanf("%d", w + i);
		w_t[0][i - 1] = max(w[i - 1], w[i]);
	}
	if(N == 2)
	{
		printf("%d", max(w[0], w[1]));
		return 0;
	}
	for (m = 4; m <= N; m = m + 2, flag = !flag)
		for (i = 0; i < N - m + 1; i++)
			w_t[flag][i] = max(w[i] + w_t[!flag][i + 2], max(w[i], w[i + m - 1]) + w_t[!flag][i + 1], w[i + m - 1] + w_t[!flag][i]);
	printf("%d", w_t[!flag][0]);
	delete[]w_t[0];
	delete[]w_t[1];
	return 0;
}
int max(int a, int b)
{
	if (a > b)	return a; 
	else	return b;
}
int max(int a, int b, int c)
{
	return max(max(a, b), c);
}