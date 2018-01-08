#pragma once
#include <SFML/Graphics.hpp>

class Camera
	{
	private:
		float zoom;
	public:
		sf::View cam;

		Camera (sf::FloatRect fr)
			{
			zoom = 1;

			cam.reset (fr);
			}

		sf::View PlayerCam (sf::Vector2f pos)
			{
			cam.setCenter (pos.x, pos.y);
			return cam;
			}

		sf::View update (sf::RenderWindow &window, sf::Vector2i MousePos, float deltaZoom, float time)
			{
			if (MousePos.x < 30)								   cam.move (-1000.f/zoom*time, 0);
			if (MousePos.x > signed int (window.getSize().x - 30)) cam.move (1000.f/zoom*time, 0);
			if (MousePos.y < 30)								   cam.move (0, -1000.f/zoom*time);
			if (MousePos.y > signed int (window.getSize().y - 30)) cam.move (0,  1000.f/zoom*time);

			cam.zoom (1-float (deltaZoom)/10);
			zoom /= 1-float (deltaZoom)/10;

			return cam;
			}

		float getZoom () 
			{
			return zoom;
			}
	};
