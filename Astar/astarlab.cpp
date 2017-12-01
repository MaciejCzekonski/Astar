// Astar.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;

const int stepcost = 1; // step's cost
const int xend = 19, yend = 19;
int k = 0;


void algorythm(int tda[20][20], int* xstart, int* ystart);

int main(void) {
	ifstream file{ "grid.txt" };
	if (!file.is_open()) return -1;
	int grid[20][20] = { 0 }, xstart = 0, ystart = 0;
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			file >> grid[i][j];
	while (xstart != xend || ystart != yend)
		algorythm(grid, &xstart, &ystart);

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++)
			cout << grid[i][j];
		cout << endl;
	}
}

void algorythm(int tda[20][20], int* xstart, int* ystart) {
	double H = 0, F0 = 0, F1 = 0, F2 = 0, F3 = 0, smallest = 100, Fvalues[4] = { 0 };
	int i = 0, j = 0;
	if (tda[*ystart][*xstart] == 0)
		tda[*ystart][*xstart] = 3;
	else
		switch (k) {
		case 1:
			tda[*ystart][*xstart] = 2;
			tda[*ystart - 1][*xstart] = 2;
			break;
		case 2:
			tda[*ystart][*xstart] = 2;
			tda[*ystart][*xstart - 1] = 2;
			break;
		case 3:
			tda[*ystart][*xstart] = 2;
			tda[*ystart + 1][*xstart] = 2;
			break;
		case 4:
			tda[*ystart][*xstart] = 2;
			tda[*ystart][*xstart + 1] = 2;
			break;
		}
	if (*ystart + 1 < 20 && tda[*ystart + 1][*xstart] != 5 && tda[*ystart + 1][*xstart] != 2) {
		H = sqrt((*ystart + 1 - yend)*(*ystart + 1 - yend) + (*xstart - xend)*(*xstart - xend));
		F0 = stepcost + H;
		Fvalues[0] = F0;
	}
	if (*xstart + 1 < 20 && tda[*ystart][*xstart + 1] != 5 && tda[*ystart][*xstart + 1] != 2) {
		H = sqrt((*ystart - yend)*(*ystart - yend) + (*xstart + 1 - xend)*(*xstart + 1 - xend));
		F1 = stepcost + H;
		Fvalues[1] = F1;
	}
	if (*ystart - 1 >= 0 && tda[*ystart - 1][*xstart] != 5 && tda[*ystart - 1][*xstart] != 2) {
		H = sqrt((*ystart - 1 - yend)*(*ystart - 1 - yend) + (*xstart - xend)*(*xstart - xend));
		F2 = stepcost + H;
		Fvalues[2] = F2;
	}
	if (*xstart - 1 >= 0 && tda[*ystart][*xstart - 1] != 5 && tda[*ystart][*xstart - 1] != 2) {
		H = sqrt((*ystart - yend)*(*ystart - yend) + (*xstart - 1 - xend)*(*xstart - 1 - xend));
		F3 = stepcost + H;
		Fvalues[3] = F3;
	}
	for (i = 0; i < 4; i++)
		if (Fvalues[i] < smallest && Fvalues[i] > 0) {
			smallest = Fvalues[i];
		}
	for (i = 0; i < 4; i++)
		if (smallest == Fvalues[i]) {
			switch (i) {
			case 0:
				*ystart += 1;
				k = 1;
				break;
			case 1:
				*xstart += 1;
				k = 2;
				break;
			case 2:
				*ystart -= 1;
				k = 3;
				break;
			case 3:
				*xstart -= 1;
				k = 4;
				break;
			}
			break;
		}
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++)
			cout << tda[i][j];
		cout << endl;
	}

	for (int i = 0; i < 10; i++)
		cout << endl;

	return;
}

