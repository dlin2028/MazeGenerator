#pragma once
#include <iostream>
#include <random>
#include <string>
#include <ctime>
#include <algorithm>
#include "..\..\UnionFind\UnionFind\UnionFind.h"
#include <SFML/Graphics.hpp>
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

	unique_ptr<UnionFind> find = make_unique<UnionFind>(width * height);

	vector<int> starts;
	vector<int> ends;

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

	vector<int> xPos;

	vector<char> output;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < (width * 2) + 1; j++)
		{
			output.push_back('X');
		}
		for (int j = 0; j < width; j++)
		{
			output.push_back('X');
			xPos.push_back(output.size());
			output.push_back(' ');
		}
		output.push_back('X');
	}
	for (int i = 0; i < (width * 2) + 1; i++)
	{
		output.push_back('X');
	}

	int outWidth = width * 2 + 1;
	for (int i = 0; i < starts.size(); i++)
	{
		int pos = xPos[starts[i]];
		switch (ends[i])
		{
		case 0: //up
			output[pos - outWidth] = ' ';
			break;
		case 1: //down
			output[pos + outWidth] = ' ';
			break;
		case 2: //left
			output[pos - 1] = ' ';
			break;
		default: //right
			output[pos + 1] = ' ';
			break;
		}
	}

	vector<sf::RectangleShape> rects;
	int currHeight = -1;
	for (int i = 0; i < output.size(); i++)
	{
		if (i % (width * 2 + 1) == 0)
		{
			currHeight++;
			cout << endl;
		}
		sf::RectangleShape rect(sf::Vector2f(10, 10));
		rect.setPosition((i % outWidth) * 10, currHeight * 10);
		rect.setFillColor(output[i] == 'X' ? sf::Color::Green : sf::Color::White);
		rects.push_back(rect);
		cout << output[i];
	}

	sf::RenderWindow window(sf::VideoMode(outWidth * 10, (height * 2 + 1) * 10), "wowoow");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (int i = 0; i < rects.size(); i++)
		{
			window.draw(rects[i]);
		}
		window.display();
	}
}