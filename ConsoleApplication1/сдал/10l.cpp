#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;


int* read_array(int len)
{
	int* arr = new int[len];
	for (int i = 0; i < len; i++) {
		cin >> arr[i];
	}
	printf("m allredy read\n");
	return arr;
}



void print_array(int arr[], int len)
{
	for (int i = 0; i < len; i++) {
		cout << arr[i] << " ";
	}
}
void print_arrayF(float arr[], int len)
{
	for (int i = 0; i < len; i++) {
		cout << arr[i]<<"\n";
	}
}


//int ppe(int A[], int len)
//{
//	int k;
//	for (int i = 0; i < len; i++) {
//		if (A[i] > 0)
//		{
//			k = i + 1;
//		}
//	}
//	return k;
//}


float* my_task(int len, int A[], int B[], float C[])
{
	for (int i = 0; i < len; i++) {
		C[i] = sqrt(abs(A[i]) + (abs(B[i])));
	}
	return C;
}


void task2(int len, float C[], int A[], int B[]) {
	int kk = 0,k=0, d1 = -1, d2 = -1, d3 = -1, d4 = -1, k1 = -1, k2 = -1, k3 = -1,net=1;
	float d;
	setlocale(0,"");
	//d - уже записана k1
	//k1 первая идентичкая данной и тд k2 k3 
	for (int i = 0; i < len; i++) {
		for (int i2 = 0; i2 < len; i2++) {
			if (C[i] == C[i2]) {
				if (d1 == -1) {
					kk += 1;
					k1 = i2;
					d1 = 1;
				} else 
					if (d2 == -1) {
					kk += 1;
					k2 = i2;
					d2 = 1;
				} else 
				    if (d3 == -1) {
						kk += 1;
						k3 = i2;
						d3 = 1;
				}
			}
		}
		if (kk == 3) {
			break;
			//kk 3 одинаковых(или 4 ????)
		}
	}
	d1 = -1; d2 = -1; d3 = -1; d4 = -1;
	// обнулить
// если 3 однинаковых 
	if (kk == 3) {
		if ((A[k1] < 0 and B[k1] < 0) and d1 < 0) {
			d1 = 1;
			//k++;
			k =k+1;
		}
		if ((A[k1] > 0 and B[k1] < 0) and d2 < 0) {
			d2 = 1;
			//k++;
			k = k + 1;
		}
		if ((A[k1] < 0 and B[k1] > 0) and d3 < 0) {
			d3 = 1;
			//k++;
			k = k + 1;
		}
		if ((A[k1] > 0 and B[k1] > 0) and d4 < 0) {
			d4 = 1;
			//k++;
			k = k + 1;
		}
		// удаляет четверь и защитывает как 1 точку


		if ((A[k2] < 0 and B[k2] < 0) and d1 < 0) {
			d1 = 1;
			//k++;
			k = k + 1;
		}
		if ((A[k2] > 0 and B[k2] < 0) and d2 < 0) {
			d2 = 1;
			//k++;
			k = k + 1;
		}
		if ((A[k2] < 0 and B[k2] > 0) and d3 < 0) {
			d3 = 1;
			//k++;
			k = k + 1;
		}
		if ((A[k2] > 0 and B[k2] > 0) and d4 < 0) {
			d4 = 1;
			//k++;
			k = k + 1;
		}
		// удаляет четверь и защитывает как 2 точку


		// удаляет четверь и защитывает как 3 точку(K++)
		if ((A[k3] < 0 and B[k3] < 0) and d1 < 0) {
			d1 = 1;
			//k++;
			k = k + 1;
		}
		if ((A[k3] > 0 and B[k3] < 0) and d2 < 0) {
			d2 = 1;
			//k++;
			k = k + 1;
		}
		if ((A[k3] < 0 and B[k3] > 0) and d3 < 0) {
			d3 = 1;
			//k++;
			k = k + 1;
		}
		if ((A[k3] > 0 and B[k3] > 0) and d4 < 0) {
			d4 = 1;
			//k++;
			k = k + 1;
		}
	}
	if (k==3) {
		printf("%d %d \n", A[k1], B[k1]); // корды 1 точки(если они в разных четвяртях)
		printf("%d %d \n", A[k2], B[k2]); //корды 2
		printf("%d %d \n", A[k3], B[k3]); //корды 3
	}
	else {
		printf("таких точек нет\n");//какого то всё равно пишет в любой ситуации
	}
}

int comp1(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}


int comp2(const void* a, const void* b)
{
	return (*(int*)b - *(int*)a);
}

int main()
{
	int len, c;
	scanf_s("%d", &len);
	int* arrA = new int[len];
	arrA = read_array(len);
	int* arrB = new int[len];
	arrB = read_array(len);
	float* arrC = new float[len];

	arrC = my_task(len, arrA, arrB, arrC);

	task2(len, arrC, arrA, arrB);

	qsort(arrC, len, sizeof(float), comp1);
	//print_arrayF(arrC, len);

}

