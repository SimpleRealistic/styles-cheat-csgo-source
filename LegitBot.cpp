/*
Syn's AyyWare Framework 2015
*/

#include "LegitBot.h"
#include "RenderManager.h"
#include <iostream>
#include "RageBot.h"

void CLegitBot::Init()
{
	IsLocked = false;
	TargetID = -1;
}

void CLegitBot::Draw()
{

}

void CLegitBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	// Master switch
	if (!Menu::Window.LegitBotTab.AimbotEnable.GetState())
		return;

	// Aimbot
	if (Menu::Window.LegitBotTab.AimbotEnable.GetState())
		DoAimbot(bSendPacket, pCmd);

	// Triggerbot
	if (Menu::Window.LegitBotTab.TriggerEnable.GetState() && (!Menu::Window.LegitBotTab.TriggerKeyPress.GetState() || GUI.GetKeyState(Menu::Window.LegitBotTab.TriggerKeyBind.GetKey())))
		DoTrigger(pCmd);
}

void CLegitBot::SyncWeaponSettings()
{
	std::vector<int> HitBoxesToScan;
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pWeapon)
		return;

	if (GameUtils::IsPistol(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponPistSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponPistFoV.GetValue() * 2;

		RecoilControl = Menu::Window.LegitBotTab.WeaponPistRecoil.GetState();
		RecoilAmountX = Menu::Window.LegitBotTab.WeaponPistRecoilAmountX.GetValue() / 50;
		RecoilAmountY = Menu::Window.LegitBotTab.WeaponPistRecoilAmountY.GetValue() / 50;

		PSilent = Menu::Window.LegitBotTab.WeaponPistPSilent.GetState();
		Inacc = Menu::Window.LegitBotTab.WeaponPistInacc.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponPistHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			break;
		case 4:
			Multihitbox = true;
			break;
		}
	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponSnipSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponSnipFoV.GetValue() * 2;

		RecoilControl = Menu::Window.LegitBotTab.WeaponSnipRecoil.GetState();
		RecoilAmountX = Menu::Window.LegitBotTab.WeaponSnipRecoilAmountX.GetValue() / 50;
		RecoilAmountY = Menu::Window.LegitBotTab.WeaponSnipRecoilAmountY.GetValue() / 50;

		PSilent = Menu::Window.LegitBotTab.WeaponSnipPSilent.GetState();
		Inacc = Menu::Window.LegitBotTab.WeaponSnipInacc.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponSnipHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}
	}
	else
	{
		Speed = Menu::Window.LegitBotTab.WeaponMainSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMainFoV.GetValue() * 2;

		RecoilControl = Menu::Window.LegitBotTab.WeaponMainRecoil.GetState();
		RecoilAmountX = Menu::Window.LegitBotTab.WeaponMainRecoilAmountX.GetValue() / 50;
		RecoilAmountY = Menu::Window.LegitBotTab.WeaponMainRecoilAmountY.GetValue() / 50;

		PSilent = Menu::Window.LegitBotTab.WeaponMainPSilent.GetState();
		Inacc = Menu::Window.LegitBotTab.WeaponMainInacc.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponMainHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}
	}
}

// Functionality
void CLegitBot::DoAimbot(bool &bSendPacket, CUserCmd *pCmd)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	bool FindNewTarget = true;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			return;
		}
	}
	else
	{
		return;
	}

	// Make sure we have a good target
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = I::EntList->GetClientEntity(TargetID);

		if (pTarget  && pTarget->IsValidPlayer())
		{
			SyncWeaponSettings();
			if (HitBox >= 0)
			{
				Vector View; I::Engine->GetViewAngles(View);
				View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;
				float nFoV = FovToPlayer(hackManager.pLocal()->GetEyePosition(), View, pTarget, HitBox);
				if (nFoV < FoV)
					FindNewTarget = false;
			}
		}
	}

	// Find a new target, apparently we need to
	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		TargetID = GetTargetCrosshair();

		// Memes
		if (TargetID >= 0)
		{
			pTarget = I::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	SyncWeaponSettings();

	// If we finally have a good target
	if (TargetID >= 0 && pTarget)
	{
		//HitBox = (int)CSGOHitboxID::Head;//
		SyncWeaponSettings();

		// Key
		if (Menu::Window.LegitBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.LegitBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		Vector Point;
		Vector AimPoint;

		if (Multihitbox)
		{
			AimPoint = GetHitboxPosition(pTarget, besthitbox);
			Point = pTarget->GetPredicted(AimPoint);
		}
		else
		{
			AimPoint = GetHitboxPosition(pTarget, HitBox);
			Point = pTarget->GetPredicted(AimPoint);
		}

		if (AimAtPoint(AimPoint, pCmd, pLocal, bSendPacket))
		{
			//IsLocked = true;
			if (Menu::Window.LegitBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
			{
				pCmd->buttons |= IN_ATTACK;
			}
		}
	}

	// Auto Pistol
	static bool WasFiring = false;
	if (GameUtils::IsPistol(pWeapon) && Menu::Window.LegitBotTab.AimbotAutoPistol.GetState())
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			static bool WasFiring = false;
			WasFiring = !WasFiring;

			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}
	}
}

bool CLegitBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	if (pEntity && !pEntity->IsDormant() && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		ClientClass *pClientClass = pEntity->GetClientClass(); player_info_t pinfo;

		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && I::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
			{
				if (Menu::Window.LegitBotTab.Smokecheck.GetState()) {
					typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

					static DWORD GoesThroughSmokeOffset = (DWORD)U::Memory::FindPatternVersion2("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
					static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

					if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), pEntity->GetBonePos(6)))
						return false;
				}
				if (Multihitbox)
				{
					if (!pEntity->HasGunGameImmunity())
					{
						if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, 0) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 6) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 4))
							return true;
						else
							return false;
					}
				}
				else
				{
					if (!pEntity->HasGunGameImmunity() && GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBox))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool TargetMeetsTriggerRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && I::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
			{
				// Spawn Check
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}

void CLegitBot::DoTrigger(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	// Don't triggerbot with the knife..
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			return;
		}
	}
	else
		return;

	// Triggerbot
	// Get the view with the recoil
	Vector ViewAngles;
	I::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	// Build a ray going fowards at that angle
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	// Get ray start / end
	Vector end = hackManager.pLocal()->GetEyePosition() + fowardVec, endScreen;

	trace_t Trace;
	UTIL_TraceLine(hackManager.pLocal()->GetEyePosition(), end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt && 0 < Trace.hitgroup <= 7) // hitbox not hitgroup
	{
		if (TargetMeetsTriggerRequirements(Trace.m_pEnt) && !time < Menu::Window.LegitBotTab.TriggerDelay.GetValue())
		{
			float time = 0;
			time++;
			float delay = Menu::Window.LegitBotTab.TriggerDelay.GetValue() / 1000.f;

			if ((time * 64) < delay)
			{
				return;
			}
			else
			{
				pCmd->buttons |= IN_ATTACK;
				time = 0;
			}
		}
	}

	// Auto Pistol
	if (GameUtils::IsPistol(pWeapon) && Menu::Window.LegitBotTab.AimbotAutoPistol.GetState())
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			static bool WasFiring = false;
			WasFiring = !WasFiring;

			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}
	}
}

float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
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

int CLegitBot::GetTargetCrosshair()
{
	SyncWeaponSettings();
	int target = -1;
	float minFoV = FoV;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector View; I::Engine->GetViewAngles(View);
	View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	for (int i = 0; i < I::Engine->GetMaxClients(); i++)
	{
		IClientEntity *pEntity = I::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			if (Multihitbox)
			{
				float fov1 = FovToPlayer(hackManager.pLocal()->GetEyePosition(), View, pEntity, 0);
				float fov2 = FovToPlayer(hackManager.pLocal()->GetEyePosition(), View, pEntity, 4);
				float fov3 = FovToPlayer(hackManager.pLocal()->GetEyePosition(), View, pEntity, 6);

				if (fov1 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV)
				{
					FoV = fov1;
					target = i;
					besthitbox = 0;
				}
				if (fov2 < FoV || fov1 < FoV && fov2 < FoV || fov3 < FoV)
				{
					FoV = fov2;
					target = i;
					besthitbox = 4;
				}
				if (fov3 < FoV || fov1 < FoV && fov3 < FoV || fov2 < FoV)
				{
					FoV = fov3;
					target = i;
					besthitbox = 6;
				}
			}
			else
			{
				int NewHitBox = HitBox;
				if (NewHitBox >= 0)
				{
					float fov = FovToPlayer(hackManager.pLocal()->GetEyePosition(), View, pEntity, 0);
					if (fov < FoV)
					{
						FoV = fov;
						target = i;
					}
				}
			}
		}
	}
	return target;
}

bool CLegitBot::AimAtPoint(Vector point, CUserCmd *pCmd, IClientEntity* pLocal, bool &bSendPacket)
{
	if (point.Length() == 0) return false;

	Vector angles;

	CalcAngle(hackManager.pLocal()->GetEyePosition(), point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return false;
	}

	if (RecoilControl)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();

		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles.x -= AimPunch.x * RecoilAmountX;
			angles.y -= AimPunch.y * RecoilAmountY;
			GameUtils::NormaliseViewAngle(angles);
		}
	}

	IsLocked = true;
	//-----------------------------------------------

	Vector ang = angles - pCmd->viewangles;
	bool v = false;

	if (ang.Length() > Speed)
	{
		Normalize(ang, ang);
		ang *= Speed;
	}
	else
	{
		v = true;
	}

	if (PSilent)
	{
		Vector Oldview = pCmd->viewangles;
		Vector qAimAngles = pCmd->viewangles;
		float Oldsidemove = pCmd->sidemove;
		float Oldforwardmove = pCmd->forwardmove;

		static int ChokedPackets = -1;

		if (GameUtils::CanShoot() && ChokedPackets < 6)
		{
			bSendPacket = false;
			pCmd->viewangles += ang;
			pCmd->viewangles = angles;
			ChokedPackets++;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles = Oldview;
			pCmd->sidemove = Oldsidemove;
			pCmd->forwardmove = Oldforwardmove;
			ChokedPackets = -1;
		}

		pCmd->viewangles.z = 0;
	}
	else
	{
		pCmd->viewangles += ang;
		I::Engine->SetViewAngles(pCmd->viewangles);
	}
	return v;
}