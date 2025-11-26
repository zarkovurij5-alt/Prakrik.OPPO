#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	float k1, k2, k3;
	int x1, y1, x2, y2, x3, y3;
	scanf_s("%d%d%d%d%d%d", &x1, &y1, &x2, &y2, &x3, &y3);
	k1 =(x3-x1)/(x2-x1);
	k2 = (y3 - y1) / (y2 - y1);
	if ( k1==k2)
	{
		printf("Yes");
	}
	else {
		printf("No");
	}
}
