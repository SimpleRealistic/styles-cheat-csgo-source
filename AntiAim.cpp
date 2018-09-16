#include "Ragebot.h"
#include "QAngle.hpp"
#include "entities.h"
#include "Resolver.h"
#include "Globals.h"

#define rad(a) a * 0.01745329251
#define RandomInt(min, max) (rand() % (max - min + 1) + min)

void AngleVectors4(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;
	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[2]), &sp, &cp);
	SinCos(DEG2RAD(angles[3]), &sr, &cr);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

float Bolbilize2(float Yaw)
{
	if (Yaw > 180)
	{
		Yaw -= (round(Yaw / 360) * 360.f);
	}
	else if (Yaw < -180)
	{
		Yaw += (round(Yaw / 360) * -360.f);
	}
	return Yaw;
}

float RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

namespace AntiAimYaw 
{

}

bool EntityIsValid()
{
	for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++) //nice one psilent INT = int
	{
		IClientEntity* pEntity = (IClientEntity*)I::EntList->GetClientEntity(i);
		if (pEntity == NULL)
			continue;
		if (!pEntity->IsAlive())
			continue;
		if (pEntity == hackManager.pLocal())
			continue;
		if (pEntity->IsDormant())
			continue;
		if (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum())
			continue;
		player_info_t info;
		if (!(I::Engine->GetPlayerInfo(pEntity->GetIndex(), &info)))
			continue;
		return true;
	}
	return false;
}

void Aimbot::AtTargets(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon || !GameUtils::IsBallisticWeapon(pWeapon))
		return;

	Vector eye_position = pLocal->GetEyePosition();

	float best_dist = pWeapon->GetCSWpnData()->flRange;

	IClientEntity* target = nullptr;

	for (int i = 0; i < I::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = I::EntList->GetClientEntity(i);
		if (pEntity->IsValidPlayer())
		{
			if (TargetID != -1)
				target = I::EntList->GetClientEntity(TargetID);
			else
				target = pEntity;

			Vector target_position = target->GetEyePosition();

			float temp_dist = eye_position.DistTo(target_position);

			if (best_dist > temp_dist)
			{
				best_dist = temp_dist;
				CalcAngle(eye_position, target_position, pCmd->viewangles);
			}
		}
	}
}

void inline SinCos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

void AngleVectors(const QAngle &angles, Vector *forward)
{
	float	sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);

	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}

void VectorSubtract1(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

void NormalizeNum(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

float fov_player(Vector ViewOffSet, Vector View, IClientEntity* entity, int hitbox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);

	Vector AimPos = hackManager.pLocal()->GetBonePos(8);

	VectorSubtract1(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	NormalizeNum(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;

	IClientEntity* local_player = hackManager.pLocal();

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetOrigin() + local_player->GetViewOffset();

	Vector angles;
	I::Engine->GetViewAngles(angles);

	for (int i = 1; i <= I::Globals->maxClients; i++)
	{
		IClientEntity *entity = I::EntList->GetClientEntity(i);

		if (!entity || !entity->IsAlive() || entity->GetTeamNum() == local_player->GetTeamNum() || entity->IsDormant() || entity == local_player)
			continue;

		float fov = fov_player(local_position, angles, entity, 0);

		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}

	return index;
}

void freestanding(CUserCmd* pCmd)
{
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	auto leyepos = hackManager.pLocal()->GetOrigin() + hackManager.pLocal()->GetViewOffset();
	auto headpos = hackManager.pLocal()->GetBonePos(8);
	auto origin = hackManager.pLocal()->GetOrigin();

	auto checkWallThickness = [&](IClientEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetOrigin() + pPlayer->GetViewOffset();

		Ray_t ray;
		ray.Init(newhead, eyepos);

		CTraceFilterSkipTwoEntities filter(pPlayer, hackManager.pLocal());

		trace_t trace1, trace2;
		I::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.endpos;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		I::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.endpos;

		float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) + 3.f;
		return endpos1.Dist(endpos2) + add / 3;
	};

	int index = closest_to_crosshair();

	IClientEntity* entity;

	if (index != -1)
		entity = I::EntList->GetClientEntity(index);

	float step = (2 * PI) / 17.f; // One PI = half a circle ( for stacker cause low iq :sunglasses: ), 28

	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}

	if (no_active)
		pCmd->viewangles.y += 180.f;
	else
		pCmd->viewangles.y = RAD2DEG(bestrotation);
}

void Aimbot::RageAntiAim(CUserCmd *pCmd, bool &bSendPacket)
{
	CBaseCombatWeapon* CSWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	IClientEntity* pLocal = hackManager.pLocal();
	float PlayerVelocity = abs(pLocal->GetVelocity().Length());
	float server_time = pLocal->GetTickBase() * I::Globals->interval_per_tick;
	bool PlayerMoving;

#pragma region Antiaim Checks

	if (PlayerVelocity > 0.1f)
		PlayerMoving = true;
	else
		PlayerMoving = false;

	if (pCmd->buttons & IN_USE) // Pickup Weapons
		return;

	if (pLocal->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	if (pLocal->GetMoveType() == MOVETYPE_LADDER && PlayerMoving) // Climb Ladders
		return;

	if ((pCmd->buttons & IN_ATTACK) && !Menu::Window.RageBotTab4.PSilentAim.GetState())
		return;

#pragma

	if (CSWeapon)
	{
		CSWeaponInfo* pWeaponInfo = CSWeapon->GetCSWpnData();

		if (!GameUtils::IsBallisticWeapon(CSWeapon))
		{
			if (Menu::Window.RageBotTab4.AntiAimKnife.GetState())
			{
				if (!GameUtils::CanShoot() || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else
			{
				return;
			}
		}
	}

	static int iChoked = -1;
	iChoked++;
	if (iChoked < 1)
	{
		bSendPacket = false;
		RealYaw(pCmd, bSendPacket);
	}
	else
	{
		bSendPacket = true;
		FakeYaw(pCmd, bSendPacket);
		iChoked = -1;
	}
}

void Aimbot::RealYaw(CUserCmd *pCmd, bool &bSendPacket)
{
	CBaseCombatWeapon* CSWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	IClientEntity* pLocal = hackManager.pLocal();
	float PlayerVelocity = abs(pLocal->GetVelocity().Length());
	float server_time = pLocal->GetTickBase() * I::Globals->interval_per_tick;
	bool PlayerMoving;

#pragma region Antiaim Checks

	if (PlayerVelocity > 0.1f)
		PlayerMoving = true;
	else
		PlayerMoving = false;

	if (pCmd->buttons & IN_USE) // Pickup Weapons
		return;

	if (pLocal->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	if (pLocal->GetMoveType() == MOVETYPE_LADDER && PlayerMoving) // Climb Ladders
		return;

	if ((pCmd->buttons & IN_ATTACK) && !Menu::Window.RageBotTab4.PSilentAim.GetState())
		return;

#pragma

	if (CSWeapon)
	{
		CSWeaponInfo* pWeaponInfo = CSWeapon->GetCSWpnData();

		if (!GameUtils::IsBallisticWeapon(CSWeapon))
		{
			if (Menu::Window.RageBotTab4.AntiAimKnife.GetState())
			{
				if (!GameUtils::CanShoot() || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else
			{
				return;
			}
		}
	}

	static bool jitter = false; jitter = !jitter;

	switch (Menu::Window.RageBotTab4.AntiAimPitch.GetIndex())
	{
	case 1:
		pCmd->viewangles.x = 70.4;
		break;

	case 2:
		pCmd->viewangles.x = 89.0;
		break;

	case 3:
		pCmd->viewangles.x = -89.0;
		break;
	}

	if (!PlayerMoving)
	{
		switch (Menu::Window.RageBotTab4.AntiAimStandingYaw.GetIndex())
		{
		case 1:
			pCmd->viewangles.y -= 180;
			break;

		case 2:
			pCmd->viewangles.y -= 0;

			break;

		case 3:
			pCmd->viewangles.y += 90;

			break;

		case 4:
			pCmd->viewangles.y -= 90;

			break;

		case 5:
			pCmd->viewangles.y = (float)(fmod(server_time / 0.39 * 360.0f, 360.0f));

			break;

		case 6:
			if (jitter)
			{
				pCmd->viewangles.y += 165;

			}
			else
			{
				pCmd->viewangles.y -= 165;

			}

			break;

		case 7:
			if (jitter)
			{
				pCmd->viewangles.y += 15;

			}
			else
			{
				pCmd->viewangles.y -= 15;

			}

			break;

		case 8:
			if (jitter)
			{
				pCmd->viewangles.y += 85;

			}
			else
			{
				pCmd->viewangles.y += 105;

			}

			break;

		case 9:
			if (jitter)
			{
				pCmd->viewangles.y -= 85;

			}
			else
			{
				pCmd->viewangles.y -= 105;

			}

			break;

		case 10:

			static float last_real;
			bool no_active = true;
			float bestrotation = 0.f;
			float highestthickness = 0.f;
			Vector besthead;

			auto leyepos = hackManager.pLocal()->GetOrigin() + hackManager.pLocal()->GetViewOffset();
			auto headpos = hackManager.pLocal()->GetBonePos(8);
			auto origin = hackManager.pLocal()->GetOrigin();

			auto checkWallThickness = [&](IClientEntity* pPlayer, Vector newhead) -> float
			{
				Vector endpos1, endpos2;
				Vector eyepos = pPlayer->GetOrigin() + pPlayer->GetViewOffset();

				Ray_t ray;
				ray.Init(newhead, eyepos);

				CTraceFilterSkipTwoEntities filter(pPlayer, hackManager.pLocal());

				trace_t trace1, trace2;
				I::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

				if (trace1.DidHit())
					endpos1 = trace1.endpos;
				else
					return 0.f;

				ray.Init(eyepos, newhead);
				I::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

				if (trace2.DidHit())
					endpos2 = trace2.endpos;

				float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) + 3.f;
				return endpos1.Dist(endpos2) + add / 3;
			};

			int index = closest_to_crosshair();

			IClientEntity* entity;

			if (index != -1)
				entity = I::EntList->GetClientEntity(index);

			float step = (2 * PI) / 17.f; // One PI = half a circle ( for stacker cause low iq :sunglasses: ), 28

			float radius = fabs(Vector(headpos - origin).Length2D());

			if (index == -1)
			{
				no_active = true;
			}
			else
			{
				for (float rotation = 0; rotation < (PI * 2.0); rotation += step)
				{
					Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

					float totalthickness = 0.f;

					no_active = false;

					totalthickness += checkWallThickness(entity, newhead);

					if (totalthickness > highestthickness)
					{
						highestthickness = totalthickness;
						bestrotation = rotation;
						besthead = newhead;
					}
				}
			}

			if (no_active)
				pCmd->viewangles.y -= 180;
			else
				pCmd->viewangles.y = RAD2DEG(bestrotation);

			break;
		}
	}
	else
	{
		switch (Menu::Window.RageBotTab4.AntiAimMovingYaw.GetIndex())
		{
		case 1:
			pCmd->viewangles.y -= 180;
			break;

		case 2:
			pCmd->viewangles.y -= 0;

			break;

		case 3:
			pCmd->viewangles.y += 90;

			break;

		case 4:
			pCmd->viewangles.y -= 90;

			break;

		case 5:
			pCmd->viewangles.y = (float)(fmod(server_time / 0.39 * 360.0f, 360.0f));

			break;

		case 6:
			if (jitter)
			{
				pCmd->viewangles.y += 165;

			}
			else
			{
				pCmd->viewangles.y -= 165;

			}

			break;

		case 7:
			if (jitter)
			{
				pCmd->viewangles.y += 15;

			}
			else
			{
				pCmd->viewangles.y -= 15;

			}

			break;

		case 8:
			if (jitter)
			{
				pCmd->viewangles.y += 85;

			}
			else
			{
				pCmd->viewangles.y += 105;

			}

			break;

		case 9:
			if (jitter)
			{
				pCmd->viewangles.y -= 85;

			}
			else
			{
				pCmd->viewangles.y -= 105;

			}

			break;

		case 10:

			static float last_real;
			bool no_active = true;
			float bestrotation = 0.f;
			float highestthickness = 0.f;
			Vector besthead;

			auto leyepos = hackManager.pLocal()->GetOrigin() + hackManager.pLocal()->GetViewOffset();
			auto headpos = hackManager.pLocal()->GetBonePos(8);
			auto origin = hackManager.pLocal()->GetOrigin();

			auto checkWallThickness = [&](IClientEntity* pPlayer, Vector newhead) -> float
			{
				Vector endpos1, endpos2;
				Vector eyepos = pPlayer->GetOrigin() + pPlayer->GetViewOffset();

				Ray_t ray;
				ray.Init(newhead, eyepos);

				CTraceFilterSkipTwoEntities filter(pPlayer, hackManager.pLocal());

				trace_t trace1, trace2;
				I::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

				if (trace1.DidHit())
					endpos1 = trace1.endpos;
				else
					return 0.f;

				ray.Init(eyepos, newhead);
				I::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

				if (trace2.DidHit())
					endpos2 = trace2.endpos;

				float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) + 3.f;
				return endpos1.Dist(endpos2) + add / 3;
			};

			int index = closest_to_crosshair();

			IClientEntity* entity;

			if (index != -1)
				entity = I::EntList->GetClientEntity(index);

			float step = (2 * PI) / 17.f; // One PI = half a circle ( for stacker cause low iq :sunglasses: ), 28

			float radius = fabs(Vector(headpos - origin).Length2D());

			if (index == -1)
			{
				no_active = true;
			}
			else
			{
				for (float rotation = 0; rotation < (PI * 2.0); rotation += step)
				{
					Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

					float totalthickness = 0.f;

					no_active = false;

					totalthickness += checkWallThickness(entity, newhead);

					if (totalthickness > highestthickness)
					{
						highestthickness = totalthickness;
						bestrotation = rotation;
						besthead = newhead;
					}
				}
			}

			if (no_active)
				pCmd->viewangles.y -= 180;
			else
				pCmd->viewangles.y = RAD2DEG(bestrotation);

			break;
		}
	}
}

void Aimbot::FakeYaw(CUserCmd *pCmd, bool &bSendPacket)
{
	CBaseCombatWeapon* CSWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	IClientEntity* pLocal = hackManager.pLocal();
	float PlayerVelocity = abs(pLocal->GetVelocity().Length());
	float server_time = pLocal->GetTickBase() * I::Globals->interval_per_tick;
	bool PlayerMoving;

#pragma region Antiaim Checks

	if (PlayerVelocity > 0.1f)
		PlayerMoving = true;
	else
		PlayerMoving = false;

	if (pCmd->buttons & IN_USE) // Pickup Weapons
		return;

	if (pLocal->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	if (pLocal->GetMoveType() == MOVETYPE_LADDER && PlayerMoving) // Climb Ladders
		return;

	if ((pCmd->buttons & IN_ATTACK) && !Menu::Window.RageBotTab4.PSilentAim.GetState())
		return;

#pragma

	if (CSWeapon)
	{
		CSWeaponInfo* pWeaponInfo = CSWeapon->GetCSWpnData();

		if (!GameUtils::IsBallisticWeapon(CSWeapon))
		{
			if (Menu::Window.RageBotTab4.AntiAimKnife.GetState())
			{
				if (!GameUtils::CanShoot() || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else
			{
				return;
			}
		}
	}

	static bool jitter = false; jitter = !jitter;

	switch (Menu::Window.RageBotTab4.AntiAimPitch.GetIndex())
	{
	case 1:
		pCmd->viewangles.x = 70.4;
		break;

	case 2:
		pCmd->viewangles.x = 89.0;
		break;

	case 3:
		pCmd->viewangles.x = -89.0;
		break;
	}

	switch (Menu::Window.RageBotTab4.AntiAimFakeYaw.GetIndex())
	{
	case 1:
		pCmd->viewangles.y -= 180;
		break;

	case 2:
		pCmd->viewangles.y -= 0;

		break;

	case 3:
		pCmd->viewangles.y += 90;

		break;

	case 4:
		pCmd->viewangles.y -= 90;

		break;

	case 5:
		pCmd->viewangles.y = (float)(fmod(server_time / 0.39 * 360.0f, 360.0f));

		break;

	case 6:
		if (jitter)
		{
			pCmd->viewangles.y += 165;

		}
		else
		{
			pCmd->viewangles.y -= 165;

		}

		break;

	case 7:
		if (jitter)
		{
			pCmd->viewangles.y += 15;

		}
		else
		{
			pCmd->viewangles.y -= 15;

		}

		break;

	case 8:
		if (jitter)
		{
			pCmd->viewangles.y += 85;

		}
		else
		{
			pCmd->viewangles.y += 105;

		}

		break;

	case 9:
		if (jitter)
		{
			pCmd->viewangles.y -= 85;

		}
		else
		{
			pCmd->viewangles.y -= 105;

		}
		break;

	case 10:

		static float last_real;
		bool no_active = true;
		float bestrotation = 0.f;
		float highestthickness = 0.f;
		Vector besthead;

		auto leyepos = hackManager.pLocal()->GetOrigin() + hackManager.pLocal()->GetViewOffset();
		auto headpos = hackManager.pLocal()->GetBonePos(8);
		auto origin = hackManager.pLocal()->GetOrigin();

		auto checkWallThickness = [&](IClientEntity* pPlayer, Vector newhead) -> float
		{
			Vector endpos1, endpos2;
			Vector eyepos = pPlayer->GetOrigin() + pPlayer->GetViewOffset();

			Ray_t ray;
			ray.Init(newhead, eyepos);

			CTraceFilterSkipTwoEntities filter(pPlayer, hackManager.pLocal());

			trace_t trace1, trace2;
			I::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

			if (trace1.DidHit())
				endpos1 = trace1.endpos;
			else
				return 0.f;

			ray.Init(eyepos, newhead);
			I::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

			if (trace2.DidHit())
				endpos2 = trace2.endpos;

			float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) + 3.f;
			return endpos1.Dist(endpos2) + add / 3;
		};

		int index = closest_to_crosshair();

		IClientEntity* entity;

		if (index != -1)
			entity = I::EntList->GetClientEntity(index);

		float step = (2 * PI) / 17.f; // One PI = half a circle ( for stacker cause low iq :sunglasses: ), 28

		float radius = fabs(Vector(headpos - origin).Length2D());

		if (index == -1)
		{
			no_active = true;
		}
		else
		{
			for (float rotation = 0; rotation < (PI * 2.0); rotation += step)
			{
				Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

				float totalthickness = 0.f;

				no_active = false;

				totalthickness += checkWallThickness(entity, newhead);

				if (totalthickness > highestthickness)
				{
					highestthickness = totalthickness;
					bestrotation = rotation;
					besthead = newhead;
				}
			}
		}

		if (no_active)
			pCmd->viewangles.y -= 0;
		else
			pCmd->viewangles.y = RAD2DEG(bestrotation + 180);

		break;
	}
}