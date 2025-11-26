#include <iostream>
#include <cmath>

using namespace std;
float ai(int i,float x)
{
	return 2*pow(x,i);
}


int main()
{
	float x,p;
	int i,n;
	scanf_s("%d%f", &n,&x);
	p = 1;
	for (i = 1; i <= n; i++) 
	{
		p=p*ai(i,x);
	}
	printf("%f", p);
}
