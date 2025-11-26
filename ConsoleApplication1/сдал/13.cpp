#include <iostream>
#include <cmath>

using namespace std;
double ai(double i)
{
	return i/(i+1);
}


int main()
{
	double s;
	int i,n;
	scanf_s("%d", &n);
	s = 0.0;
	for (i = 1; i <= n; i++) 
	{
		s=s+ai(i);
	}
	s = round(s * 100) / 100;
	printf("%3.2f", s);
}
