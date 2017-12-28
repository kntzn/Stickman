#pragma once
#include <SFML/System.hpp>

float vecL (sf::Vector2f vec)
	{
	return sqrt (vec.x*vec.x + vec.y*vec.y);
	}