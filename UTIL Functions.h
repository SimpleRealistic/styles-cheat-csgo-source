#pragma once

#include "SDK.h"

namespace GameUtils
{
	void NormaliseViewAngle(Vector &angle);
	void NormalizeVector(Vector & vecIn);
	bool IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID);
	bool IsBallisticWeapon(void* weapon);
	bool IsPistol(void* weapon);
	bool isRevolver(void* weapon);
	bool IsSniper(void* weapon);
	bool IsG(void * weapon);
	bool IsRevolver(void* weapon);
	bool IsMP(void* weapon);
	bool IsRifle(void* weapon);
	bool IsScopedWeapon(void* weapon);
	int GetPlayerCompRank(IClientEntity* pEntity);
	bool CanShoot();



	static void ClampAngles(Vector &angles)
	{
		if (std::isnan(angles.x)) angles.x = 0.0f;
		if (std::isnan(angles.y)) angles.y = 0.0f;
		if (std::isnan(angles.z)) angles.z = 0.0f;
		if (std::isinf(angles.x)) angles.x = 0.0f;
		if (std::isinf(angles.y)) angles.y = 0.0f;
		if (std::isinf(angles.z)) angles.z = 0.0f;

		if (angles.x > 89.0f && angles.x <= 180.0f)
			angles.x = 89.0f;
		while (angles.x > 180.0f)
			angles.x -= 360.0f;
		while (angles.x < -89.0f)
			angles.x = -89.0f;
		while (angles.y > 180.0f)
			angles.y -= 360.0f;
		while (angles.y < -180.0f)
			angles.y += 360.0f;

		angles.z = 0;
	}
	static void Clamp(Vector &angles)
	{
		NormaliseViewAngle(angles);
		ClampAngles(angles);
	}
};

// Trace Line Memes
void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
	const IClientEntity *ignore, int collisionGroup, trace_t *ptr);

void TraceLine(const Vector & vecAbsStart, const Vector & vecAbsEnd, unsigned int mask, IClientEntity * ignore, trace_t * ptr);

void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);

bool IsBreakableEntity(IClientEntity* ent);

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace);

void SayInChat(const char *text);

Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox);

void ForceUpdate();