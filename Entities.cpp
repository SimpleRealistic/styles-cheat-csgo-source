#include "Entities.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hacks.h"
#include "XorStr.hpp"
#include "Globals.h"

#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / I::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t ) ( I::Globals->interval_per_tick *( t ) )

bool IClientEntity::IsValidESP()
{
	if (!this)
		return false;

	if (this->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer)
		return false;

	if (this->IsDormant())
		return false;

	if (this->GetIndex() == hackManager.pLocal()->GetIndex() && !Menu::Window.VisualsTab.FilterSelf.GetState())
		return false;

	if (!this->IsAlive())
		return false;

	return true;
}

bool IClientEntity::IsValidPlayer()
{
	if (!this)
		return false;

	if (this->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer)
		return false;

	if (this->IsDormant())
		return false;

	if (this->GetIndex() == hackManager.pLocal()->GetIndex())
		return false;

	if (this->HasGunGameImmunity())
		return false;

	if (this->GetTeamNum() == hackManager.pLocal()->GetTeamNum())
		return false;

	if (!this->IsAlive())
		return false;

	return true;
}

CBaseCombatWeapon* IClientEntity::GetWeapon()
{
	static int iOffset = U::FindNet("DT_BaseCombatCharacter", "m_hActiveWeapon");
	ULONG pWeepEhandle = *(PULONG)((DWORD)this + iOffset);
	return (CBaseCombatWeapon*)(I::EntList->GetClientEntityFromHandle((HANDLE)pWeepEhandle));
}

Vector* IClientEntity::GetEyeAngles()
{
	return (Vector*)((DWORD)this + U::FindNet("DT_CSPlayer", "m_angEyeAngles"));
}

Vector* IClientEntity::GetHeadRotation()
{
	return (Vector*)((DWORD)this + U::FindNet("DT_CSPlayer", "m_angRotation"));
}

int IClientEntity::GetObserverMode()
{
	return *(int*)((DWORD)this + U::FindNet("DT_CSPlayer", "m_iObserverMode"));
}

char* IClientEntity::GetLastPlaceName()
{
	return (char*)((DWORD)this + U::FindNet("DT_CSPlayer", "m_szLastPlaceName"));
}

bool IClientEntity::IsTargetingLocal()
{
	Vector src, dst, forward;
	trace_t tr;

	if (!this)
		return false;

	Vector viewangle = *this->GetEyeAngles();

	AngleVectors(viewangle, &forward);
	forward *= 8142.f;
	src = this->GetEyePosition();
	dst = src + forward;

	TraceLine(src, dst, MASK_SHOT, this, &tr);

	if (tr.m_pEnt == hackManager.pLocal())
		return true;

	return false;
}

Vector IClientEntity::GetBonePos(int i)
{
	matrix3x4 boneMatrix[128];
	if (this->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, I::Engine->GetLastTimeStamp()))
	{
		return Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
	}
	return Vector(0, 0, 0);
}

int IClientEntity::GetChokedTicks() {
	if (this->GetSimulationTime() > globals.OldSimulationTime[this->GetIndex()])
		return fabs(this->GetSimulationTime() - globals.OldSimulationTime[this->GetIndex()]);
	return 0;
}

int IClientEntity::GetMoveType()
{
	return *(int*)((DWORD)this + 0x258);
}

float IClientEntity::GetMaxSpeed()
{
	return *(float*)((DWORD)this + U::FindNet("DT_CSPlayer", "m_flMaxspeed"));
}

bool IClientEntity::IsOnGround()
{
	return *this->GetFlags() & FL_ONGROUND;
}

bool IClientEntity::IsInAir()
{
	return !(*this->GetFlags() & FL_ONGROUND);
}

Vector IClientEntity::GetVecOrigin()
{
	return *(Vector*)((DWORD)this + 0x134);
}