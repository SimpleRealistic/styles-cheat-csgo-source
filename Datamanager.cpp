#include "Datamanager.h"
#include "CommonIncludes.h"
#include "RageBot.h"

#define MODULATIONFACOTR 1500
cPrediction DataManager;
void cPrediction::ReportMove(Vector  vOrigin, int iIndex)
{
	if (iIndex >= 128 || iIndex < 0)
		return;

	IClientEntity* pBaseEntity = I::EntList->GetBaseEntity(iIndex);
	if (!pBaseEntity)		return;

	IClientEntity* pLocalPlayer = hackManager.pLocal();
	if (!pLocalPlayer)		return;

	if (pBaseEntity->GetIndex() == pLocalPlayer->GetIndex()) return;

	if (!pBaseEntity->GetHealth()) return;



	matrixentry_t& Matrix = m_Matrix[iIndex];

	Matrix.dwFraction = timeGetTime() - Matrix.dwTime;
	Matrix.dwTime = timeGetTime();
	Matrix.vOrigin = vOrigin;
	Matrix.vecOrigins.push_back(vOrigin);
}

bool cPrediction::bGetCurOrigin(Vector& vOrigin, int iIndex)
{
	return bGetOrigin(vOrigin, iIndex, timeGetTime());
}
float get_network_latency()
{
	INetChannelInfo* nci = I::Engine->GetNetChannelInfo();
	if (nci != nullptr)
		return nci->GetAvgLatency(FLOW_OUTGOING);

	return 0.f;
}
int get_choked_ticks(int tickbase)
{
	float diff = I::Globals->curtime - tickbase;
	float latency = get_network_latency();
	return max(0.0f, diff);
}
Vector	Acceleration[64];
Vector OldNetworkPos[64];
Vector	OldVelocity[64];
void cPrediction::UpdatePlayerPos()
{
	for (int s = 1; s <= I::Globals->maxClients; ++s)
	{
		IClientEntity* pEntity = I::EntList->GetBaseEntity(s);
		auto pLocal = hackManager.pLocal();

		if (!pEntity)
			continue;

		if (!pEntity->IsValidPlayer())
			continue;

		if (!OldNetworkPos[s]) OldNetworkPos[s] = Vector(0, 0, 0);
		if (!OldVelocity[s]) OldVelocity[s] = Vector(0, 0, 0);
		if (!Acceleration[s]) Acceleration[s] = Vector(0, 0, 0);

		if (!OldNetworkPos[s] && pEntity->GetVelocity().Length() > 0) OldNetworkPos[s] = pEntity->GetAbsOrigin();
		if (!OldVelocity[s] && pEntity->GetVelocity().Length() > 0) OldVelocity[s] = pEntity->GetVelocity();
		if (!Acceleration[s] && pEntity->GetVelocity().Length() > 0) Acceleration[s] = Vector(0, 0, 0);

		if ((OldNetworkPos[s] != pEntity->GetAbsOrigin()) && pEntity->GetVelocity().Length() > 0)
		{
			Vector CurrentVelocity = pEntity->GetAbsOrigin() - OldNetworkPos[s];
			Acceleration[s] = CurrentVelocity - OldVelocity[s];
			if (pEntity != pLocal)
				(pEntity->GetVelocity()) = CurrentVelocity;

			OldNetworkPos[s] = pEntity->GetAbsOrigin();
			OldVelocity[s] = CurrentVelocity;
		}

		if (pEntity != pLocal)
			if (pEntity->GetChokedTicks() && pEntity->GetVelocity().Length() > 0)
				*pEntity->GetOriginPtr() = pEntity->GetAbsOrigin() + (pEntity->GetVelocity() + Acceleration[s]) * (pEntity->GetChokedTicks());
	}
}

bool cPrediction::bGetOrigin(Vector& vOrigin, int iIndex, DWORD dwTime)
{

	if (iIndex >= 128 || iIndex < 0)
		return false;

	IClientEntity* pBaseEntity = I::EntList->GetBaseEntity(iIndex);
	if (!pBaseEntity)		return false;

	IClientEntity* pLocalPlayer = hackManager.pLocal();
	if (!pLocalPlayer)		return false;

	if (pBaseEntity->GetIndex() == pLocalPlayer->GetIndex()) return false;

	if (!pBaseEntity->GetHealth()) return false;


	matrixentry_t& Matrix = m_Matrix[iIndex];

	if (Matrix.vecOrigins.size() == 0 || Matrix.dwFraction > MODULATIONFACOTR)
	{
		Matrix.dwFraction = 0;
		return false;
	}

	DWORD dwInterfraction = dwTime - Matrix.dwFraction;

	if (Matrix.vecOrigins.size() == 1)
	{
		vOrigin = Matrix.vOrigin;
		return true;
	}
	int iSize = (int)Matrix.vecOrigins.size();

	Vector vCurDistance = Matrix.vOrigin - Matrix.vecOrigins[iSize - 1];
	Vector vLastDistance = Matrix.vecOrigins[iSize - 1] - Matrix.vecOrigins[iSize - 2];

	float flInterp = vCurDistance.Length() - vLastDistance.Length();

	if (flInterp == 0.0) // constant or no acceleration
	{
		return true;
	}

	int iRecords = iSize - 1;

	float flAcceleration[2];
	float flAbsAcceleration = 1.0f;
	Vector vecAcceleration = vCurDistance - vLastDistance;

	if (iRecords >= 2)
	{
		Vector vOldDistance = Matrix.vecOrigins[iSize - 2] - Matrix.vecOrigins[iSize - 3];

		flAcceleration[0] = vCurDistance.Length() - vLastDistance.Length();
		flAcceleration[1] = vLastDistance.Length() - vOldDistance.Length();

		flAbsAcceleration = (flAcceleration[0] + flAcceleration[1]) / 2;
	}

	I::CVar->ConsoleColorPrintf(Color(201, 201, 201, 255), "Backtracked Information: Acceleration X: %i Acceleration Y: %i Choked Ticks: %i \n", flAcceleration[0], flAcceleration[1], pBaseEntity->GetChokedTicks());
	pBaseEntity->GetAbsOrigin() += ((vecAcceleration * (6)));
	//printf("%i", get_choked_ticks(pBaseEntity->GetTickBase()));
	return true;
}

void cPrediction::Reset(void)
{
	for (int i = 0; i < 128; i++)
	{
		m_Matrix[i].vecOrigins.clear();
		m_Matrix[i].dwFraction = 0;
		m_Matrix[i].dwTime = 0;
	}
}

//
//C_DataManager DataManager;
//
//void C_DataManager::Get(SDK::CBaseEntity *localPlayer)
//{
//	if (localPlayer == g_pTools->GetLocalPlayer()) return;
//
//
//	S_Data *slot = &m_data[localPlayer->GetIndex()];
//
//
//	slot->m_iSimulationTime = localPlayer->m_flSimulationTime();
//
//	if (!slot->m_iTickCount || slot->m_iSimulationTime)
//	{
//		slot->m_iTickCount = localPlayer->GetTickBase();
//		slot->m_iSimulationTime = localPlayer->m_flSimulationTime();
//	}
//	if ((fabs(slot->m_iSimulationTime - localPlayer->GetTickBase()) >= localPlayer->m_flSimulationTime()))
//	{
//		slot->m_iTickCount = localPlayer->GetTickBase();
//		slot->m_Velocity = localPlayer->GetVelocity();
//		slot->m_playerOrigin = localPlayer->GetAbsOrigin();
//		slot->m_iSimulationTime = localPlayer->m_flSimulationTime();
//		printf("Player logged on %i tick \n", slot->m_iTickCount);
//	}
//	else
//	{
//		//printf("Current %i tick, Saved tick %i \n", (localPlayer->GetTickBase() - localPlayer->GetChockedTicks()), slot->m_iTickCount);
//	}
//}
////=================================================
//void C_DataManager::Set(SDK::CBaseEntity *localPlayer)
//{
//	int tick = localPlayer->GetTickBase();
//
//	S_Data *slot = &m_data[localPlayer->GetIndex()];
//
//	bool validation[MAX_TARGETS];
//	Validate(localPlayer, slot, validation);
//
//
//	if (slot->m_Velocity != localPlayer->GetVelocity())
//	{
//		localPlayer->GetVelocity() = slot->m_Velocity;
//	}
//	if (slot->m_playerOrigin != SDK::Vector(0,0,0))
//	{
//		localPlayer->GetAbsOrigin() = slot->m_playerOrigin;
//		printf("Origin backtracked %i ticks in history \n", localPlayer->GetChokedTicks());
//	}
//}
////=================================================
//void C_DataManager::Validate(SDK::CBaseEntity *localPlayer, S_Data *slot, bool *validated)
//{
//	int var = -1;
//	validated[m_iTickCount] = ((fabs(slot->m_iSimulationTime - localPlayer->GetTickBase()) >= localPlayer->m_flSimulationTime()));
//	validated[m_iSimulationTime] = slot->m_iSimulationTime != 0;
//}