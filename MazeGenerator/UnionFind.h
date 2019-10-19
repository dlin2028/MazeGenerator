#pragma	once
#include <memory>
#include <vector>
#include "UnionFind.h"
class UnionFind
{
private:
	std::vector<int> id;
	int size;
public:
	UnionFind(int size);
	bool Connected(int a, int b);
	void Union(int a, int b);
	int Find(int i);
	int NumSets();
};