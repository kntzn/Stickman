#pragma once
#include <SFML/Graphics.hpp>

#include "StickLib.h"

//All types of actions
namespace  Action
	{
	enum
		{
		Stay,
		Walk,
		Run,
		Sprint,
		Fly,
		Jump
		};
	}

// Container for all appearance parameters & timers;
class Appearance
	{
	public:
		unsigned short int hairStyle;
		unsigned short int hairColor;

		float eyesState; //timer
		float eyesStyle;
		unsigned short int eyesColor;

		unsigned short int mouthStyle;

		unsigned short int clothes;

		unsigned short int skinTone;

		float legsTimer;  //timer
		

		Appearance ()
			{
			eyesColor = 0;
			eyesStyle = 1.0f;
			eyesState = 0.5f;

			hairStyle = 0;
			hairColor = 
			mouthStyle =
			clothes = 
			skinTone = 0;

			legsTimer = 0;
			}
	};

Appearance basic;

namespace Draw
	{
	void Stickman (sf::RenderWindow &window, sf::Texture &texture, sf::Vector2f pos, bool drawHands = true,
				   sf::Vector2f velocity = sf::Vector2f (0, 0), float time = 0, bool way = 0, 
				   float angle = 0, unsigned int action = Action::Stay, Appearance &app = basic)
		{
		bool legs_way = 0;
		if (velocity.x < 0) legs_way = 1;

		//body
		sf::ConvexShape body;
		body.setPosition (pos - sf::Vector2f (0, 130));

		body.setOutlineThickness (1);
		body.setOutlineColor (sf::Color::Black);

		body.setPointCount (4);
		body.setPoint (0, sf::Vector2f (-30, -50));
		body.setPoint (1, sf::Vector2f ( 30, -50));
		body.setPoint (2, sf::Vector2f ( 30,  50));
		body.setPoint (3, sf::Vector2f (-30,  50));
		
		body.setTexture (&texture);
		body.setTextureRect (sf::IntRect (30*(app.clothes + way), 45, 30*(1 - 2*int (way)), 50));
		window.draw (body);
		
		//legs
		sf::Sprite legs;
		legs.scale (2.0f, 2.0f);

		app.legsTimer += time*fabs (velocity.x);
		if (app.legsTimer > 7) app.legsTimer = 0;
		legs.setOrigin (45, 40);
		legs.setPosition (pos);

		legs.setTexture (texture);

		//hands
		sf::Sprite hands;
		if (drawHands)
			{
			hands.scale (2.0f, 2.0f);

			hands.setOrigin (40+10.0f*way, 10);
			hands.setPosition (pos - sf::Vector2f (float (10 - way*20), 160));

			hands.setTexture (texture);
			}

		switch (action)
			{
			case Action::Stay:
				{
				if (fabs (velocity.x) > 3.f)
					legs.setTextureRect  (sf::IntRect (90*(legs_way),      165, 90*(1 - 2*int (legs_way)),  40));
				else
					legs.setTextureRect  (sf::IntRect (90*(legs_way), 125, 90*(1 - 2*int (legs_way)), 40));
					
				if (drawHands)
					hands.setTextureRect (sf::IntRect (90*(1 + way), 205, 90*(1 - 2*int (way)), 20));

				break;
				}
			case Action::Walk:
				{
				if (app.legsTimer > 5) app.legsTimer = 0;
				legs.setTextureRect (sf::IntRect (90*int (1 + app.legsTimer + legs_way), 165, 90*(1 - 2*int (legs_way)), 40));
				if (drawHands)
					hands.setTextureRect (sf::IntRect (90*int (1 + way), 205, 90*(1 - 2*int (way)), 20));

				break;
				}
			case Action::Run:
				{
				legs.setTextureRect (sf::IntRect (90*int (1 + app.legsTimer + legs_way), 125, 90*(1 - 2*int (legs_way)), 40));
				if (drawHands)
					hands.setTextureRect (sf::IntRect (90*int (1 + way), 205, 90*(1 - 2*int (way)), 20));

				break;
				}
			case Action::Sprint:
				{
				legs.setTextureRect (sf::IntRect (90*int (1 + app.legsTimer + legs_way), 125, 90*(1 - 2*int (legs_way)), 40));
				if (drawHands)
					hands.setTextureRect (sf::IntRect (90*int (1 + app.legsTimer + way), 205, 90*(1 - 2*int (way)), 20));

				break;
				}
			case Action::Jump:
				{
				legs.setTextureRect (sf::IntRect (90*(legs_way+3), 165, 90*(1 - 2*int (legs_way)), 40));
				if (drawHands)
					hands.setTextureRect (sf::IntRect (90*(way+1), 205, 90*(1 - 2*int (way)), 20));

				break;
				}
			case Action::Fly:
				{
				if (fabs (velocity.x) > 3.f)
					legs.setTextureRect (sf::IntRect (90*(legs_way), 165, 90*(1 - 2*int (legs_way)), 40));
				else
					legs.setTextureRect (sf::IntRect (90*(legs_way+4), 165, 90*(1 - 2*int (legs_way)), 40));

				if (drawHands)
					hands.setTextureRect (sf::IntRect (90*(way+1), 205, 90*(1 - 2*int (way)), 20));

				break;
				}


			default:
				break;
			}
		
		if (drawHands)
			window.draw (hands);

		window.draw (legs);

		//head
		sf::CircleShape head;
		head.setPosition (pos - sf::Vector2f (0, 225));

		head.scale (2.0f, 2.0f);
		head.setRotation (45 + angle);
		head.setPointCount (4);
		head.setRadius (14.5f*sqrt2);
		head.setOutlineThickness (1);
		head.setOutlineColor (sf::Color::Black);

		head.setOrigin (15, 15);

		head.setFillColor (sf::Color (232, 227, 219));
		window.draw (head);

		//hair
		sf::Sprite hair;
		hair.setPosition (pos - sf::Vector2f (0, 210));
		hair.scale (2.0f, 2.0f);

		hair.setOrigin (30.5f, 30.5f);

		hair.setTexture (texture);
		hair.setTextureRect (sf::IntRect (60*(app.hairStyle + way), 0, 60*(1 - 2*int (way)), 44));
		window.draw (hair);

		//eyes
		sf::Sprite eyes;
		eyes.scale (1, app.eyesStyle);
		eyes.setPosition (pos - sf::Vector2f (0, 210));
		eyes.setOrigin (20 + 20.0f*way, 25);

		eyes.setTexture (texture);

		app.eyesState += 0.3f*time;
		if (app.eyesState > 1) app.eyesState = 0;

		if (app.eyesState > 0.05f) eyes.setTextureRect (sf::IntRect (60*(app.eyesColor + 1 + way), 95, 60*(1 - 2*int (way)), 30));
		else                       eyes.setTextureRect (sf::IntRect (60*way, 95, 60*(1 - 2*int (way)), 30));

		window.draw (eyes);


		}

	void SimpleTxtr (sf::RenderWindow &window, sf::Texture &texture, sf::IntRect rect, sf::Vector2f pos, float angle = 0)
		{
		sf::Sprite s;
		s.setTexture (texture);
		//s.setTextureRect (rect);
		s.setOrigin (sf::Vector2f (rect.width/2.f, rect.height/2.f));
		s.setPosition (pos);
		s.setRotation (angle);
		s.setTextureRect (rect);
		window.draw (s);
		}
	}