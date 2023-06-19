#include<stdio.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)
int main()
{
	int n, m;
	char i, j, a[8] = { 0 };
	scanf("%d", &n);
	if (n <= 1)
	{
		printf("%d", 10 - 9 * n);
		return 0;
	}
	for (i = 9; i > 1; i--)
		while (n % i == 0)
		{
			n /= i;
			a[i - 2]++;
		}
	if (n != 1)	printf("-1");
	else
		for (i = 2; i < 10; i++)
			for (j = 0; j < a[i - 2]; j++)
				printf("%d", i);
	return 0;
}