#pragma once

#include "Vector.h"
#include "RageBot.h"

class Utils {
public:

	static bool Clamp(Vector & angles);
	static void Normalize(Vector& angle);
	static void ClampAngles(Vector & angles);
	static void Normalize(float& angle);
};

