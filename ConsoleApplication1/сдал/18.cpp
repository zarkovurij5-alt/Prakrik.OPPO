#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double f(double x) {
	double p; p = 3.14;
	return pow(sin(p*x),2) ;
}
int main()
{
	int i,i1,v;
	double a, b, s, x,y,scale;
	scanf_s("%lf%lf%lf%lf", &a, &b, &s,&scale);
	x = a;
	while (x <= b)
	{
		y = f(x);
		v = scale * y;
		cout << setw(v) << "$(" << x << ", " << y << ")\n";
		//printf("$(%lf,%lf)\n", x, y);
		x += s;
	}
}


//setprecision(5)