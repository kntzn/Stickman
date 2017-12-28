#pragma once
#include <SFML/Graphics.hpp>

class WindowParameters 
	{
	public:
		sf::VideoMode vm;
		sf::ContextSettings cs;
		int Mode;
		unsigned int fpsLim;

		WindowParameters (sf::VideoMode VM, sf::ContextSettings CS, unsigned int FPSLim = 60, int MODE = sf::Style::Default)
			{
			vm = VM;
			cs = CS;
			Mode = MODE;
			fpsLim = FPSLim;
			}

		WindowParameters ()
			{
			}

	};

WindowParameters LoadWindowParameters ()
	{
	WindowParameters param;
	FILE* f;
	fopen_s (&f, "Settings/setup.txt", "r");

	if (fscanf_s (f, "%d ", &param.cs.antialiasingLevel) != EOF)
		if (fscanf_s (f, "%d ", &param.vm.width) != EOF)
			if (fscanf_s (f, "%d ", &param.vm.height) != EOF)
				if (fscanf_s (f, "%d ", &param.Mode) != EOF)
					if (fscanf_s (f, "%d ", &param.fpsLim) != EOF)
						{
						fclose (f);
						printf ("kukusiky5\n");
						return param;
						}
					else
						{
						fclose (f);
						return param;
						}
				else
					{
					fclose (f);
					printf ("kukusiky\n");
					return param;
					}
			else
				{
				fclose (f);
				printf ("kukusiky\n");
				return param;
				}
		else
			{
			fclose (f);
			printf ("kukusiky\n");
			return param;
			}
	else
		{
		fclose (f);
		printf ("kukusiky\n");
		return param;
		}

	fclose (f);
	printf ("kukusiky\n");
	return param;
	}

sf::Image LoadImage (sf::String path, bool Mask = true, sf::Color color = sf::Color (0, 0, 255))
	{
	sf::Image image;
	image.loadFromFile ("Data/img/" + path);
	if (Mask) image.createMaskFromColor (color);

	return image;
	}
