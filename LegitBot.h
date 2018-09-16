/*
Project X
*/

#pragma once

#include "Hacks.h"

class CLegitBot : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:
	// Targetting
	int GetTargetCrosshair();

	bool AimAtPoint(Vector point, CUserCmd *pCmd, IClientEntity* pLocal, bool &bSendPacket);

	void SyncWeaponSettings();

	// Functionality
	void DoAimbot(bool &bSendpacket, CUserCmd *pCmd);
	bool TargetMeetsRequirements(IClientEntity * pEntity);
	void DoTrigger(CUserCmd *pCmd);

	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity * pEntity, int aHitBox);

	// Aimbot
	bool IsLocked;
	int TargetID;
	int HitBox;
	bool Multihitbox;
	int besthitbox;
	Vector AimPoint;

	float Speed;
	float FoV;
	bool RecoilControl;
	float RecoilAmountX;
	float RecoilAmountY;
	bool PSilent;
	float Inacc;
};