#pragma once
#include <SFML\System.hpp>
#include "Object.h"
#include "Map"
#include "Sticklib.h"
#include "sfVector.h"

// Enums
enum gunType
	{
	Knife,
	Pistol,
	Rifle,
	Shotgun,
	Machinegun,
	sRifle
	};
enum damageType
	{
	Slash, 
	Kinetic,
	Electric,
	Plasma
	};
enum triggerType
	{
	SemiAuto,
	Auto,
	Hold
	};
enum gunSlot
	{
	Primary,
	Secondary,
	Melee
	};

class Bullet: public Object
	{
	private:
		sf::IntRect txtr;
		float distance = 0, maxDist = 0;
	public:
		Bullet (sf::Image &img, sf::Vector2f POS, sf::Vector2f VEL, float ANG, sf::IntRect TXTR, float maxD, float M): Object (img, POS, M)
			{
			angle = ANG;
			velocity = VEL;
			txtr = TXTR;
			maxDist = maxD;
			}

		void Control (sf::Vector2f target, float time)
			{
			if (distance > maxDist)
				{
				life = false;
				}
			distance += vecL (velocity)*time;
			}

		void CheckBorders (Level &level, float time)
			{
			if (level.PhysicalMap [int (position.y/100)] [int (position.x/100)])
				{
				life = false;
				}
			}

		void Draw (sf::RenderWindow &window, float time)
			{
			Draw::SimpleTxtr (window, texture, txtr, position, -angle*toDeg+90);
			//Test with big red point
			/*sf::CircleShape cs;
			cs.setPosition (position);
			cs.setRadius (15);
			cs.setFillColor (sf::Color::Red);
			window.draw (cs);*/
			}
	};

class Gun
	{
	private:
		// Damage of Bullet Shot, Range of shot, speed of bullet, mass of gun, length of gun, dispersion of shot
		float damage = 0, range = 0, bulletSpeed = 0, mass = 0, length = 0, dispersion = 0, reloadTime = 0, shotDelay = 0;
		// Type of trigger, Type of damage, Type of gun, Nuber of Bullets per Shot
		int trgType = 0, dmgType = 0, gType = 0, nBpS = 0, magazine = 0;
		// Charging animation (for Plasma guns)
		bool chargeAnimation = false;

		// Bullet texture rect
		sf::IntRect bulTxtrRect;

		// TIMERS
		float reloadTimer = 0, shootTimer = 0, bulletsLeftInMagazine = magazine;
		bool readyToFire = false;


		// Gun id
		int id = 0;
	public:
		// Default constructor
		Gun ()
			{
			}

		// Constructor for guns' copies
		Gun (int ID, int GTYPE, int TRIGGER,
			 float DMG, int DMGTYPE, float bSPEED, int nBPS, int MAG,
			 float RANGE, float DISP,
			 float MASS, float LEN,
			 float relT, float sDelay,
			 sf::IntRect BULLETRECT,
			 bool chrgAnim = false)
			{
			id = ID;
			gType = GTYPE;
			trgType = TRIGGER;

			damage = DMG;
			dmgType = DMGTYPE;
			bulletSpeed = bSPEED;
			nBpS = nBPS;
			magazine = MAG;

			range = RANGE;
			dispersion = DISP;

			mass = MASS;
			length = LEN;

			reloadTime = relT;
			shotDelay = sDelay;

			bulTxtrRect = BULLETRECT;

			chargeAnimation = chrgAnim;
			}

		void update (float time)
			{
			std::cout << "Bul Left: " << bulletsLeftInMagazine << " ShotDelay: " << shootTimer << " reloadTimer: " << reloadTimer << std::endl;

			if (bulletsLeftInMagazine == 0)
				{
				reloadTimer = reloadTime;
				bulletsLeftInMagazine = magazine;
				}

			if (shootTimer > 0)
				shootTimer -= time;
			else
				shootTimer = 0;

			if (reloadTimer > 0)
				reloadTimer -= time;
			else
				reloadTimer = 0;

			if (reloadTimer == 0 && shootTimer == 0)
				readyToFire = true;
			else
				readyToFire = false;
			}

		void fire ()
			{
			readyToFire = false;
			shootTimer = shotDelay;
			bulletsLeftInMagazine--;
			}

		//---------------GETTERS---------------//
		// Returns nuber of bullets per shot
		int nBulletsPerShot ()
			{
			return nBpS;
			}
		// Returns gun's length from right hand
		float getLength ()
			{
			return length;
			}
		// Returns gun's id
		int getID ()
			{
			return id;
			}
		// Returns true if gun is ready
		bool isReady ()
			{
			return readyToFire;
			}
		// Returns true if charge animation is available
		bool animationAvailable ()
			{
			return chargeAnimation;
			}
		// Returns percentage of charge
		float rechargePercentage ()
			{
			return (reloadTime-reloadTimer)/reloadTime;
			}

		Bullet createBullet (sf::Image &img, sf::Vector2f position, float angle, float currentDisp, sf::Vector2f additionalVel = sf::Vector2f (0, 0))
			{
			float disp = rangeRand (-currentDisp-dispersion, currentDisp+dispersion);
			disp = 0;
			return Bullet (img,
						   position,
						   sf::Vector2f (bulletSpeed*sin (-angle-disp), bulletSpeed*cos (-angle-disp)),
						   -(angle+disp),
						   bulTxtrRect,
						   range,
						   1.0f);
			}
	};

void CreateBulletsFromGun (std::vector <Bullet*> &objects, sf::Image &img, sf::Vector2f position, float angle, float currentDisp, Gun gun, sf::Vector2f additionalVel)
	{
	for (int i = 0; i < gun.nBulletsPerShot(); i++)
		objects.push_back (new Bullet (gun.createBullet (img, position, angle, currentDisp, additionalVel)));
	}
//
//
//
// WHAT IS NEXT? 
// reload animation
//
//

//TODO: All Guns here:

Gun hands  (0, gunType::Knife,  triggerType::SemiAuto, 10,  damageType::Kinetic, 40.f, 1, 1,   150.f, 0,        0,   120.f, 0.25f,   0, sf::IntRect (280, 0, 25, 60), false);
Gun PSR400 (1, gunType::sRifle, triggerType::Hold,     200, damageType::Plasma,  80.f, 1, 1,  5000.f, Pi/32.f, 10,   210.f,  3.f,    0, sf::IntRect (540, 88, 36, 5),  true);
Gun F12    (2, gunType::Pistol, triggerType::SemiAuto, 35,  damageType::Kinetic, 30.f, 1, 12, 2000.f, Pi/16.f, 1.5f, 142.f,  3.f, 0.5f, sf::IntRect (275, 155, 5, 3), false);