#include "Maze.h"
#include <queue>
#include <algorithm>
#include <map>

using namespace std;

Maze::Maze(int w, int h)
{
	width = w;
	height = h;
}

Maze::~Maze()
{
	find.release();
}


void Maze::Generate()
{
	srand(5);
	find = std::make_unique<UnionFind>(width * height);

	while (find->NumSets() != 1)
	{
		int i = (rand() % (width * height));

		switch (rand() % 4)
		{
		case 0: //up
			if (i > width - 1)
			{
				if (!find->Connected(i, i - width))
				{
					starts.push_back(i);
					ends.push_back(0);

					find->Union(i, i - width);
				}
			}
			break;
		case 1: //down
			if (i < (height - 1) * width)
			{
				if (!find->Connected(i, i + width))
				{
					starts.push_back(i);
					ends.push_back(1);

					find->Union(i, i + width);
				}
			}
			break;
		case 2: //left
			if (i % width != 0)
			{
				if (!find->Connected(i, i - 1))
				{
					starts.push_back(i);
					ends.push_back(2);

					find->Union(i, i - 1);
				}
			}
			break;
		default: //right
			if ((i + 1) % width != 0)
			{
				if (!find->Connected(i, i + 1))
				{
					starts.push_back(i);
					ends.push_back(3);

					find->Union(i, i + 1);
				}
			}
			break;
		}
	}
}

vector<sf::RectangleShape> Maze::generateRects()
{
	vector<bool> output;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < (width * 2) + 1; j++)
		{
			output.push_back(true);
		}
		for (int j = 0; j < width; j++)
		{
			output.push_back(true);
			posMap.push_back(output.size());
			output.push_back(false);
		}
		output.push_back(true);
	}
	for (int i = 0; i < (width * 2) + 1; i++)
	{
		output.push_back(true);
	}

	int outWidth = width * 2 + 1;
	for (int i = 0; i < starts.size(); i++)
	{
		int pos = posMap[starts[i]];
		switch (ends[i])
		{
		case 0: //up
			output[pos - outWidth] = false;
			break;
		case 1: //down
			output[pos + outWidth] = false;
			break;
		case 2: //left
			output[pos - 1] = false;
			break;
		default: //right
			output[pos + 1] = false;
			break;
		}
	}

	vector<sf::RectangleShape> rects;
	int currHeight = -1;
	for (int i = 0; i < output.size(); i++)
	{
		if (i % outWidth == 0)
		{
			currHeight++;
		}

		sf::RectangleShape rect(sf::Vector2f(10, 10));
		rect.setPosition((i % outWidth) * 10, currHeight * 10);
		rect.setFillColor(output[i] ? sf::Color::Green : sf::Color::White);
		rects.push_back(rect);
	}

	return rects;
}
void Maze::remove(std::vector<int>& v)
{
	auto end = v.end();
	for (auto it = v.begin(); it != end; ++it) {
		end = std::remove(it + 1, end, *it);
	}

	v.erase(end, v.end());
}
vector<int> Maze::getEdges(int node)
{
	vector<int> output;
	for (int i = 0; i < starts.size(); i++)
	{
		switch (ends[i])
		{
		case 0:
			if(starts[i] == node)
				output.push_back(node - width);
			else if(starts[i] == node + width)
				output.push_back(node + width);
			break;
		case 1:
			if (starts[i] == node)
				output.push_back(node + width);
			else if (starts[i] == node - width)
				output.push_back(node - width);
			break;
		case 2:
			if (starts[i] == node || starts[i] == node + 1)
				output.push_back(node - 1);
			else if (starts[i] == node + 1)
				output.push_back(node + 1);
			break;
		default:
			if (starts[i] == node)
				output.push_back(node + 1);
			else if (starts[i] == node - 1)
				output.push_back(node - 1);
			break;
		}
	}
	remove(output);
	return output;
}

vector<int> Maze::getPath(int start, int end)
{
	queue<int> queue;
	vector<int> discovered;
	discovered.clear();
	std::map<int, int> parents;

	queue.push(start);
	discovered.push_back(start);

	int curr;

	while (queue.size() > 0)
	{
		curr = queue.front();
		queue.pop();

		if (curr == end)
		{
			vector<int> path; 
			while (curr != start)
			{
				path.push_back(curr);
				curr = parents[curr];
			}
			path.push_back(curr);
			return path;
		}
		vector<int> edges = getEdges(curr);
		for (int i = 0; i < edges.size(); i++)
		{
			if (std::find(discovered.begin(), discovered.end(), edges[i]) == discovered.end())
			{
				discovered.push_back(edges[i]);
				parents[edges[i]] = curr;
				queue.push(edges[i]);
			}
		}
	}
	throw std::invalid_argument("no path found");
}

void Maze::Display()
{
	vector<sf::RectangleShape> rects = generateRects();

	const int outWidth = (width * 2 + 1);

	sf::RenderWindow window(sf::VideoMode(outWidth * 10, (height * 2 + 1) * 10), "wowoow11");

	sf::RectangleShape* start = nullptr;
	sf::RectangleShape* dest = nullptr;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			sf::Vector2i position = sf::Mouse::getPosition(window);
			sf::RectangleShape* rect = &rects[(position.x / 10) % outWidth + (position.y / 10) * outWidth];

			if (rect->getFillColor() != sf::Color::Green)
			{
				if (start != nullptr && dest != nullptr)
				{
					start = nullptr;
					dest = nullptr;
					generateRects();
				}

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (start != nullptr)
					{
						start->setFillColor(sf::Color::White);
					}
					start = rect;
					start->setFillColor(sf::Color::Red);
				}
				else
				{
					if (dest != nullptr)
					{
						dest->setFillColor(sf::Color::White);
					}
					dest = rect;
					dest->setFillColor(sf::Color::Blue);
				}

				if (start != nullptr && dest != nullptr)
				{
					sf::Vector2f pos = start->getPosition();
					int s = (int)(pos.x / 10) % outWidth + (int)(pos.y / 10) * outWidth;
					pos = dest->getPosition();
					int d = (int)(pos.x / 10) % outWidth + (int)(pos.y / 10) * outWidth;

					for (int i = 0; i < posMap.size(); i++)
					{
						invPosMap[posMap[i]] = i;
					}
					vector<int> path = getPath(invPosMap[s], invPosMap[d]);
					

					for (int i = 0; i < path.size(); i++)
					{
						int pos = posMap[path[i]];
						rects[pos].setFillColor(sf::Color::Black);
						if (i != 0)
						{
							int diff =  path[i - 1] - path[i];
							if (diff == 1)
							{
								rects[pos + 1].setFillColor(sf::Color::Black);
							}
							else if (diff == -1)
							{
								rects[pos - 1].setFillColor(sf::Color::Black);
							}
							else if (diff == width)
							{
								rects[pos + outWidth].setFillColor(sf::Color::Black);
							}
							else //if (diff == -width)
							{
								rects[pos - outWidth].setFillColor(sf::Color::Black);
							}
						}
					}
				}
			}
		}

		window.clear();
		for (int i = 0; i < rects.size(); i++)
		{
			window.draw(rects[i]);
		}
		window.display();
	}
}

