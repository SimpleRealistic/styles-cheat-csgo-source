#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "DLLMain.h"
#include <time.h>
#include "QAngle.hpp"
#include "Globals.h"

#define charenc( s ) ( s )

#define M_PI		3.14159265358979323846f
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )

Vector AutoStrafeView;

static Vector Normalize(Vector& angs)
{
	while (angs.y < -180.0f)
		angs.y += 360.0f;
	while (angs.y > 180.0f)
		angs.y -= 360.0f;
	if (angs.x > 89.0f)
		angs.x = 89.0f;
	if (angs.x < -89.0f)
		angs.x = -89.0f;
	angs.z = 0;
	return angs;
}
static vec_t Normalize_y(vec_t ang)
{
	while (ang < -180.0f)
		ang += 360.0f;
	while (ang > 180.0f)
		ang -= 360.0f;
	return ang;
}
void AngleVectors2(const Vector& angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void CMiscHacks::Init()
{
	// Any init
}
void CMiscHacks::Draw()
{
	// Any drawing	
}
void CMiscHacks::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	if (hackManager.pLocal()->IsAlive())
	{
		Fakelag(pCmd, bSendPacket);

		Vector OrigAng = pCmd->viewangles;
		Normalize(OrigAng);
		BhopMovement(pCmd, OrigAng);
	}


	if (Menu::Window.MiscTab.Logs.GetState())
	{

		ConVar* Developer = I::CVar->FindVar("developer");
		*(float*)((DWORD)&Developer->fnChangeCallback + 0xC) = NULL;
		Developer->SetValue("1");

		ConVar* con_filter_enable = I::CVar->FindVar("con_filter_enable");
		*(float*)((DWORD)&con_filter_enable->fnChangeCallback + 0xC) = NULL;
		con_filter_enable->SetValue("2");

		ConVar* con_filter_text = I::CVar->FindVar("con_filter_text");
		*(float*)((DWORD)&con_filter_text->fnChangeCallback + 0xC) = NULL;
		con_filter_text->SetValue("[selytS]");
	}

	//BhopMovement(pCmd);

	namechanger.Manage();
	chatspammer.Manage();
	radiospammer.Manage();
	killspammer.Manage();

	if (miscconfig.iAirStuckKey >= 0 && GUI.GetKeyState(miscconfig.iAirStuckKey))
	{
		pCmd->tick_count = 0xFFFFF;
	}
}

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

/*void CMiscHacks::BhopMovement(CUserCmd *pCmd)
{
int MoveType = hackManager.pLocal()->GetMoveType();

if (miscconfig.bCircleStrafer && GetAsyncKeyState('C') && !cstrafer.IsActive && (pCmd->buttons & IN_SPEED) && !(MoveType == MOVETYPE_NOCLIP || MoveType == MOVETYPE_LADDER))
cstrafer.Start(pCmd);

if (miscconfig.bAutoJump && (pCmd->buttons & IN_JUMP) && hackManager.pLocal()->IsInAir())
pCmd->buttons &= ~IN_JUMP;

if (miscconfig.bAutoStrafe && !(MoveType == MOVETYPE_NOCLIP || MoveType == MOVETYPE_LADDER) && !cstrafer.IsActive)
{
switch (miscconfig.iAutoStrafeType)
{
case 0:
if (!(hackManager.pLocal()->GetFlags() & FL_ONGROUND))
{
pCmd->forwardmove = 0.f;
pCmd->sidemove = pCmd->mousedx < 0.0f ? -450.f : 450.f;
}
case 1:
if (!(hackManager.pLocal()->GetFlags() & FL_ONGROUND))
{
if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
pCmd->sidemove = pCmd->mousedx < 0.0f ? -450.f : 450.f;
}
else {
pCmd->forwardmove = 4200.0f / hackManager.pLocal()->GetVelocity().Length2D();
pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
if (pCmd->forwardmove > 450.f)
pCmd->forwardmove = 450.f;
}
}
else if (pCmd->buttons & IN_JUMP) {
pCmd->forwardmove = 450.f;
}
}
}

if (miscconfig.bCircleStrafer && cstrafer.IsActive)
cstrafer.Strafe(pCmd);
else if (cstrafer.IsActive)
cstrafer.IsActive = false;
}*/

float RightMovement;
bool IsActive;
float StrafeAngle;

float GetTraceFractionWorldProps(Vector startpos, Vector endpos) {

	Ray_t ray;
	trace_t tr;
	CTraceFilterWorldOnly filter;

	ray.Init(startpos, endpos);

	I::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	return tr.fraction;
}

void Start(CUserCmd* pCmd)
{
	auto pLocal = hackManager.pLocal();
	StrafeAngle = 0;
	IsActive = true;

	Vector CurrentAngles;
	I::Engine->GetViewAngles(CurrentAngles);
	CurrentAngles.x = 0;
	Vector Forward = CurrentAngles.Direction();
	Vector Right = Forward.Cross(Vector(0, 0, 1));
	Vector Left = Vector(Right.x * -1.0f, Right.y * -1.0f, Right.z);

	float LeftPath = GetTraceFractionWorldProps(pLocal->GetOrigin() + Vector(0, 0, 10), pLocal->GetOrigin() + Left * 250.0f + Vector(0, 0, 10));
	float RightPath = GetTraceFractionWorldProps(pLocal->GetOrigin() + Vector(0, 0, 10), pLocal->GetOrigin() + Right * 250.0f + Vector(0, 0, 10));

	if (LeftPath > RightPath)
		RightMovement = -1;
	else
		RightMovement = 1;
}

void Strafe(CUserCmd* pCmd)
{
	auto pLocal = hackManager.pLocal();
	Vector Velocity = pLocal->GetVelocity();
	Velocity.z = 0;
	float Speed = Velocity.Length();
	if (Speed < 45) Speed = 45;

	float FinalPath = GetTraceFractionWorldProps(pLocal->GetOrigin() + Vector(0, 0, 10), pLocal->GetOrigin() + Vector(0, 0, 10) + Velocity / 3.0f);
	float DeltaAngle = RightMovement * fmax((275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / I::Globals->interval_per_tick)) * 4.20f, 2.0f);
	StrafeAngle += DeltaAngle;

	if (!(*pLocal->GetFlags() & FL_ONGROUND))
		pCmd->buttons &= ~IN_JUMP;
	else
		pCmd->buttons |= IN_JUMP;

	if (fabs(StrafeAngle) >= 360.0f)
	{
		StrafeAngle = 0.0f;
		IsActive = false;
		RightMovement = 0;
	}
	else
	{
		pCmd->forwardmove = cos((StrafeAngle + 90 * RightMovement) * (M_PI / 180.0f)) * pLocal->GetMaxSpeed();
		pCmd->sidemove = sin((StrafeAngle + 90 * RightMovement) * (M_PI / 180.0f)) * pLocal->GetMaxSpeed();
	}
}


void CMiscHacks::BhopMovement(CUserCmd* pCmd, Vector& OrigAng)
{

	bool CircleStrafe = GetAsyncKeyState('C') & 0x8000;

	if (!(*hackManager.pLocal()->GetFlags() & FL_ONGROUND) || (*hackManager.pLocal()->GetFlags() & FL_INWATER) && pCmd->buttons & IN_JUMP)
	{
		if (Menu::Window.MiscTab.OtherCircleStrafe.GetState() && !IsActive && CircleStrafe && !(hackManager.pLocal()->GetMoveType() == MOVETYPE_NOCLIP) || hackManager.pLocal()->GetMoveType() == MOVETYPE_LADDER)
			Start(pCmd);

		if (Menu::Window.MiscTab.OtherCircleStrafe.GetState() && IsActive)
		{
			Strafe(pCmd);
			return;
		}
		else if (IsActive)
			IsActive = false;

		if (!(*hackManager.pLocal()->GetFlags() & FL_INWATER))
			if (miscconfig.bAutoJump)
				pCmd->buttons &= ~IN_JUMP;

		if (Menu::Window.MoveTab.AutoStrafer.GetState())
		{
			if (hackManager.pLocal()->GetVelocity().Length2D() == 0 && (pCmd->forwardmove == 0 && pCmd->sidemove == 0))
			{
				pCmd->forwardmove = 450.f;
			}
		
			if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
				pCmd->sidemove = pCmd->mousedx < 0.0f ? -450.f : 450.f;
			}
			else {
				pCmd->forwardmove = 4200.0f / hackManager.pLocal()->GetVelocity().Length2D();
				pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
				if (pCmd->forwardmove > 450.f)
					pCmd->forwardmove = 450.f;
			}
		}
	}
	else if (pCmd->buttons & IN_JUMP && !IsActive)
		pCmd->forwardmove = 450.f;
}
void CMiscHacks::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
	bool flag = miscconfig.bFlagEnabled;

	int choke = miscconfig.iFlagFactor;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!hackManager.pLocal())
		return;

	if (!pWeapon)
		return;

	if (miscconfig.bFlagRandomize)
		choke = (rand() % 8) + 7;

	choke += 1;

	if (choke > 15)
		choke = 15;

	if (flag)
	{
		static int tick;
		tick++;

		int mode = miscconfig.iFlagType;

		if (miscconfig.bFlagInAir && (*hackManager.pLocal()->GetFlags() & FL_ONGROUND)) return;

		if (mode == 1) bSendPacket = flag ? !(tick % choke) : 1;
		else if (mode == 2)
		{
			static int factor = 2;
			bSendPacket = flag ? !(tick % factor + 1) : 1;
			float flVelocity = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer())->GetVelocity().Length2D() * I::Globals->interval_per_tick;
			if (bSendPacket)
				factor = clamp(static_cast<int>(std::ceil(69.f / flVelocity)), 1, choke);
		}
		else if (mode == 3)
		{
			static int factor = 7;
			static bool bSwitch = false;
			if (bSwitch)
				factor += rand() % 5 + 10;

			bSendPacket = flag ? !(tick % factor + 1) : 1;
			float flVelocity = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer())->GetVelocity().Length2D() * I::Globals->interval_per_tick;
			if (bSendPacket)
				factor = clamp(static_cast<int>(std::ceil(69.f / flVelocity)), 1, choke);
			bSwitch = !bSwitch;
		}
	}
}

void SetClanTag(const char* tag)
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(U::Memory::FindPatternVersion2(XorStr("engine.dll"), XorStr("53 56 57 8B DA 8B F9 FF 15")));
	pSetClanTag(tag, tag);
}

void CMiscHacks::PostProcessing()
{
	if (Menu::Window.MiscTab.PostProcessing.GetState())
	{
		ConVar* PostProcessing = I::CVar->FindVar("mat_postprocess_enable");
		*(float*)((DWORD)&PostProcessing->fnChangeCallback + 0xC) = NULL;
		PostProcessing->SetValueChar("0");
	}
	else
	{
		ConVar* PostProcessing = I::CVar->FindVar("mat_postprocess_enable");
		*(float*)((DWORD)&PostProcessing->fnChangeCallback + 0xC) = NULL;
		PostProcessing->SetValueChar("1");
	}
}

void ClanChanger::SetTag(float frametime)
{
	if (!miscconfig.bClanChanger)
	{
		SetClanTag("");
		return;
	}
	if (miscconfig.iClanChangerTag == 0)
	{
		SetClanTag("");
		return;
	}

	if (miscconfig.iClanChangerTag == 1)
		SuicideFlash();
	else if (miscconfig.iClanChangerTag == 2)
		SkeetTag();
	if (miscconfig.iClanChangerTag == 3)
		XenoHook();
	else if (miscconfig.iClanChangerTag == 4)
		BlackDeath();
	if (miscconfig.iClanChangerTag == 5)
		BlackFlash();

	g_ClanChangerTick += I::Globals->frametime * 1200;
}
bool ClanChanger::TickCheck(int min, int max)
{
	bool res = false;

	if (g_ClanChangerTick > min && g_ClanChangerTick < max)
		res = true;

	return res;
}
void ClanChanger::SuicideFlash()
{
	if (TickCheck(0, 200))
		SetClanTag(XorStr(""));
	else if (TickCheck(200, 400))
		SetClanTag(XorStr("suicide.cc"));

	if (g_ClanChangerTick > 400)
		g_ClanChangerTick = 0;
}
void ClanChanger::SkeetTag()
{
	if (g_ClanChangerTick > 0 && g_ClanChangerTick < 100)
		SetClanTag(XorStr("             s"));
	else if (g_ClanChangerTick > 100 && g_ClanChangerTick < 200)
		SetClanTag(XorStr("            sk"));
	else if (g_ClanChangerTick > 200 && g_ClanChangerTick < 300)
		SetClanTag(XorStr("           ske"));
	else if (g_ClanChangerTick > 300 && g_ClanChangerTick < 400)
		SetClanTag(XorStr("          skee"));
	else if (g_ClanChangerTick > 400 && g_ClanChangerTick < 500)
		SetClanTag(XorStr("         skeet"));
	else if (g_ClanChangerTick > 500 && g_ClanChangerTick < 600)
		SetClanTag(XorStr("        skeet."));
	else if (g_ClanChangerTick > 600 && g_ClanChangerTick < 700)
		SetClanTag(XorStr("       skeet.c"));
	else if (g_ClanChangerTick > 700 && g_ClanChangerTick < 800)
		SetClanTag(XorStr("      skeet.cc"));
	else if (g_ClanChangerTick > 800 && g_ClanChangerTick < 900)
		SetClanTag(XorStr("     skeet.cc "));
	else if (g_ClanChangerTick > 900 && g_ClanChangerTick < 1000)
		SetClanTag(XorStr("    skeet.cc  "));
	else if (g_ClanChangerTick > 1000 && g_ClanChangerTick < 1100)
		SetClanTag(XorStr("   skeet.cc   "));
	else if (g_ClanChangerTick > 1100 && g_ClanChangerTick < 1200)
		SetClanTag(XorStr("  skeet.cc    "));
	else if (g_ClanChangerTick > 1200 && g_ClanChangerTick < 1300)
		SetClanTag(XorStr(" skeet.cc     "));
	else if (g_ClanChangerTick > 1300 && g_ClanChangerTick < 1400)
		SetClanTag(XorStr("skeet.cc      "));
	else if (g_ClanChangerTick > 1400 && g_ClanChangerTick < 1500)
		SetClanTag(XorStr("keet.cc       "));
	else if (g_ClanChangerTick > 1500 && g_ClanChangerTick < 1600)
		SetClanTag(XorStr("eet.cc        "));
	else if (g_ClanChangerTick > 1600 && g_ClanChangerTick < 1700)
		SetClanTag(XorStr("et.cc         "));
	else if (g_ClanChangerTick > 1700 && g_ClanChangerTick < 1800)
		SetClanTag(XorStr("t.cc           "));
	else if (g_ClanChangerTick > 1800 && g_ClanChangerTick < 1900)
		SetClanTag(XorStr(".cc            "));
	else if (g_ClanChangerTick > 1900 && g_ClanChangerTick < 2000)
		SetClanTag(XorStr("cc             "));
	else if (g_ClanChangerTick > 2000 && g_ClanChangerTick < 2100)
		SetClanTag(XorStr("c              "));
	else if (g_ClanChangerTick > 2100 && g_ClanChangerTick < 2200)
		SetClanTag(XorStr("               "));
	if (g_ClanChangerTick > 2200)
		g_ClanChangerTick = 0;
}

void ClanChanger::BlackFlash()
{
	if (TickCheck(0, 200))
		SetClanTag(XorStr(""));
	else if (TickCheck(200, 400))
		SetClanTag(XorStr("BlackDeath.cc"));

	if (g_ClanChangerTick > 300)
		g_ClanChangerTick = 0;
}
void ClanChanger::BlackDeath()
{
	if (g_ClanChangerTick > 0 && g_ClanChangerTick < 100)
		SetClanTag(XorStr("              g"));
	else if (g_ClanChangerTick > 100 && g_ClanChangerTick < 200)
		SetClanTag(XorStr("             ga"));
	else if (g_ClanChangerTick > 200 && g_ClanChangerTick < 300)
		SetClanTag(XorStr("            gam"));
	else if (g_ClanChangerTick > 300 && g_ClanChangerTick < 400)
		SetClanTag(XorStr("           game"));
	else if (g_ClanChangerTick > 400 && g_ClanChangerTick < 500)
		SetClanTag(XorStr("          games"));
	else if (g_ClanChangerTick > 500 && g_ClanChangerTick < 600)
		SetClanTag(XorStr("         gamese"));
	else if (g_ClanChangerTick > 600 && g_ClanChangerTick < 700)
		SetClanTag(XorStr("        gamesen"));
	else if (g_ClanChangerTick > 700 && g_ClanChangerTick < 800)
		SetClanTag(XorStr("       gamesens"));
	else if (g_ClanChangerTick > 800 && g_ClanChangerTick < 900)
		SetClanTag(XorStr("      gamesense"));
	else if (g_ClanChangerTick > 900 && g_ClanChangerTick < 1000)
		SetClanTag(XorStr("     gamesense "));
	else if (g_ClanChangerTick > 1000 && g_ClanChangerTick < 1100)
		SetClanTag(XorStr("    gamesense  "));
	else if (g_ClanChangerTick > 1100 && g_ClanChangerTick < 1200)
		SetClanTag(XorStr("   gamesense   "));
	else if (g_ClanChangerTick > 1200 && g_ClanChangerTick < 1300)
		SetClanTag(XorStr("  gamesense    "));
	else if (g_ClanChangerTick > 1300 && g_ClanChangerTick < 1400)
		SetClanTag(XorStr(" gamesense     "));
	else if (g_ClanChangerTick > 1400 && g_ClanChangerTick < 1500)
		SetClanTag(XorStr("gamesense      "));
	else if (g_ClanChangerTick > 1500 && g_ClanChangerTick < 1600)
		SetClanTag(XorStr("amesense       "));
	else if (g_ClanChangerTick > 1600 && g_ClanChangerTick < 1700)
		SetClanTag(XorStr("mesense        "));
	else if (g_ClanChangerTick > 1700 && g_ClanChangerTick < 1800)
		SetClanTag(XorStr("esense         "));
	else if (g_ClanChangerTick > 1800 && g_ClanChangerTick < 1900)
		SetClanTag(XorStr("sense          "));
	else if (g_ClanChangerTick > 1900 && g_ClanChangerTick < 2000)
		SetClanTag(XorStr("ense           "));
	else if (g_ClanChangerTick > 2000 && g_ClanChangerTick < 2100)
		SetClanTag(XorStr("nse            "));
	else if (g_ClanChangerTick > 2100 && g_ClanChangerTick < 2200)
		SetClanTag(XorStr("se              "));
	else if (g_ClanChangerTick > 2200 && g_ClanChangerTick < 2300)
		SetClanTag(XorStr("e               "));
	else if (g_ClanChangerTick > 2300 && g_ClanChangerTick < 2400)
		SetClanTag(XorStr("                "));
	if (g_ClanChangerTick > 2500)
		g_ClanChangerTick = 0;
}

void ClanChanger::XenoHook()
{
	if (g_ClanChangerTick > 0 && g_ClanChangerTick < 100)
		SetClanTag(XorStr("             b"));
	else if (g_ClanChangerTick > 100 && g_ClanChangerTick < 200)
		SetClanTag(XorStr("            ba"));
	else if (g_ClanChangerTick > 200 && g_ClanChangerTick < 300)
		SetClanTag(XorStr("           bad"));
	else if (g_ClanChangerTick > 300 && g_ClanChangerTick < 400)
		SetClanTag(XorStr("          badc"));
	else if (g_ClanChangerTick > 400 && g_ClanChangerTick < 500)
		SetClanTag(XorStr("         badca"));
	else if (g_ClanChangerTick > 500 && g_ClanChangerTick < 600)
		SetClanTag(XorStr("        badcac"));
	else if (g_ClanChangerTick > 600 && g_ClanChangerTick < 700)
		SetClanTag(XorStr("       badcache"));
	else if (g_ClanChangerTick > 700 && g_ClanChangerTick < 800)
		SetClanTag(XorStr("      badcache."));
	else if (g_ClanChangerTick > 800 && g_ClanChangerTick < 900)
		SetClanTag(XorStr("     badcache.n "));
	else if (g_ClanChangerTick > 900 && g_ClanChangerTick < 1000)
		SetClanTag(XorStr("     badcache.ne"));
	else if (g_ClanChangerTick > 1000 && g_ClanChangerTick < 1100)
		SetClanTag(XorStr("    badcache.net"));
	else if (g_ClanChangerTick > 1100 && g_ClanChangerTick < 1200)
		SetClanTag(XorStr("   badcache.net "));
	else if (g_ClanChangerTick > 1200 && g_ClanChangerTick < 1300)
		SetClanTag(XorStr("  badcache.net  "));
	else if (g_ClanChangerTick > 1300 && g_ClanChangerTick < 1400)
		SetClanTag(XorStr("badcache.net    "));
	else if (g_ClanChangerTick > 1400 && g_ClanChangerTick < 1500)
		SetClanTag(XorStr("adcache.net     "));
	else if (g_ClanChangerTick > 1500 && g_ClanChangerTick < 1600)
		SetClanTag(XorStr("dcache.net      "));
	else if (g_ClanChangerTick > 1600 && g_ClanChangerTick < 1700)
		SetClanTag(XorStr("cache.net       "));
	else if (g_ClanChangerTick > 1700 && g_ClanChangerTick < 1800)
		SetClanTag(XorStr("ache.net        "));
	else if (g_ClanChangerTick > 1800 && g_ClanChangerTick < 1900)
		SetClanTag(XorStr("che.net         "));
	else if (g_ClanChangerTick > 1900 && g_ClanChangerTick < 2000)
		SetClanTag(XorStr("he.net          "));
	else if (g_ClanChangerTick > 2000 && g_ClanChangerTick < 2100)
		SetClanTag(XorStr("e.net           "));
	else if (g_ClanChangerTick > 2100 && g_ClanChangerTick < 2200)
		SetClanTag(XorStr("               "));
	if (g_ClanChangerTick > 2200)
		g_ClanChangerTick = 0;
}

void NameChanger::SetName(const char* name)
{
	ConVar* nameConvar = I::CVar->FindVar(XorStr("name"));
	*(int*)((DWORD)&nameConvar->fnChangeCallback + 0xC) = NULL;
	nameConvar->SetValueChar(name);
}
void NameChanger::Manage()
{
	if (!miscconfig.bNameChanger) return;
	if (miscconfig.iNameChangerType == 0) return;

	if (miscconfig.iNameChangerType == 1)
		Suicide();
	else if (miscconfig.iNameChangerType == 2)
		BlackDeath();
	else if (miscconfig.iNameChangerType == 3)
		XenoHook();
	else if (miscconfig.iNameChangerType == 4)
		AcidTrip();
}
void NameChanger::Suicide()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.1)
		return;

	if (flip)
		SetName(XorStr(u8"�suicide.cc"));
	else
		SetName(XorStr(u8"suicide.cc�"));

	flip = !flip;

	start_t = clock();
}

void NameChanger::BlackDeath()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.1)
		return;

	if (flip)
		SetName(XorStr(u8"�blackdeath.cc"));
	else
		SetName(XorStr(u8"blackdeath.cc�"));
	flip = !flip;

	start_t = clock();
}

void NameChanger::XenoHook()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.1)
		return;

	if (flip)
		SetName(XorStr(u8"�XenoHook.xyz"));
	else
		SetName(XorStr(u8"XenoHook.xyz�"));
	flip = !flip;

	start_t = clock();
}

void NameChanger::AcidTrip()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.1)
		return;

	if (flip)
		SetName(XorStr(u8"�AcidTrip.xyz"));
	else
		SetName(XorStr(u8"AcidTrip.xyz�"));
	flip = !flip;

	start_t = clock();
}
/*void NameChanger::Interwebz()
{
static clock_t start_t = clock();
double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
if (timeSoFar < 0.1)
return;

if (flip)
SetName(XorStr("-INTERWEBZ"));
else
SetName(XorStr("INTERWEBZ-"));
flip = !flip;

start_t = clock();
}*/
/*void NameChanger::AyywareCrasher()
{
static clock_t start_t = clock();
double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
if (timeSoFar < 0.1)
return;

SetName(XorStr(u8"��������������������������������"));

flip = !flip;

start_t = clock();
}*/

void ChatSpammer::Manage()
{
	if (!miscconfig.bChatSpammer) return;
	if (miscconfig.iChatSpammerType == 0) return;

	if (miscconfig.iChatSpammerType == 1)
		BlackDeath();
	else if (miscconfig.iChatSpammerType == 2)
		DeathJokes();
	else if (miscconfig.iChatSpammerType == 3)
		Racist();
}
void ChatSpammer::BlackDeath()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (miscconfig.iChatSpammerSpeed == 2)
	{
		if (timeSoFar < 0.7)
			return;
	}
	else if (miscconfig.iChatSpammerSpeed == 1)
	{
		if (timeSoFar < 1)
			return;
	}
	else
	{
		if (timeSoFar < 1.5)
			return;
	}

	SayInChat("BlackDeath.cc - Makeing your Child Drop Dead");

	start_t = clock();
}

void ChatSpammer::DeathJokes()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (miscconfig.iChatSpammerSpeed == 2)
	{
		if (timeSoFar < 0.7)
			return;
	}
	else if (miscconfig.iChatSpammerSpeed == 1)
	{
		if (timeSoFar < 1)
			return;
	}
	else
	{
		if (timeSoFar < 1.5)
			return;
	}

	std::vector< std::string > msgs;

	msgs.push_back("My life is an open book. But it's very poorly written and I die in the end.");
	int randomIndex = rand() % msgs.size();
	SayInChat(msgs[randomIndex].c_str());


	start_t = clock();
}


void ChatSpammer::Racist()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (miscconfig.iChatSpammerSpeed == 2)
	{
		if (timeSoFar < 0.7)
			return;
	}
	else if (miscconfig.iChatSpammerSpeed == 1)
	{
		if (timeSoFar < 1)
			return;
	}
	else
	{
		if (timeSoFar < 1.5)
			return;
	}

	std::vector< std::string > msgs;

	msgs.push_back("What did God say when he made the first black man ? \"Damn, I burnt one.\"");


	int randomIndex = rand() % msgs.size();
	SayInChat(msgs[randomIndex].c_str());


	start_t = clock();
}

void RadioSpammer::Manage()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 1)
		return;

	if (miscconfig.bRadioSpammer)
	{
		const char* radioCommands[] = {
			"coverme",
			"takepoint",
			"holdpos",
			"regroup",
			"followme",
			"takingfire",
			"go",
			"fallback",
			"sticktog",
			"report",
			"roger",
			"enemyspot",
			"needbackup",
			"sectorclear",
			"inposition",
			"reportingin",
			"getout",
			"negative",
			"enemydown",
		};

		I::Engine->ClientCmd_Unrestricted(radioCommands[rand() % std::size(radioCommands)]);
	}

	start_t = clock();
}

/*void ChatSpammer::Interwebz()
{
static clock_t start_t = clock();
double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
if (miscconfig.iChatSpammerSpeed == 2)
{
if (timeSoFar < 0.7)
return;
}
else if (miscconfig.iChatSpammerSpeed == 1)
{
if (timeSoFar < 1)
return;
}
else
{
if (timeSoFar < 1.5)
return;
}

SayInChat("Interwebz.cc owns me and all");

start_t = clock();
}*/

void KillSpammer::Manage()
{

	if (miscconfig.bKillMessage && globals.killspammer.killedPlayerQueue.size() > 0)
	{
		static clock_t start_t = clock();
		double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		if (timeSoFar < 0.7)
			return;

		player_info_t pinfo;
		I::Engine->GetPlayerInfo(globals.killspammer.killedPlayerQueue[0], &pinfo);

		const char* dead_player_name = pinfo.name;

		if (miscconfig.iKillMessageType == 0)
		{
			I::Engine->ClientCmd_Unrestricted(XorStr("say \"uff jaa 1\""));
		}
		else if (miscconfig.iKillMessageType == 1)
		{
			std::vector<std::string> msgs;
			char msg[128];
			msgs.push_back(msg);
			if (miscconfig.iKillMessageType == 2)
			{
				I::Engine->ClientCmd_Unrestricted(XorStr("say \"f12 cya @ media\""));
			}

			char buffer[250];
			int randomIndex = rand() % msgs.size();
			sprintf_s(buffer, XorStr("say \"%s\""), msgs[randomIndex].c_str());
			I::Engine->ClientCmd_Unrestricted(buffer);
		}

		globals.killspammer.killedPlayerQueue.erase(globals.killspammer.killedPlayerQueue.begin(), globals.killspammer.killedPlayerQueue.begin() + 1);

		start_t = clock();
	}
}