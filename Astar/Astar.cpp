#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

int xEnd = 0, yEnd = 0, xStart = 0, yStart = 0;
size_t xSize = 0, ySize = 0;
int warningstate = 0;
double G = 0, cost = 1;

class Node
{
public:
	int x, y, xP, yP;
	double F, H;
	Node(int X, int Y, int xp, int yp) : x(X), y(Y), xP(xp), yP(yp) {
		calculateH();
	}
	double calculateF(double G) {
		F = H + G;
		return F;
	}
	double calculateH() {
		H = sqrt((x - xEnd)*(x - xEnd) + (y - yEnd)*(y - yEnd));
		return H;
	}
};

vector<Node> algorithm(vector<int> &grid, int xCurrent, int yCurrent);
void newNode(int xCurrent, int yCurrent, vector<Node> &open, vector<Node> &closed, vector<int> grid, int direction);
double calculateG(vector<Node> parents, int xCurrent, int yCurrent);
vector<Node> createPath(vector<Node> &path, vector<Node> parents, int x, int y);

int main(void) {
	xStart = 0, yStart = 0;

	ifstream gridFile("grid.txt");
	if (!gridFile.is_open()) return -1;
	string line;
	vector<int> grid;
	while (getline(gridFile, line)) {
		istringstream sstream(line);
		int digit;
		char delimiter;
		xSize = 0;
		ySize++;
		while (sstream >> digit) {
			grid.push_back(digit);
			xSize++;
			sstream >> delimiter;
		}
	}
	gridFile.close();

	xEnd = xSize - 1;
	yEnd = ySize - 1;

	grid[yStart*xSize + xStart] = 1;
	vector<Node> path = algorithm(grid, xStart, yStart);

	for (int i = 0; i < path.size(); i++) {
		grid[path[i].y*xSize + path[i].x] = 3;
	}
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int Colors[6] = { 8,3,0,6,0,4 };

	if (warningstate != 1)
		for (int i = 0; i < ySize; i++) {
			for (int j = 0; j < xSize; j++) {
				SetConsoleTextAttribute(hConsole, Colors[grid[i*xSize + j]]);
				cout << grid[i*xSize + j];
			}
			cout << endl;
		}
}

vector<Node> algorithm(vector<int> &grid, int xCurrent, int yCurrent) {
	vector<Node> open;
	vector<Node> closed;

	while ((xCurrent != xEnd || yCurrent != yEnd)) {
		double minF = DBL_MAX;
		int posInVector = 0;
		
		if (xCurrent + 1 < xSize) {							//E
			newNode(xCurrent, yCurrent, open, closed, grid, 0);
		}
		if (xCurrent + 1 < xSize && yCurrent + 1 < ySize) { //SE
			newNode(xCurrent, yCurrent, open, closed, grid, 4);
		}
		if (yCurrent + 1 < ySize){							//S
			newNode(xCurrent, yCurrent, open, closed, grid, 1);
		}
		if (xCurrent - 1 >= 0 && yCurrent + 1 < ySize) {	//SW
			newNode(xCurrent, yCurrent, open, closed, grid, 5);
		}
		if (xCurrent - 1 >= 0){								//W
			newNode(xCurrent, yCurrent, open, closed, grid, 2);
		}
		if (xCurrent - 1 >= 0 && yCurrent - 1 >= 0) {		//NW
			newNode(xCurrent, yCurrent, open, closed, grid, 6);
		}
		if (yCurrent - 1 >= 0) {							//N
			newNode(xCurrent, yCurrent, open, closed, grid, 3);
		}
		if (xCurrent + 1 < xSize && yCurrent - 1 >= 0) {	//NE
			newNode(xCurrent, yCurrent, open, closed, grid, 7);
		}

		if (open.empty()) {
			break;
		}

		for (int j = 0; j < open.size(); j++)
			if (minF >= open[j].F) {
				if (minF == open[j].F) {
					if (open[j].H < open[posInVector].H)
						posInVector = j;
				}
				else {
					minF = open[j].F;
					posInVector = j;
				}
			}

		closed.push_back(open[posInVector]);
		xCurrent = open[posInVector].x;
		yCurrent = open[posInVector].y;
		open.erase(open.begin() + posInVector);
	}

	vector<Node> path;

	if (xCurrent == xEnd && yCurrent == yEnd) {
		path = createPath(path, closed, xEnd, yEnd);
	}
	else {
		warningstate = 1;
	}
	return path;
}

void newNode(int xCurrent, int yCurrent, vector<Node> &open, vector<Node> &closed, vector<int> grid, int direction) {
	int xNext = 0 + xCurrent, yNext = 0 + yCurrent;
	switch (direction) {
	case 0:
		xNext++;
		break;
	case 1:
		yNext++;
		break;
	case 2:
		xNext--;
		break;
	case 3:
		yNext--;
		break;
	case 4:
		xNext++;
		yNext++;
		break;
	case 5:
		xNext--;
		yNext++;
		break;
	case 6:
		xNext--;
		yNext--;
		break;
	case 7:
		xNext++;
		yNext--;
		break;
	}

	auto itNode = find_if(closed.begin(), closed.end(), [&xNext, &yNext](const Node& node) {return node.x == xNext && node.y == yNext; });
	if (grid[yNext*xSize + xNext] != 5 && !(itNode != closed.end())) {
		auto itNodeOpen = find_if(open.begin(), open.end(), [&xNext, &yNext](const Node& node) {return node.x == xNext && node.y == yNext; });
		Node nodeRight(xNext, yNext, xCurrent, yCurrent);
		if (itNodeOpen != open.end()) {
			nodeRight.F = nodeRight.calculateF(calculateG(closed, xCurrent, yCurrent));
			auto index = distance(open.begin(), itNodeOpen);
			(nodeRight.F < open[index].F) ? open.erase(open.begin() + index), open.push_back(nodeRight) : "";
		}
		else {
			nodeRight.F = nodeRight.calculateF(calculateG(closed, xCurrent, yCurrent));
			open.push_back(nodeRight);
		}
		G = 0;
	}
}

double calculateG(vector<Node> parents, int xCurrent, int yCurrent) {
	auto itParent = find_if(parents.begin(), parents.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent && node.y == yCurrent; });
	if (itParent != parents.end()) {
		auto index = distance(parents.begin(), itParent);
		if (xCurrent != xStart || yCurrent != yStart) {
			if (xCurrent != parents[index].xP && yCurrent != parents[index].yP) {
				cost = sqrt(2*pow(cost,2));
			}
			else {
				cost = 1;
			}
			G += cost;
			return calculateG(parents, parents[index].xP, parents[index].yP);
		}
	}
	return G;
}

vector<Node> createPath(vector<Node> &path, vector<Node> parents, int x, int y) {
	auto itParent = find_if(parents.begin(), parents.end(), [&x, &y](const Node& node) {return node.x == x && node.y == y; });
	if (itParent != parents.end()) {
		auto index = distance(parents.begin(), itParent);
		if (x != xStart || y != yStart) {
			path.push_back(parents[index]);
			return createPath(path, parents, parents[index].xP, parents[index].yP);
		}
	}
	return path;
}