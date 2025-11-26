#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int comp1(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}
int comp2(const void* a, const void* b)
{
	return (*(int*)b - *(int*)a);
}

float* read_arrayF(int len)
{
	float* arr = new float[len];
	for (int i = 0; i < len; i++) {
		cin >> arr[i];
	}
	printf("m allredy read\n");
	return arr;
}
void sort(float arr[], int len, bool asc) {
	if (asc) {
		qsort(arr, len, sizeof(float), comp1);
	}else{ qsort(arr, len, sizeof(float), comp2); }
}




void print_arrayF(float arr[], int len)
{
	for (int i = 0; i < len; i++) {
		cout << arr[i] << " ";
	}
}


int main()
{
	int len;
	char asc;
	bool c;
	scanf_s("%d", &len);
	float* arr = new float[len];
	cin >> c;
	arr = read_arrayF(len);
	sort(arr, len, c);
	print_arrayF(arr, len);
}
