

#include "UTIL Functions.h"
#include "Utilities.h"
#include "Hacks.h"
#include "RenderManager.h"
#include "Autowall.h"
#include "Settings.h"

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)U::Memory::FindPatternVersion2("client.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}

void TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, IClientEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init2(vecAbsStart, vecAbsEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;
	I::Trace->TraceRay(ray, mask, &filter, ptr);
}


void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	static DWORD dwAddress = U::Memory::FindPatternVersion2("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");

	if (!dwAddress)
		return;

	_asm
	{
			MOV		EAX, filter
			LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	dwAddress
			ADD		ESP, 0xC
	}
}

bool IsBreakableEntity(IClientEntity* ent)
{
	typedef bool(__thiscall* IsBreakbaleEntity_t)(IClientEntity*);
	IsBreakbaleEntity_t IsBreakbaleEntityFn = (IsBreakbaleEntity_t)U::Memory::FindPatternVersion2("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");
	if (IsBreakbaleEntityFn)
		return IsBreakbaleEntityFn(ent);
	else
		return false;
}

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)U::Memory::FindPatternVersion2(("client.dll"), ("55 8B EC 83 EC 30 F3 0F 10 75"));

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}

void GameUtils::NormaliseViewAngle(Vector &angle)
{
	while (angle.y <= -180) angle.y += 360;
	while (angle.y > 180) angle.y -= 360;
	while (angle.x <= -180) angle.x += 360;
	while (angle.x > 180) angle.x -= 360;

	
	if (angle.x > 89.0) angle.x = 89;
	if (angle.x < -89.0) angle.x = -89;
	if (angle.y < -180) angle.y = -179.999;
	if (angle.y > 180) angle.y = 179.999;

	angle.z = 0;
}

void GameUtils::NormalizeVector(Vector& vecIn)
{
	for (int axis = 0; axis < 3; ++axis)
	{
		while (vecIn[axis] > 180.f)
			vecIn[axis] -= 360.f;

		while (vecIn[axis] < -180.f)
			vecIn[axis] += 360.f;
	}
	vecIn[2] = 0.f;
}

bool GameUtils::IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID)
{
	if (BoneID < 0) return false;

	trace_t Trace;
	Vector end = GetHitboxPosition(pEntity, BoneID);//pEntity->GetBonePos(BoneID);

	UTIL_TraceLine(hackManager.pLocal()->GetEyePosition(), end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt == pEntity)
	{
		return true;
	}

	if (Trace.fraction == 1.0f)
	{
		return true;
	}

	return false;

}

bool GameUtils::IsBallisticWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	return !(id >= WEAPON_KNIFE_CT && id <= WEAPON_KNIFE_T || id == 0 || id >= WEAPON_KNIFE_BAYONET);
}

bool GameUtils::IsPistol(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_DEAGLE,WEAPON_CZ75A,WEAPON_ELITE,WEAPON_USP_SILENCER,WEAPON_P250,WEAPON_HKP2000, WEAPON_TEC9,WEAPON_REVOLVER,WEAPON_FIVESEVEN,WEAPON_GLOCK };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


bool GameUtils::isRevolver(void* weapon)
{
	if (!weapon) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	return id == WEAPON_REVOLVER;
}

bool GameUtils::IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsG(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_FLASHBANG,WEAPON_HEGRENADE,WEAPON_SMOKEGRENADE,WEAPON_MOLOTOV,WEAPON_DECOY,WEAPON_INCGRENADE };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsRevolver(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_REVOLVER };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


bool GameUtils::IsMP(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMAC10 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponP90 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponUMP45 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponBizon || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMP7 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMP9)
		return true;
	else
		return false;
}

bool GameUtils::IsRifle(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AK47,WEAPON_AUG,WEAPON_FAMAS,WEAPON_GALILAR,WEAPON_M4A1,WEAPON_M4A1_SILENCER,WEAPON_SG556 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


bool GameUtils::IsScopedWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20, WEAPON_AUG, WEAPON_SG556 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	I::Engine->ClientCmd_Unrestricted(buffer);
}

bool CanWallbang(Vector& EndPos, float &Damage)
{
	if (CanHit(EndPos, &Damage))
	{
		return true;
	}
	return false;
}

Vector MultipointVectors[] = { Vector(0,0,0), Vector(0,0,1.5),Vector(0,0,3),Vector(0,0,4), Vector(0,0,-2), Vector(0,0,-4), Vector(0,0,4.8), Vector(0,0,5), Vector(0,0,5.4), Vector(0,3,0), Vector(3,0,0),Vector(-3,0,0),Vector(0,-3,0), Vector(0,2,4.2), Vector(0,-2,4.2), Vector(2,0,4.2), Vector(-2,0,4.2),  Vector(3.8,0,0), Vector(-3.8,0,0),Vector(0,3.6,0), Vector(0,-3.6,0), Vector(0,1.2,3.2), Vector(0,0.6,1.4), Vector(0,3.1,5.1), Vector(0,0,6.2), Vector(0,2.5,0), Vector(2.1,2.1,2.1) };

Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];

	if (pEntity->SetupBones(matrix, 128, 256, I::Engine->GetLastTimeStamp()))
	{
		studiohdr_t* hdr = I::ModelInfo->GetStudiomodel(pEntity->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
		mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

		if (hitbox)
		{
			Vector vMin, vMax, vCenter, sCenter;
			VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
			VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
			vCenter = (vMin + vMax) * 0.5;

			float HighestDamage = 0;
			float Damage = 0.f;

			if (ragebotconfig.bMultiPoint && hackManager.pLocal()->IsAlive()) // Credits Abortion Clinic
			{
				for (auto i = 0; i <= ragebotconfig.iMultipointType + 1; i++)
				{
					if (CanWallbang(vCenter + MultipointVectors[i], Damage))
						if (Damage > HighestDamage)
						{
							HighestDamage = Damage;
							vCenter += MultipointVectors[i];
						}
						else
							continue;
					else
						continue;
				}
			}
			return vCenter;
		}
	}
	return Vector(0, 0, 0);
}


int GameUtils::GetPlayerCompRank(IClientEntity* pEntity)
{
	static DWORD m_iCompetitiveRanking = 0x1A44;
	static DWORD GameResources = *(DWORD*)(U::Memory::FindPattern("client.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx")+0x2);
	
	return *(int*)((DWORD)GameResources + m_iCompetitiveRanking + (int)pEntity->GetIndex() * 4);
}

bool GameUtils::CanShoot()
{
	if (!hackManager.pLocal()) return false;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!hackManager.pLocal())
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = hackManager.pLocal()->GetTickBase() * I::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

void ForceUpdate()
{
	// Shh
	static DWORD clientstateaddr = U::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9\xF3\x0F\x11\x45\xF8\x83\xBF\xE8\x00\x00\x00\x02", "xx????xxxxxxxxxxxxxx");
	static uintptr_t pEngineBase = (uintptr_t)GetModuleHandleA("engine.dll");

	static uintptr_t pClientState = **(uintptr_t**)(U::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9", "xx????xx") + 2);

	static uintptr_t dwAddr1 = U::Memory::FindPattern("engine.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00", "x????x????x????xx????????");

	//E8 call is being used here
	static uintptr_t dwRelAddr = *(uintptr_t*)(dwAddr1 + 1);
	static uintptr_t sub_B5E60 = ((dwAddr1 + 5) + dwRelAddr);

	__asm
	{
		pushad
		mov edi, pClientState
		lea ecx, dword ptr[edi + 0x8]
		call sub_B5E60
		mov dword ptr[edi + 0x154], 0xFFFFFFFF
		popad
	}
}