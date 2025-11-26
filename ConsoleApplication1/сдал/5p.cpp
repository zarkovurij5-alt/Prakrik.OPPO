#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	int x, n, i,i1;
	scanf_s("%d", &x);
	n = x;
	for (i = ceil(x/2); i <= n; i++)
	{
			printf("%d", i);
			for (i1 = 1; i1 <= i; i1++)
			{
				printf("*");
			}
			printf("\n");
	}
}
