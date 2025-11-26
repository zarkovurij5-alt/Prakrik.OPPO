

#include <iostream>
#include <locale>
#include <math.h>

void print_diag(int s) {
	int x1, x, y1, y,i;
	x = s; y = s;
	y1 = 5; x1 = 1;
	while (y1 <= y) {
		while (x1 <= x) {
			for (i = 1; i < x1; i++) {
				printf(" ");
			}
			printf("*\n");
			x1 += 1;
		}
		y1 += 1;
	}
}

int main() {
	int s;
	scanf_s("%d", &s);
	print_diag(s);
}
