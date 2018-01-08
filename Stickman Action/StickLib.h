#pragma once
#include <math.h>
//Pi number 
const float Pi = 3.141592653589793238462643383279f; 
const float toRad = Pi/180;
const float toDeg = 1/toRad;

float sqrt2 = float (sqrt(2.0f));

signed sign (float var)
	{
	if (var < 0) return -1;
	else         return  1;
	}

float rangeRand (float left, float right)
	{
	if (left >= right)
		return left;
	else
		return left + float (rand ()%(int ((right-left)*10000)))/10000.f;
	}

float getNumberLastDigits (float num, int n)
	{
	return num - int (num/pow (10, n))*pow (10, n);
	}