

#pragma once

#include "Hacks.h"
#include "XorStr.hpp"
#include "Hooks.h"

Vector GetAutostrafeView();

class ClanChanger
{
public:
	float g_ClanChangerTick;

	void SetTag(float frametime);
	bool TickCheck(int min, int max);

	void SuicideFlash();
	void SkeetTag();
	void BlackDeath();
	void BlackFlash();
	void XenoHook();
};


class NameChanger
{
public:
	bool flip = false;

	void SetName(const char* name);
	
	void Manage();
	void Suicide();
	void BlackDeath();
	void XenoHook();
	void AcidTrip();
};

class ChatSpammer
{
public:
	void Manage();
	void BlackDeath();
	void DeathJokes();
	void Racist();
};

class RadioSpammer
{
public:
	void Manage();
};

class KillSpammer
{
public:
	void Manage();
};

class CMiscHacks : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool& bSendPacket);
private:
	ClanChanger clanchanger;
	NameChanger namechanger;
	ChatSpammer chatspammer;
	RadioSpammer radiospammer;
	KillSpammer killspammer;

	//void BhopMovement(CUserCmd *pCmd);
	void BhopMovement(CUserCmd* pCmd, Vector& OrigAngle);
	void PostProcessing();
	void Fakelag(CUserCmd *pCmd, bool &bSendPacket);
};