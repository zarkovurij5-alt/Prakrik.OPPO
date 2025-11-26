#include <iostream>
#include <cmath>

using namespace std;
float xd(float x,int s)
{
	return ( pow(x,s)/s ) ;
}
int main()
{
	int i,s;
	float x,b,f,S,pi;
	scanf_s("%f%f", &x, &b);
	pi = 3.14; s = 1; S = 1; i = 1;
	f =pi/2- atan(x);
	while (abs(f - S) >= b)
	{
		s =s+ 2;
		if (i == 1)
		{
			S =S -xd(x, s);
			i = 0;
		}
		else
		{
			S =S + xd(x, s);
			i = 1;
		}
	}
	printf("%f %f %f",S, f, abs(f - S));
}
