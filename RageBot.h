
#pragma once

#include "Hacks.h"
#include "Settings.h"
#include "Autowall.h"
#include "Globals.h"

#define MAX_PLAYERS 64
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / I::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( I::Globals->interval_per_tick *( t ) )
#define LC_NONE				0
#define LC_ALIVE			(1<<0)
#define LC_ORIGIN_CHANGED	(1<<8)
#define LC_ANGLES_CHANGED	(1<<9)
#define LC_SIZE_CHANGED		(1<<10)
#define LC_ANIMATION_CHANGED (1<<11)
#define LAG_COMPENSATION_TELEPORTED_DISTANCE_SQR ( 64.0f * 64.0f )
#define LAG_COMPENSATION_EPS_SQR ( 0.1f * 0.1f )
#define LAG_COMPENSATION_ERROR_EPS_SQR ( 4.0f * 4.0f )

class AntiAim
{
public:
	Vector LastAntiAimAngles;
	int AntiAimType;
	float fakeAngles;
};

enum eTargeting
{
	SELECTION_FOV,
	SELECTION_HEALTH,
	SELECTION_DISTANCE,
	SELECTION_THREAT,
	SELECTION_FIRSTVALID
};

class Aimbot
{
public:

	int iMaxChokedticks = 14;
	int iChokedticks = -1;

	float AimNearPlayer(IClientEntity * pEnt);

	int TargetSelection(CUserCmd * pCmd);

	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
	int HitScan(IClientEntity * pEntity);
	void SyncWeaponHitchance();
	float                       WeaponHitchance;
	float hitchance(IClientEntity* pLocal);


	// Functionality
	void DoAimbot(CUserCmd *pCmd, bool &bSendPacket);

	// Aimbot
	int TargetID;

	void AtTargets(CUserCmd * pCmd);
	void RageAntiAim(CUserCmd * pCmd, bool & bSendPacket);
	void RealYaw(CUserCmd * pCmd, bool & bSendPacket);
	void FakeYaw(CUserCmd * pCmd, bool & bSendPacket);
};

class CRageBot : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool& bSendPacket);
private:
	AntiAim aa;
	Aimbot aimbot;

	void RemoveRecoil(CUserCmd* pCmd);
};