#include "EnginePrediction.h"
#include "Hacks.h"
#include "NetVars.h"
#include "XorStr.hpp"

Vector vecBaseVelocity;
float flFallVelocity;
float m_accuracyPenalty;


int GetIDToOffsetIndex(int id)
{
	switch (id)
	{
	case WEAPON_GLOCK:
		return OFFSETINDEX_GLOCK;
		break;
	case OFFSETINDEX_FAMAS:
		return OFFSETINDEX_FAMAS;
		break;
	case WEAPON_M4A1:
		return OFFSETINDEX_M4A1;
		break;
	case WEAPON_USP:
		return OFFSETINDEX_USP;
		break;

	default:
		break;
	}
	return -1;
}

void PredictWeapon(IClientEntity *localPlayer)
{
	CBaseCombatWeapon *weapon = localPlayer->GetWeapon();

	if (!weapon)
		return;

	bool shouldupdate = true;
	int id = weapon->GetWeaponId();
	int offsetIndex = GetIDToOffsetIndex(id);

	if ((id == WEAPON_GLOCK) || (id == WEAPON_FAMAS))
	{
		if (weapon->IsSpecialMode(offsetIndex) && weapon->HasBurstShotsRemaining(offsetIndex))
		{
			shouldupdate = false;
		}
	}
	if (shouldupdate)
	{
		m_accuracyPenalty = weapon->GetInaccuracy();
		weapon->UpdateAccPenalty();
	}
}

void StartPrediction(CUserCmd* pCmd)
{
	auto bMoveData = *reinterpret_cast<void**>(reinterpret_cast<DWORD>(I::GameMovement) + 0x8);

	if (I::MoveHelper && I::Engine->IsConnected() && I::Engine->IsInGame() && hackManager.pLocal()->IsAlive())
	{
		vecBaseVelocity = hackManager.pLocal()->GetBaseVelocity();
		flFallVelocity = *hackManager.pLocal()->GetFallVelocity();

		I::MoveHelper->SetHost(hackManager.pLocal());

		*I::PredictionRandomSeed = pCmd->random_seed;
		float curtime = I::Globals->curtime;
		float frametime = I::Globals->frametime;
		int iFlags = *hackManager.pLocal()->GetFlags();

		I::Globals->curtime = (float)hackManager.pLocal()->GetTickBase() * I::Globals->interval_per_tick;
		I::Globals->frametime = I::Globals->interval_per_tick;

		I::GameMovement->StartTrackPredictionErrors(hackManager.pLocal());
		I::Prediction->SetupMove(hackManager.pLocal(), pCmd, nullptr, bMoveData);
		I::GameMovement->ProcessMovement(hackManager.pLocal(), bMoveData);
		I::Prediction->FinishMove(hackManager.pLocal(), pCmd, bMoveData);
		I::GameMovement->FinishTrackPredictionErrors(hackManager.pLocal());

		I::Globals->curtime = curtime;
		I::Globals->frametime = frametime;
		*hackManager.pLocal()->GetFlags() = iFlags;
		*I::PredictionRandomSeed = -1;

		I::MoveHelper->SetHost(nullptr);

		hackManager.pLocal()->GetBaseVelocity() = vecBaseVelocity;
		*hackManager.pLocal()->GetFallVelocity() = flFallVelocity;
		PredictWeapon(hackManager.pLocal());
	}
}