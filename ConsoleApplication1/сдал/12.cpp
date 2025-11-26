

#include <iostream>
#include <locale>
#include <math.h>
void print_rect(int x, int y, int sx, int sy) {
	int i1, i2;
	for (i1 = 1 + 1; i1 <= y; i1++)
	{
		printf(" \n");
	}
	for (i2 = 1 + 1; i2 <= x; i2++)
	{
		printf(" ");
	}
	for (i1 = 1; i1 <= sy; i1++) {
		for (i2 = 1; i2 <= sx; i2++) {
			printf("@");
		}
		printf("\n");
	}
}
int main() {
	int x, y, sx, sy, i1,i2;
	scanf_s("%d%d%d%d", &x,&y,&sx,&sy);
	print_rect(x, y, sx, sy);
}


