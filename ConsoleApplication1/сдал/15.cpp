#include <iostream>
#include <cmath>

using namespace std;
int ai(int i)
{
	int s1;
	if ( (i%2)==0 ) {s1 = 1;}
	else { s1 = -1; }

	return i*s1;
}


int main()
{
	int i,n,s;
	scanf_s("%d", &n);
	s = 0;
	for (i = 1; i <= n; i++) 
	{
		s=s+ai(i);
	}
	printf("%d", s);
}
