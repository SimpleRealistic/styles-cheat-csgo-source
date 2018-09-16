
#include "RageBot.h"
#include "RenderManager.h"
#include "Autowall.h"
#include <iostream>
#include "Globals.h"
#include "Resolver.h"
#include "Utils.h"


#define M_PI		3.14159265358979323846f
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#define TICK_INTERVAL			( I::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )


void CRageBot::Init()
{
	aimbot.TargetID = -1;
}

void CRageBot::Draw() {}

float Aimbot::hitchance(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	float hitchance = 101.f;

	if (!pWeapon) return 0;

	if (WeaponHitchance > 1)
	{
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;
	}
	return hitchance;

}
void CRageBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	if (I::Engine->IsConnected() && I::Engine->IsInGame())
	{
		if (!hackManager.pLocal() || !hackManager.pLocal()->IsAlive()) return;

		CBaseCombatWeapon* BaseWeapon = hackManager.pLocal()->GetWeapon();
		if (BaseWeapon)
		{
			if (BaseWeapon->GetAmmoInClip())
			{
				if (ragebotconfig.bEnabled)
				{
					if (hackManager.pLocal()->IsAlive())
					{
						if (GameUtils::CanShoot())
						{
							aimbot.DoAimbot(pCmd, bSendPacket);				
						}
					}
				}
				RemoveRecoil(pCmd);
			}
			else
			{
				pCmd->buttons &= IN_ATTACK;
				pCmd->buttons |= IN_RELOAD;
			}

			if (Menu::Window.RageBotTab.Enabled.GetState())
			{
				static int ChokedPackets = -1;

				CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

				if (!pWeapon)
					return;

				if (ChokedPackets < 1 && hackManager.pLocal()->IsAlive() && pCmd->buttons & IN_ATTACK && GameUtils::CanShoot() && GameUtils::IsBallisticWeapon(pWeapon))
				{
					bSendPacket = false;
				}
				else
				{
					if (hackManager.pLocal()->IsAlive())
					{
						aimbot.RageAntiAim(pCmd, bSendPacket);
					}
					ChokedPackets = -1;
				}
			}
		}
	}
}

void CRageBot::RemoveRecoil(CUserCmd* pCmd)
{
	if (!ragebotconfig.bRemoveRecoil) return;

	if (pCmd->buttons & IN_ATTACK)
	{
		if (hackManager.pLocal())
		{
			auto qPunchAngles = hackManager.pLocal()->localPlayerExclusive()->GetAimPunchAngle();
			auto qAimAngles = pCmd->viewangles;
			qAimAngles -= qPunchAngles * 2.f;
			GameUtils::NormalizeVector(qAimAngles);
			pCmd->viewangles = qAimAngles;
		}
	}
}

float Aimbot::AimNearPlayer(IClientEntity* pEnt)
{
	auto src = pEnt->GetEyePosition();
	auto dst = hackManager.pLocal()->GetEyePosition();
	auto forward = src - dst;
	Vector Product;
	VectorAngles(forward, Product);
	GameUtils::NormalizeVector(Product);
	static auto flMaxDif = 180.f;
	auto fldif = Product.y;
	auto flPercent = 1.f - abs(fldif / flMaxDif);
	return flPercent;
}

int Aimbot::TargetSelection(CUserCmd* pCmd) {

	// Main definitions
	float flFOV = 360.f;
	float flDistance = 8192.f;
	float flThreatLevel = 0.f;
	float flHealth = 101.f;
	float flDamage = 0.f;
	int iBestTarget = -1;


	for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pClient = I::EntList->GetClientEntity(i);

		if (!pClient->IsValidPlayer())
			continue;

		Vector vecHitbox = pClient->GetBonePos(8);

		if (ragebotconfig.iTargetSelection == SELECTION_FOV)
		{
			Vector TempTargetAbs;
			CalcAngle(hackManager.pLocal()->GetEyePosition(), vecHitbox, TempTargetAbs);
			auto flTempFOV = GetFov(pCmd->viewangles, TempTargetAbs);

			int NewHitBox = HitScan(pClient);
			if (NewHitBox >= 0)
			{
				if (flTempFOV < flFOV)
				{
					flFOV = flTempFOV;
					iBestTarget = i;
				}
			}
		}

		if (ragebotconfig.iTargetSelection == SELECTION_HEALTH)
		{
			int flTempHealth = pClient->GetHealth();

			int NewHitBox = HitScan(pClient);
			if (NewHitBox >= 0)
			{
				if (flTempHealth < flHealth)
				{
					flHealth = flTempHealth;
					iBestTarget = i;
				}
			}
		}

		if (ragebotconfig.iTargetSelection == SELECTION_DISTANCE)
		{
			Vector TempTargetAbs;
			CalcAngle(hackManager.pLocal()->GetEyePosition(), vecHitbox, TempTargetAbs);
			auto flTempFOVs = GetFov(pCmd->viewangles, TempTargetAbs);// shouldnt these be skipped by the if ( Options.Rage.Aimbot.Targeting ==  label? o well gey names inc
			auto flTempDistance = VectorDistance(hackManager.pLocal()->GetAbsOrigin(), pClient->GetAbsOrigin());
			if (flTempDistance < flDistance && flTempFOVs < flFOV)
			{
				int NewHitBox = HitScan(pClient);
				if (NewHitBox >= 0)
				{
					flFOV = flTempFOVs;
					flDistance = flTempDistance;
					iBestTarget = i;
				}
			}
		}

		if (ragebotconfig.iTargetSelection == SELECTION_THREAT)
		{
			auto flTempdistance = VectorDistance(hackManager.pLocal()->GetAbsOrigin(), pClient->GetAbsOrigin());
			auto flTempHealths = pClient->GetHealth();
			auto flTempThreat = flTempHealths / flTempdistance;
			flTempThreat += static_cast<int>(this->AimNearPlayer(pClient));
			if (pClient->IsTargetingLocal())
				flTempThreat += 100;
			if (flTempThreat > flThreatLevel)
			{
				int NewHitBox = HitScan(pClient);
				if (NewHitBox >= 0)
				{
					flThreatLevel = flTempThreat;
					iBestTarget = i;
				}
			}
		}

		if (ragebotconfig.iTargetSelection == SELECTION_FIRSTVALID)
		{
			if (pClient)
			{
				int NewHitBox = HitScan(pClient);
				if (NewHitBox >= 0)
				{
					iBestTarget = i;
				}
				else
					continue;
			}
		}
	}
	return iBestTarget;
}






float Aimbot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.0f;
	Vector Angles = View;
	Vector Origin = ViewOffSet;
	Vector Delta(0, 0, 0);
	Vector Forward(0, 0, 0);
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	VectorSubtract(AimPos, Origin, Delta);
	Normalize(Delta, Delta);
	FLOAT DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int Aimbot::HitScan(IClientEntity* pEntity)
{
	IClientEntity* pLocal = hackManager.pLocal();
	std::vector<int> hitboxes;
	bool AWall = ragebotconfig.bAutoWall;
	int hbox = ragebotconfig.iAutoFireHitbox;

	bool bRevert = true;

	for (int i = 0; i < hitscanconfig.hitboxes.size(); i++)
	{
		if (hitscanconfig.hitboxes[i].bselected)
			bRevert = false;
	}

		if (bRevert)
		{
			switch (hbox)
			{
			case 0: hitboxes.push_back((int)CSGOHitboxID::Head);
			case 1: hitboxes.push_back((int)CSGOHitboxID::Neck);
			case 2: hitboxes.push_back((int)CSGOHitboxID::Chest);
			case 3: hitboxes.push_back((int)CSGOHitboxID::Stomach);
			case 4: hitboxes.push_back((int)CSGOHitboxID::Pelvis);
			}
		}
		else
		{
			switch (hbox)
			{
			case 0: hitboxes.push_back((int)CSGOHitboxID::Head);
			case 1: hitboxes.push_back((int)CSGOHitboxID::Neck);
			case 2: hitboxes.push_back((int)CSGOHitboxID::Chest);
			case 3: hitboxes.push_back((int)CSGOHitboxID::Stomach);
			case 4: hitboxes.push_back((int)CSGOHitboxID::Pelvis);
			}

			if (hitscanconfig.hitboxes[0].bselected && hbox != 0)
				hitboxes.push_back((int)CSGOHitboxID::Head);

			if (hitscanconfig.hitboxes[1].bselected && hbox != 1)
			{
				hitboxes.push_back((int)CSGOHitboxID::Neck);
			}

			if (hitscanconfig.hitboxes[2].bselected)
			{
				if(hbox != 2)
				hitboxes.push_back((int)CSGOHitboxID::Chest);

				hitboxes.push_back((int)CSGOHitboxID::LowerChest);
				hitboxes.push_back((int)CSGOHitboxID::UpperChest);
			}

			if (hitscanconfig.hitboxes[3].bselected)
			{
				if (hbox != 3)
				hitboxes.push_back((int)CSGOHitboxID::Stomach);

				if (hbox != 4)
				hitboxes.push_back((int)CSGOHitboxID::Pelvis);
			}

			if (hitscanconfig.hitboxes[4].bselected)
			{
				hitboxes.push_back((int)CSGOHitboxID::LeftUpperArm);
				hitboxes.push_back((int)CSGOHitboxID::LeftLowerArm);
			}

			if (hitscanconfig.hitboxes[5].bselected)
			{
				hitboxes.push_back((int)CSGOHitboxID::RightUpperArm);
				hitboxes.push_back((int)CSGOHitboxID::RightLowerArm);
			}

			if (hitscanconfig.hitboxes[6].bselected)
			{
				hitboxes.push_back((int)CSGOHitboxID::LeftShin);
				hitboxes.push_back((int)CSGOHitboxID::LeftThigh);
				hitboxes.push_back((int)CSGOHitboxID::LeftFoot);
			}

			if (hitscanconfig.hitboxes[7].bselected)
			{
				hitboxes.push_back((int)CSGOHitboxID::RightShin);
				hitboxes.push_back((int)CSGOHitboxID::RightThigh);
				hitboxes.push_back((int)CSGOHitboxID::RightFoot);
			}
		}

		for (auto HitBoxID : hitboxes)
		{
			if (AWall)
			{
				Vector Point = GetHitboxPosition(pEntity, HitBoxID);


				float Damage = 0.f;

				if (CanHit(Point, &Damage))
				{
					float DamageCheck = ragebotconfig.flMinDmg;

					if (ragebotconfig.flMinDmg == 0 && Damage >= pEntity->GetHealth())
						DamageCheck = pEntity->GetHealth();
					else if (ragebotconfig.flMinDmg == 0)
						DamageCheck = pEntity->GetHealth() / 5;

					if (Damage >= DamageCheck)
					{
						return HitBoxID;
					}
				}
			}
			else
			{
				if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
					return HitBoxID;
			}
		}
	return -1;
}

void Aimbot::SyncWeaponHitchance() // Custom weapon settings
{
	IClientEntity* LocalPlayer = hackManager.pLocal();
	CBaseCombatWeapon* CSWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeaponHandle());

	if (!CSWeapon || GameUtils::isRevolver(CSWeapon))
	{
		WeaponHitchance = NULL;
		return;
	}

	if (GameUtils::IsPistol(CSWeapon))
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponPistolHitchance.GetValue();
	}
	else if (GameUtils::IsSniper(CSWeapon))
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponSniperHitchance.GetValue();
	}
	else if (GameUtils::IsRifle(CSWeapon))
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponRifleHitchance.GetValue();
	}
	else if (GameUtils::IsMP(CSWeapon))
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponSMGHitchance.GetValue();
	}
	else
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponOtherHitchance.GetValue();
	}
}


void Aimbot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	TargetID = this->TargetSelection(pCmd);
	CBaseCombatWeapon* WeaponHandle = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (WeaponHandle)
	{
		if (WeaponHandle->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(WeaponHandle)) return;

		if (ragebotconfig.bSyncHitchance)
			SyncWeaponHitchance(); // Sync hitchance with configs
		else 
			WeaponHitchance = 0.f; // Disable
	}
	else
	{
		// No weapon return.
		return;
	}

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (GameUtils::IsRevolver(pWeapon))
	{
		static int delay = 0;
		delay++;
		if (delay <= 15)pCmd->buttons |= IN_ATTACK;
		else delay = 0;
	}
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon)) return;
	}
	else return;

	if (TargetID != -1)
	{
		IClientEntity* BestTarget = I::EntList->GetClientEntity(TargetID);
		globals.Target = BestTarget;

		Vector hitbox;
		hitbox = GetHitboxPosition(BestTarget, HitScan(BestTarget)); // raw hitbox

		if (hitbox != Vector(0, 0, 0))
		{
			BestTarget->GetPredicted(hitbox);

			Vector qTarget;
			CalcAngle(hackManager.pLocal()->GetEyePosition(), hitbox, qTarget);
			if (Utils::Clamp(qTarget)) { pCmd->viewangles = qTarget; }

			if (ragebotconfig.bAutoScope && GameUtils::IsScopedWeapon(WeaponHandle) && !WeaponHandle->IsScoped())
			{
				pCmd->buttons |= IN_ATTACK2;
			}
			else
			{
				if (WeaponHandle->ChanceToHit(WeaponHitchance))
				{
					if (ragebotconfig.bAutoFire && ragebotconfig.iAutoFireMode == 0 && !(pCmd->buttons & IN_ATTACK) || ragebotconfig.bAutoFire && ragebotconfig.iAutoFireMode == 1 && ragebotconfig.iAutoFireKey >= 0 && GUI.GetKeyState(ragebotconfig.iAutoFireKey))
					{
						pCmd->buttons |= IN_ATTACK;
					}
				}
			}

			if (GameUtils::IsPistol(WeaponHandle))
			{
				if ((pCmd->buttons & IN_ATTACK) && (WeaponHandle->GetNextPrimaryAttack() - ((float)hackManager.pLocal()->GetTickBase() * I::Globals->interval_per_tick) > 0))
					pCmd->buttons &= ~IN_ATTACK;
			}

			if (!ragebotconfig.bSilent && !Menu::Window.RageBotTab4.PSilentAim.GetState())
			{
				I::Engine->SetViewAngles(qTarget);
			}

			if (ragebotconfig.bSilent && !Menu::Window.RageBotTab4.PSilentAim.GetState())
			{
				pCmd->viewangles = qTarget;
			}

			if (Menu::Window.RageBotTab4.PSilentAim.GetState()) {

				Vector oViewangles = pCmd->viewangles;
				float oForwardmove = pCmd->forwardmove;
				float oSidemove = pCmd->sidemove;

				if (GameUtils::CanShoot && pCmd->buttons & IN_ATTACK)
				{
					bSendPacket = false;
					iChokedticks++;
				}
				//So we dont kill ourselfs
				if (iChokedticks > iMaxChokedticks)
					bSendPacket = true;//this will overwrite fakelag values when psilent is on
				if (bSendPacket)
				{
					iChokedticks = 0;
					pCmd->viewangles = oViewangles;
					pCmd->forwardmove = oForwardmove;
					pCmd->sidemove = oSidemove;
				}
			}
		}
		if (resolverconfig.bBacktrack) pCmd->tick_count += TICKS_TO_TIME(I::Globals->interval_per_tick);
	}
}