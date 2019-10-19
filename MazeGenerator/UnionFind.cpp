#include "UnionFind.h"

UnionFind::UnionFind(int size)
{
	this->size = size;
	for (int i = 0; i < size; i++)
	{
		id.push_back(i);
	}
}

bool UnionFind::Connected(int a, int b)
{
	return id[a] == id[b];
}

void UnionFind::Union(int a, int b)
{
	if (!Connected(a, b))
	{
		int aId = Find(a);
		int bId = Find(b);

		for (int i = 0; i < size; i++)
		{
			if (id[i] == bId)
			{
				id[i] = aId;
			}
		}
	}
}

int UnionFind::Find(int i)
{
	return id[i];
}

int UnionFind::NumSets()
{
	std::vector<int> sets;
	for (int i = 0; i < id.size(); i++)
	{
		bool contains = false;
		for (int j = 0; j < sets.size(); j++)
		{
			if (sets[j] == id[i])
			{
				contains = true;
				continue;
			}
		}
		if (!contains)
		{
			sets.push_back(id[i]);
		}
	}
	return sets.size();
}
