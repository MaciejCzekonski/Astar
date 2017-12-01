// Astar.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

const int xEnd = 19, yEnd = 19;
int warningstate = 0, G = 0;

class Node
{
public:
	int x, y, xP, yP;
	double F;
	Node(int X, int Y, int xp, int yp) : x(X), y(Y), xP(xp), yP(yp) {}
	double calculateF(int G) {
		F = sqrt((x - xEnd)*(x - xEnd) + (y - yEnd)*(y - yEnd)) + G;
		return F;
	}
};

void algorithm(int tda[20][20], int xCurrent, int yCurrent);
int calculateG(vector<Node> parents, int xCurrent, int yCurrent);
void createPath(int grid[20][20], vector<Node> parents, int x, int y);

int main(void) {
	ifstream file{ "grid.txt" };
	if (!file.is_open()) return -1;
	int grid[20][20] = { 0 }, xstart = 0, ystart = 0;
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			file >> grid[i][j];
	
	algorithm(grid, xstart, ystart);
	grid[0][0] = 3;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int Colors[6] = { 8,0,0,6,0,4 };

	if(warningstate != 1)
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			SetConsoleTextAttribute(hConsole, Colors[grid[i][j]]);
			cout << grid[i][j];
		}
		cout << endl;
	}
}

void algorithm(int tda[20][20], int xCurrent, int yCurrent) {
	vector<Node> open;
	vector<Node> closed;
	Node start(0, 0, 0 ,0);
	closed.push_back(start);
	open.push_back(start);
	double smallest;
	int posInVector = 0;


	while (xCurrent != xEnd || yCurrent != yEnd) {
		smallest = 1000;

		auto itRightC = find_if(closed.begin(), closed.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent + 1 && node.y == yCurrent; });
		if (xCurrent + 1 < 20 && tda[yCurrent][xCurrent + 1] != 5 && !(itRightC != closed.end())) {
			auto itRightO = find_if(open.begin(), open.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent + 1 && node.y == yCurrent; });
			Node nodeRight(xCurrent + 1, yCurrent, xCurrent, yCurrent);
			if (itRightO != open.end()) {
				nodeRight.F = nodeRight.calculateF(calculateG(closed, xCurrent, yCurrent));
				auto index = distance(open.begin(), itRightO);
				(nodeRight.F < open[index].F) ? open.erase(open.begin() + index), open.push_back(nodeRight) : "";
			}
			else {
				nodeRight.F = nodeRight.calculateF(calculateG(closed, xCurrent, yCurrent));
				open.push_back(nodeRight);
			}
			G = 0;
		}
		auto itDownC = find_if(closed.begin(), closed.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent && node.y == yCurrent + 1; });
		if (yCurrent + 1 < 20 && tda[yCurrent + 1][xCurrent] != 5 && !(itDownC != closed.end())) {
			auto itDownO = find_if(open.begin(), open.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent && node.y == yCurrent + 1; });
			Node nodeDown(xCurrent, yCurrent + 1, xCurrent, yCurrent);
			if (itDownO != open.end()) {
				nodeDown.F = nodeDown.calculateF(calculateG(closed, xCurrent, yCurrent));
				auto index = distance(open.begin(), itDownO);
				(nodeDown.F < open[index].F) ? open.erase(open.begin() + index), open.push_back(nodeDown) : "";
			}
			else {
				nodeDown.F = nodeDown.calculateF(calculateG(closed, xCurrent, yCurrent));
				open.push_back(nodeDown);
			}
			G = 0;
		}
		auto itLeftC = find_if(closed.begin(), closed.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent - 1 && node.y == yCurrent; });
		if (xCurrent - 1 >= 0 && tda[yCurrent][xCurrent - 1] != 5 && !(itLeftC != closed.end())) {
			auto itLeftO = find_if(open.begin(), open.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent - 1 && node.y == yCurrent; });
			Node nodeLeft(xCurrent - 1, yCurrent, xCurrent, yCurrent);
			if (itLeftO != open.end()) {
				nodeLeft.F = nodeLeft.calculateF(calculateG(closed, xCurrent, yCurrent));
				auto index = distance(open.begin(), itLeftO);
				(nodeLeft.F < open[index].F) ? open.erase(open.begin() + index), open.push_back(nodeLeft) : "";
			}
			else {
				nodeLeft.F = nodeLeft.calculateF(calculateG(closed, xCurrent, yCurrent));
				open.push_back(nodeLeft);
			}
			G = 0;
		}
		auto itUpC = find_if(closed.begin(), closed.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent && node.y == yCurrent - 1; });
		if (yCurrent - 1 >= 0 && tda[yCurrent - 1][xCurrent] != 5 && !(itUpC != closed.end())) {
			auto itUpO = find_if(open.begin(), open.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent && node.y == yCurrent - 1; });
			Node nodeUp(xCurrent, yCurrent - 1, xCurrent, yCurrent);
			if (itUpO != open.end()) {
				nodeUp.F = nodeUp.calculateF(calculateG(closed, xCurrent, yCurrent));
				auto index = distance(open.begin(), itUpO);
				(nodeUp.F < open[index].F) ? open.erase(open.begin() + index), open.push_back(nodeUp) : "";
			}
			else {
				nodeUp.F = nodeUp.calculateF(calculateG(closed, xCurrent, yCurrent));
				open.push_back(nodeUp);
			}
			G = 0;
		}
		for (int j = 0; j < open.size(); j++)
			(smallest >= open[j].F) ? smallest = open[j].F, posInVector = j : 0;

		if(open.empty() && (xCurrent != xEnd || yCurrent != yEnd)) {
			warningstate = 1;
			cout << "There is no path available";
			break;
		}

		closed.push_back(open[posInVector]);
		xCurrent = open[posInVector].x;
		yCurrent = open[posInVector].y;
		open.erase(open.begin()+posInVector);
		
		(xCurrent == 19, yCurrent == 19) ? createPath(tda, closed, xEnd, yEnd) : "";
		
	}
}

int calculateG(vector<Node> parents, int xCurrent, int yCurrent) {	
	auto itParent = find_if(parents.begin(), parents.end(), [&xCurrent, &yCurrent](const Node& node) {return node.x == xCurrent && node.y == yCurrent; });
	if (itParent != parents.end()) {
		auto index = distance(parents.begin(), itParent);
		if (xCurrent != 0 || yCurrent != 0) {
			G++;
			return calculateG(parents, parents[index].xP, parents[index].yP);
		}
		else
			G++;
	}
	return G;
}

void createPath(int grid[20][20], vector<Node> parents, int x, int y) {
	auto itParent = find_if(parents.begin(), parents.end(), [&x, &y](const Node& node) {return node.x == x && node.y == y; });
	if (itParent != parents.end()) {
		auto index = distance(parents.begin(), itParent);
		if (x != 0 || y != 0) {
			grid[y][x] = 3;
			return createPath(grid, parents, parents[index].xP, parents[index].yP);
		}
	}
}