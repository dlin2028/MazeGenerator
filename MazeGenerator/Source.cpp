#pragma once
#include <iostream>
#include <random>
#include <string>
#include <ctime>
#include <algorithm>
#include "UnionFind.h"
#include "Maze.h"
using namespace std;

int main()
{
	int width;
	int height;

	srand(time(nullptr));

	cout << "width";
	cin >> width;
	cout << "height";
	cin >> height;

	unique_ptr<Maze> maze = make_unique<Maze>(width, height);
	maze->Generate();
	maze->Display();
}