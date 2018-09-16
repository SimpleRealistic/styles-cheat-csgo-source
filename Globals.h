#pragma once

#include "PlayerList.h"

class KillSpammerGlobals
{
public:
	std::vector<int> killedPlayerQueue;
};

class Globals
{
public:
	Vector qLastTickAngle;
	CUserCmd* UserCmd;
	IClientEntity* Target;
	int Shots;
	Color forecolor = Color(255, 165, 0, 255);
	float fov;
	KillSpammerGlobals killspammer;

	float OldSimulationTime[65];
};

extern Globals globals;