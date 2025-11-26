#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double f(double x) {
	return 5*x+2 ;
}
int main()
{
	float a, b, s, x;
	scanf_s("%f%f%f", &a, &b, &s);
	x = a;
	while (x <= b)
	{
		cout << setw(15) << setprecision(3)<< fixed << x;
		x += s;
	}
	cout << endl;
	x = a;
	while (x <= b)
	{
		cout << setw(15) << setprecision(5)<< scientific << f(x);
		x += s;
	}
}


//setprecision(5)