#pragma once
#include <memory>
#include <vector>
#include "UnionFind.h"
#include <SFML/Graphics.hpp>
using namespace std;

class Maze
{
private:
	unique_ptr<UnionFind> find;
	int width;
	int height;
	vector<int> posMap;
	vector<int> starts;
	vector<int> ends;
	vector<int> getPath(int start, int end);
	vector<int> getEdges(int node);
	vector<sf::RectangleShape> generateRects();
	std::map<int, int> invPosMap;
	void remove(vector<int>& v);
public:
	Maze(int width, int height);
	~Maze();
	void Generate();
	void Display();
};