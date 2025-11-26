#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;
double f(double x) {
	return 2*x*x-3*x+1 ;
}
int main()
{
	float a, b, s, x;
	scanf_s("%f%f%f", &a, &b, &s);
	x = a;
	while (x <= b)
	{
		cout << setw(10)<< fixed << x;
		//printf("%10f", x);
		cout << setw(15) << scientific << f(x) << endl;
		//printf("%15f\n",f(x));
		x += s;
	}
}


//setprecision(5)