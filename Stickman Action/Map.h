#pragma once
#include <SFML/Graphics.hpp>
#include "FileIO.h"
#include "Mission.h" 

#define MAP_W 500
#define MAP_H 100

sf::Color mapColors [] =
	{
	sf::Color (10, 10, 10)
	};

int Blocks  [][5][5] = 
	{
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1,

	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 1,

	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 1, 0, 1,
	1, 1, 1, 1, 1,
	};

class Level
	{
	public:
		int TileMap     [MAP_H][MAP_W] = {};
		int PhysicalMap [MAP_H][MAP_W] = {};

		void FillBlock (sf::Vector2i pos, int block)
			{
			for (int x = 0; x < 5; x++)
				for (int y = 0; y < 5; y++)
					{
					TileMap [pos.y*5 + y] [pos.x*5 + x] = Blocks [block] [y] [x];
					}
			}

		void RefreshPhysicalMap ()
			{
			for (int x = 0; x < MAP_W; x++)
				for (int y = 0; y < MAP_H; y++)
					{
					if (TileMap[y][x] == 0) PhysicalMap [y][x] = 0;
					if (TileMap[y][x] == 1) PhysicalMap [y][x] = 1;
					}
			}

		void load (const char filename [])
			{		
			char* buf = nullptr;
			size_t bufSize = 0;
			loadFromFile (filename, buf, bufSize, true, true);

			size_t cursor = 0;
			for (int y = 0; y < MAP_H; y++)
				for (int x = 0; x < MAP_W && cursor < bufSize; x++)
					{
					FillBlock (sf::Vector2i (x, y), atoi (buf+cursor));
					cursor += std::to_string (atoi (buf+cursor)).size ()+1;
					}

			free (buf);

			RefreshPhysicalMap ();
			}
	
		Level (unsigned int LVL, int FRACTION)
			{
			std::string filename;
			filename += "Data/map/";
			filename += std::to_string (LVL);
			filename += ".ini";
			
			load (filename.c_str ());
			}

	void Draw (sf::RenderWindow &window, sf::Sprite map, sf::Vector2f center)
		{
		for (int x = int (center.x)/100 - 11; x < int (center.x)/100 + 12; x++)
			for (int y = int (center.y)/100 - 6; y < int (center.y)/100 + 6; y++)
				{
				if (0 <= x && x < MAP_W)
					if (0 <= y && y < MAP_H)
						if (TileMap [y][x] != 0) 
							{
							map.setTextureRect (sf::IntRect (TileMap [y][x]*100 - 100, 0, 100, 100)); 
							map.setPosition (x*100.f, y*100.f);
							window.draw (map); 
							}
				}
		}
	};