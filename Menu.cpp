#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" // for the unload meme
#include "Interfaces.h"
#include "Globals.h"
#include "CRC32.h"
#include "XorStr.hpp"
#include "Skinchanger.h"


#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 344



//     SetSize(950, 557); 

std::map<DWORD, PlayerListItem_t> PlayerList;


SuicideWindow Menu::Window;

void SaveCallback()
{
	switch (Menu::Window.MiscTab.Configs.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "1.cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "2.cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "3.cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "4.cfg");
		break;
	case 4:
		GUI.SaveWindowState(&Menu::Window, "5.cfg");
		break;
	case 5:
		GUI.SaveWindowState(&Menu::Window, "6.cfg");
		break;
	}
}

void LoadCallback()
{
	switch (Menu::Window.MiscTab.Configs.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "1.cfg");
		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "2.cfg");
		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "3.cfg");
		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "4.cfg");
		break;
	case 4:
		GUI.LoadWindowState(&Menu::Window, "5.cfg");
		break;
	case 5:
		GUI.LoadWindowState(&Menu::Window, "6.cfg");
		break;
	}
}

void UnLoadCallbk()
{
	DoUnload = true;
}


void KnifeApplyCallbk()
{
	static ConVar* Meme = I::CVar->FindVar(XorStr("cl_fullupdate"));
	Meme->nFlags &= ~FCVAR_CHEAT;
	I::Engine->ClientCmd_Unrestricted(XorStr("cl_fullupdate"));
}

void SuicideWindow::Setup()
{
	SetPosition(100, 49);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle(XorStr("csgo hvh hack - Jun 5 2018"));
	SetTitle2(XorStr("copyright (c) ayywa.re (dont hex edit)"));
	RegisterTab(&RageBotTab);
	RegisterTab(&RageBotTab4);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&MoveTab);
	RegisterTab(&MiscTab2);
	RegisterTab(&ColorsTab);
	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	RageBotTab4.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	MoveTab.Setup();
	MiscTab2.Setup();
	ColorsTab.Setup();
}

//============================================r_1-4 setup========================================================

void CMainRageTab::Setup()
{
	SetTitle(XorStr("r_main"));

	//Aimbot
	AimbotGroup.SetText(XorStr(""));
	AimbotGroup.SetSize(292, 335);
	AimbotGroup.SetPosition(86, -13);
	AimbotGroup.AddTab(CGroupTab("main", 1));
	AimbotGroup.AddTab(CGroupTab("position", 2));
	AimbotGroup.AddTab(CGroupTab("target", 3));
	AimbotGroup.AddTab(CGroupTab("accuracy", 4));
	RegisterControl(&AimbotGroup);

	Enabled.SetFileId(XorStr("r_enabled"));
	AimbotGroup.PlaceLabledControl3(1, XorStr("activate"), this, &Enabled);

	SilentAim.SetFileId(XorStr("r_silentaim"));
	SilentAim.SetAsSub(true);
	AimbotGroup.PlaceLabledControl3(1, XorStr("silent aim"), this, &SilentAim);

	AutoFire.SetFileId(XorStr("r_autofire"));
	AimbotGroup.PlaceLabledControl3(1, XorStr("auto fire"), this, &AutoFire);

	MultiPoint.SetFileId(XorStr("r_extraplayer_scans"));
	AimbotGroup.PlaceLabledControl3(1, XorStr("extra player scans"), this, &MultiPoint);

	MultipointType.SetFileId(XorStr("r_maxplayer_scans"));
	MultipointType.SetBoundaries(0, 25);
	MultipointType.SetValue(0);
	AimbotGroup.PlaceLabledControl3(1, XorStr("max players"), this, &MultipointType);




	AutoFireTarget.SetFileId(XorStr("r_priority_hitbox"));
	AutoFireTarget.AddItem(XorStr("head"));
	AutoFireTarget.AddItem(XorStr("neck"));
	AutoFireTarget.AddItem(XorStr("chest"));
	AutoFireTarget.AddItem(XorStr("stomach"));
	AutoFireTarget.AddItem(XorStr("pelvis"));
	AimbotGroup.PlaceLabledControl3(2, XorStr("aim hitbox"), this, &AutoFireTarget);

	HitscanBones.SetFileId(XorStr("r_hitscan"));
	HitscanBones.items.push_back(MultiBoxItem(false, XorStr("head")));
	HitscanBones.items.push_back(MultiBoxItem(false, XorStr("neck")));
	HitscanBones.items.push_back(MultiBoxItem(false, XorStr("chest")));
	HitscanBones.items.push_back(MultiBoxItem(false, XorStr("stomach")));
	HitscanBones.items.push_back(MultiBoxItem(false, XorStr("left arm")));
	HitscanBones.items.push_back(MultiBoxItem(false, XorStr("right arm")));
	HitscanBones.items.push_back(MultiBoxItem(false, XorStr("left leg")));
	HitscanBones.items.push_back(MultiBoxItem(false, XorStr("right leg")));
	AimbotGroup.PlaceLabledControl3(2, XorStr(""), this, &HitscanBones);




	AutoWall.SetFileId(XorStr("r_autowall"));
	AimbotGroup.PlaceLabledControl3(3, XorStr("autowall"), this, &AutoWall);

	MinimumDamage.SetFileId(XorStr("r_mindmg"));
	MinimumDamage.SetPosition(420, 371);
	MinimumDamage.SetBoundaries(1, 100);
	MinimumDamage.SetValue(0);
	MinimumDamage.SetAsAuto(true);
	MinimumDamage.SetFormat(FORMAT_INT);
	AimbotGroup.PlaceLabledControl3(3, XorStr("minimum damage"), this, &MinimumDamage);

	AutomaticScope.SetFileId(XorStr("r_autoscope"));
	AimbotGroup.PlaceLabledControl3(3, XorStr("auto scope"), this, &AutomaticScope);

	bInterpLagComp.SetFileId(XorStr("r_history_aim"));
	bInterpLagComp.SetPosition(420, 371);
	AimbotGroup.PlaceLabledControl3(3, XorStr("history aim"), this, &bInterpLagComp);

	TargetSelection.SetFileId(XorStr("r_selection"));
	TargetSelection.AddItem(XorStr("best fov"));
	TargetSelection.AddItem(XorStr("health"));
	TargetSelection.AddItem(XorStr("best distance"));
	TargetSelection.AddItem(XorStr("threat"));;
	TargetSelection.AddItem(XorStr("first valid"));
	AimbotGroup.PlaceLabledControl3(3, XorStr("selection"), this, &TargetSelection);




	ResolverMode.SetFileId(XorStr("r_resolvermode"));
	ResolverMode.AddItem("disabled");
	ResolverMode.AddItem("default");
	ResolverMode.AddItem("experimental");
	AimbotGroup.PlaceLabledControl3(4, XorStr("resolver"), this, &ResolverMode);

	//Hitchance
	SyncHitChance.SetFileId(XorStr("w_enablesettings"));
	AimbotGroup.PlaceLabledControl3(4, XorStr("enable weapon hitchance"), this, &SyncHitChance);

	WeaponPistolHitchance.SetFileId(XorStr("w_pistolhitchance"));
	WeaponPistolHitchance.SetBoundaries(0, 100);
	WeaponPistolHitchance.SetValue(0);
	WeaponPistolHitchance.SetFormat(FORMAT_INT);
	AimbotGroup.PlaceLabledControl3(4, XorStr("pistol"), this, &WeaponPistolHitchance);

	WeaponSniperHitchance.SetFileId(XorStr("w_sniperhitchance"));
	WeaponSniperHitchance.SetBoundaries(0, 100);
	WeaponSniperHitchance.SetValue(0);
	WeaponSniperHitchance.SetFormat(FORMAT_INT);
	AimbotGroup.PlaceLabledControl3(4, XorStr("sniper"), this, &WeaponSniperHitchance);

	WeaponRifleHitchance.SetFileId(XorStr("w_riflehitchance"));
	WeaponRifleHitchance.SetBoundaries(0, 100);
	WeaponRifleHitchance.SetValue(0);
	WeaponRifleHitchance.SetFormat(FORMAT_INT);
	AimbotGroup.PlaceLabledControl3(4, XorStr("rifle"), this, &WeaponRifleHitchance);

	WeaponSMGHitchance.SetFileId(XorStr("w_smghitchance"));
	WeaponSMGHitchance.SetBoundaries(0, 100);
	WeaponSMGHitchance.SetValue(0);
	WeaponSMGHitchance.SetFormat(FORMAT_INT);
	AimbotGroup.PlaceLabledControl3(4, XorStr("subs"), this, &WeaponSMGHitchance);

	WeaponOtherHitchance.SetFileId(XorStr("w_otherhitchance"));
	WeaponOtherHitchance.SetBoundaries(0, 100);
	WeaponOtherHitchance.SetValue(0);
	WeaponOtherHitchance.SetFormat(FORMAT_INT);
	AimbotGroup.PlaceLabledControl3(4, XorStr("zeus x27 | heavy"), this, &WeaponOtherHitchance);
}

void CMiscRageTab::Setup()
{
	SetTitle(XorStr("r_misc"));

	//Anti Aim
	DefaultAntiAimGroup.SetText(XorStr(""));
	DefaultAntiAimGroup.SetSize(292, 335);
	DefaultAntiAimGroup.SetPosition(86, -13);
	DefaultAntiAimGroup.AddTab(CGroupTab("antiaim", 1));
	DefaultAntiAimGroup.AddTab(CGroupTab("antiaim2", 2));
	DefaultAntiAimGroup.AddTab(CGroupTab("misc", 3));
	RegisterControl(&DefaultAntiAimGroup);

	AntiAimPitch.SetFileId("pitch");
	AntiAimPitch.AddItem("disabled");
	AntiAimPitch.AddItem("emotion");
	AntiAimPitch.AddItem("down");
	AntiAimPitch.AddItem("up");
	DefaultAntiAimGroup.PlaceLabledControl3(1, XorStr("pitch"), this, &AntiAimPitch);

	AntiAimStandingYaw.SetFileId("r_yaw");
	AntiAimStandingYaw.AddItem(XorStr("disabled"));
	AntiAimStandingYaw.AddItem(XorStr("backwards"));
	AntiAimStandingYaw.AddItem(XorStr("forwards"));
	AntiAimStandingYaw.AddItem(XorStr("left"));
	AntiAimStandingYaw.AddItem(XorStr("right"));
	AntiAimStandingYaw.AddItem(XorStr("spin"));
	AntiAimStandingYaw.AddItem(XorStr("backwards jitter"));
	AntiAimStandingYaw.AddItem(XorStr("forwards jitter"));
	AntiAimStandingYaw.AddItem(XorStr("left jitter"));
	AntiAimStandingYaw.AddItem(XorStr("right jitter"));
	AntiAimStandingYaw.AddItem(XorStr("freestanding"));
	DefaultAntiAimGroup.PlaceLabledControl3(1, XorStr("yaw"), this, &AntiAimStandingYaw);

	AntiAimMovingYaw.SetFileId("r_move_yaw");
	AntiAimMovingYaw.AddItem(XorStr("disabled"));
	AntiAimMovingYaw.AddItem(XorStr("backwards"));
	AntiAimMovingYaw.AddItem(XorStr("forwards"));
	AntiAimMovingYaw.AddItem(XorStr("left"));
	AntiAimMovingYaw.AddItem(XorStr("right"));
	AntiAimMovingYaw.AddItem(XorStr("spin"));
	AntiAimMovingYaw.AddItem(XorStr("backwards jitter"));
	AntiAimMovingYaw.AddItem(XorStr("forwards jitter"));
	AntiAimMovingYaw.AddItem(XorStr("left jitter"));
	AntiAimMovingYaw.AddItem(XorStr("right jitter"));
	AntiAimMovingYaw.AddItem(XorStr("freestanding"));
	DefaultAntiAimGroup.PlaceLabledControl3(1, XorStr("move yaw"), this, &AntiAimMovingYaw);

	AntiAimFakeYaw.SetFileId("r_fake_yaw");
	AntiAimFakeYaw.AddItem(XorStr("disabled"));
	AntiAimFakeYaw.AddItem(XorStr("backwards"));
	AntiAimFakeYaw.AddItem(XorStr("forwards"));
	AntiAimFakeYaw.AddItem(XorStr("left"));
	AntiAimFakeYaw.AddItem(XorStr("right"));
	AntiAimFakeYaw.AddItem(XorStr("spin"));
	AntiAimFakeYaw.AddItem(XorStr("backwards jitter"));
	AntiAimFakeYaw.AddItem(XorStr("forwards jitter"));
	AntiAimFakeYaw.AddItem(XorStr("left jitter"));
	AntiAimFakeYaw.AddItem(XorStr("right jitter"));
	AntiAimFakeYaw.AddItem(XorStr("freestanding"));
	DefaultAntiAimGroup.PlaceLabledControl3(1, XorStr("fake yaw"), this, &AntiAimFakeYaw);

	AntiAimKnife.SetFileId(XorStr("r_aa_knife_nade"));
	AntiAimKnife.SetPosition(420, 410);
	DefaultAntiAimGroup.PlaceLabledControl3(1, XorStr("anti aim on knife and nades"), this, &AntiAimKnife);

	AntiAimTarget.SetFileId(XorStr("r_aa_at_target"));
	AntiAimTarget.SetPosition(420, 391);
	DefaultAntiAimGroup.PlaceLabledControl3(1, XorStr("at targets"), this, &AntiAimTarget);



	RemoveRecoil.SetFileId(XorStr("r_no_recoil"));
	DefaultAntiAimGroup.PlaceLabledControl3(3, XorStr("no recoil"), this, &RemoveRecoil);

	NoVisualRecoil.SetFileId(XorStr("v_novisualrecoil"));
	DefaultAntiAimGroup.PlaceLabledControl3(3, XorStr("no view offsets"), this, &NoVisualRecoil);

	PSilentAim.SetFileId(XorStr("r_perfect_silentaim"));
	DefaultAntiAimGroup.PlaceLabledControl3(3, XorStr("psilent aim"), this, &PSilentAim);
}

void CLegitBotTab::Setup()
{
	SetTitle("LegitAim");

#pragma region Aimbot
	AimbotGroup.SetPosition(109, -4);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(210, 170);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("auto fire", this, &AimbotAutoFire);

	AimbotFriendlyFire.SetFileId("aim_friendfire");
	AimbotGroup.PlaceLabledControl("Friendly fire", this, &AimbotFriendlyFire);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On key", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("", this, &AimbotKeyBind);

	AimbotAutoPistol.SetFileId("aim_apistol");
	AimbotGroup.PlaceLabledControl("auto pistol", this, &AimbotAutoPistol);

#pragma endregion Aimbot shit

#pragma region Triggerbot
	TriggerGroup.SetPosition(554, -4);
	TriggerGroup.SetText("Triggerbot");
	TriggerGroup.SetSize(99, 465);
	RegisterControl(&TriggerGroup);

	TriggerEnable.SetFileId("trig_enable");
	TriggerGroup.Control2(this, &TriggerEnable, "Enable");

	TriggerKeyPress.SetFileId("trig_onkey");
	TriggerGroup.Control2(this, &TriggerKeyPress, "On Key");

	TriggerKeyBind.SetFileId("trig_key");
	TriggerGroup.Control2(this, &TriggerKeyBind, "Key Bind");

	TriggerDelay.SetFileId("trig_time");
	TriggerDelay.SetBoundaries(1.f, 1000.f);
	TriggerDelay.SetValue(1.f);
	TriggerGroup.Control2(this, &TriggerDelay, "Delay");
#pragma endregion Triggerbot stuff

#pragma region Main Weapon
	WeaponMainGroup.SetPosition(109, 256);
	WeaponMainGroup.SetText("Rifles");
	WeaponMainGroup.SetSize(210, 210);
	RegisterControl(&WeaponMainGroup);

	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.1f, 100.f);
	WeaponMainSpeed.SetValue(1.0f);
	WeaponMainGroup.PlaceLabledControl(XorStr("Strength"), this, &WeaponMainSpeed);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.f, 30.f);
	WeaponMainFoV.SetValue(5.f);
	WeaponMainGroup.PlaceLabledControl(XorStr("Field of view"), this, &WeaponMainFoV);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainGroup.PlaceLabledControl(XorStr("Enable RCS"), this, &WeaponMainRecoil);

	WeaponMainRecoilAmountX.SetFileId("main_rcspitch");
	WeaponMainRecoilAmountX.SetBoundaries(0.0f, 100.f);
	WeaponMainRecoilAmountX.SetValue(0.0f);
	WeaponMainGroup.PlaceLabledControl(XorStr("RCS Pitch"), this, &WeaponMainRecoilAmountX);

	WeaponMainRecoilAmountY.SetFileId("legit_main_rcsyaw");
	WeaponMainRecoilAmountY.SetBoundaries(0.0f, 100.f);
	WeaponMainRecoilAmountY.SetValue(0.0f);
	WeaponMainGroup.PlaceLabledControl(XorStr("RCS Yaw"), this, &WeaponMainRecoilAmountY);


	WeaponMainPSilent.SetFileId("main_psilent");
	WeaponMainGroup.PlaceLabledControl(XorStr("Perfect silent"), this, &WeaponMainPSilent);


	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem(XorStr("Head"));
	WeaponMainHitbox.AddItem(XorStr("Neck"));
	WeaponMainHitbox.AddItem(XorStr("Chest"));
	WeaponMainHitbox.AddItem(XorStr("Stomach"));
	WeaponMainHitbox.AddItem(XorStr("Multi point"));
	WeaponMainGroup.Control2(this, &WeaponMainHitbox, "Aim point");
#pragma endregion

#pragma region Pistols
	WeaponPistGroup.SetPosition(335, 256);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(210, 210);
	RegisterControl(&WeaponPistGroup);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.1f, 100.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponPistGroup.PlaceLabledControl("Strength", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.1f, 30.f);
	WeaponPistFoV.SetValue(0.78f);
	WeaponPistGroup.PlaceLabledControl("Field of view", this, &WeaponPistFoV);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistGroup.PlaceLabledControl("Enable RCS", this, &WeaponPistRecoil);

	WeaponPistRecoilAmountX.SetFileId("legit_pistol_rcspitch");
	WeaponPistRecoilAmountX.SetBoundaries(0.f, 100.f);
	WeaponPistRecoilAmountX.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl(XorStr("RCS Pitch"), this, &WeaponPistRecoilAmountX);

	WeaponPistRecoilAmountY.SetFileId("legit_pistol_rcsyaw");
	WeaponPistRecoilAmountY.SetBoundaries(0.f, 100.f);
	WeaponPistRecoilAmountY.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl(XorStr("RCS Yaw"), this, &WeaponPistRecoilAmountY);


	WeaponPistPSilent.SetFileId("pist_psilent");
	WeaponPistGroup.PlaceLabledControl(XorStr("Perfect silent"), this, &WeaponPistPSilent);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem(XorStr("Head"));
	WeaponPistHitbox.AddItem(XorStr("Neck"));
	WeaponPistHitbox.AddItem(XorStr("Chest"));
	WeaponPistHitbox.AddItem(XorStr("Stomach"));
	WeaponPistHitbox.AddItem(XorStr("Multi point"));
	WeaponPistGroup.Control2(this, &WeaponPistHitbox, "Aim point");
#pragma endregion

#pragma region Snipers
	WeaponSnipGroup.SetPosition(335, -4);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(210, 170);
	RegisterControl(&WeaponSnipGroup);

	WeaponSnipSpeed.SetFileId("wconf_speed_ak");
	WeaponSnipSpeed.SetBoundaries(0.1f, 100.f);
	WeaponSnipSpeed.SetValue(1.0f);
	WeaponSnipGroup.PlaceLabledControl("Strength", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("wconf_fov_ak");
	WeaponSnipFoV.SetBoundaries(0.1f, 30.f);
	WeaponSnipFoV.SetValue(5.f);
	WeaponSnipGroup.PlaceLabledControl("Field of view", this, &WeaponSnipFoV);

	WeaponSnipRecoil.SetFileId("wconf_recoil_ak");
	WeaponSnipGroup.PlaceLabledControl("Enable RCS", this, &WeaponSnipRecoil);

	WeaponSnipRecoilAmountX.SetFileId("legit_sniper_rcspitch");
	WeaponSnipRecoilAmountX.SetBoundaries(0.f, 100.f);
	WeaponSnipRecoilAmountX.SetValue(0.f);
	WeaponSnipGroup.PlaceLabledControl(XorStr("RCS Pitch"), this, &WeaponSnipRecoilAmountX);

	WeaponSnipRecoilAmountY.SetFileId("legit_sniper_rcsyaw");
	WeaponSnipRecoilAmountY.SetBoundaries(0.f, 100.f);
	WeaponSnipRecoilAmountY.SetValue(0.f);
	WeaponSnipGroup.PlaceLabledControl(XorStr("RCS Yaw"), this, &WeaponSnipRecoilAmountY);

	WeaponSnipPSilent.SetFileId("wconf_psilent_ak");
	WeaponSnipGroup.PlaceLabledControl(XorStr("Perfect silent"), this, &WeaponSnipPSilent);

	WeaponSnipHitbox.SetFileId("wconf_hitbox_ak");
	WeaponSnipHitbox.AddItem(XorStr("Head"));
	WeaponSnipHitbox.AddItem(XorStr("Neck"));
	WeaponSnipHitbox.AddItem(XorStr("Chest"));
	WeaponSnipHitbox.AddItem(XorStr("Stomach"));
	WeaponSnipHitbox.AddItem(XorStr("Multi point"));
	WeaponSnipGroup.PlaceLabledControl(XorStr("Aim Point"), this, &WeaponSnipHitbox);

#pragma endregion

#pragma More
	AimMoreGroup.SetPosition(109, 182);
	AimMoreGroup.SetText("More");
	AimMoreGroup.SetSize(436, 68);
	RegisterControl(&AimMoreGroup);

	HitboxIndexes.SetFileId("legit_hitbox_indexes");
	HitboxIndexes.SetBoundaries(1, 10);
	HitboxIndexes.SetValue(1);
	AimMoreGroup.PlaceLabledControl(XorStr("Indexes"), this, &HitboxIndexes);

	Smokecheck.SetFileId("legit_misc_smokecheck");
	AimMoreGroup.PlaceLabledControl(XorStr("Smoke check"), this, &Smokecheck);

#pragma endregion
}


void CVisualTab::Setup()
{
	SetTitle(XorStr("visual"));

	//ESP
	EspGroup.SetText(XorStr(""));
	EspGroup.SetSize(292, 335);
	EspGroup.SetPosition(86, -13);
	EspGroup.AddTab(CGroupTab("player", 1));
	EspGroup.AddTab(CGroupTab("entity", 2));
	EspGroup.AddTab(CGroupTab("misc", 3));
	EspGroup.AddTab(CGroupTab("render", 4));
	EspGroup.AddTab(CGroupTab("color", 5));
	RegisterControl(&EspGroup);

	EnemyEspOnly.SetFileId(XorStr("v_draw_team"));
	EspGroup.PlaceLabledControl3(1, XorStr("draw team"), this, &EnemyEspOnly);

	ShowPlayerNames.SetFileId(XorStr("v_name_text"));
	EspGroup.PlaceLabledControl3(1, XorStr("name text"), this, &ShowPlayerNames);

	ShowHealth.SetFileId(XorStr("v_health_text"));
	EspGroup.PlaceLabledControl3(1, XorStr("health text"), this, &ShowHealth);

	OptionsWeapon.SetFileId(XorStr("v_weapon_text"));
	EspGroup.PlaceLabledControl3(1, XorStr("weapon text"), this, &OptionsWeapon);

	EspMode.SetFileId(XorStr("v_espmode"));
	EspMode.AddItem(XorStr("None"));
	EspMode.AddItem(XorStr("bounding"));
	EspMode.AddItem(XorStr("corner"));
	EspGroup.PlaceLabledControl3(1, XorStr("box"), this, &EspMode);

	HealthBar.SetFileId(XorStr("v_health_bar"));
	EspGroup.PlaceLabledControl3(1, XorStr("health bar"), this, &HealthBar);

	FovChangerAmt.SetFileId(XorStr("v_fovchangeramt"));
	FovChangerAmt.SetBoundaries(0, 120);
	FovChangerAmt.SetValue(0);
	EspGroup.PlaceLabledControl3(3, XorStr("fov"), this, &FovChangerAmt);


	OtherNoFlash.SetFileId("v_noflash");
	EspGroup.PlaceLabledControl3(4, "no flash", this, &OtherNoFlash);

	OtherNoSmoke.SetFileId("v_nosmoke");
	EspGroup.PlaceLabledControl3(4, "no smoke", this, &OtherNoSmoke);
}

void CMoveTab::Setup()
{
	SetTitle(XorStr("move"));

	MainGroup.SetText(XorStr(""));
	MainGroup.SetSize(292, 335);
	MainGroup.SetPosition(86, -13);
	MainGroup.AddTab(CGroupTab("strafer", 1));
	MainGroup.AddTab(CGroupTab("simulation", 2));
	RegisterControl(&MainGroup);

	AutoStrafer.SetFileId(XorStr("m_autostrafe"));
	MainGroup.PlaceLabledControl3(1, XorStr("speed strafer"), this, &AutoStrafer);
}

void CMiscTab::Setup()
{
	SetTitle(XorStr("misc"));

	MainGroup.SetText(XorStr(""));
	MainGroup.SetSize(292, 335);
	MainGroup.SetPosition(86, -13);
	MainGroup.AddTab(CGroupTab("main", 1));
	MainGroup.AddTab(CGroupTab("game", 2));
	MainGroup.AddTab(CGroupTab("game2", 3));
	MainGroup.AddTab(CGroupTab("players", 4));
	RegisterControl(&MainGroup);

	BunnyHop.SetFileId(XorStr("m_autohop"));
	MainGroup.PlaceLabledControl3(1, XorStr("auto hop"), this, &BunnyHop);

	ThirdPerson.SetFileId(XorStr("m_thirdperson"));
	MainGroup.PlaceLabledControl3(2, XorStr("enable thirdperson [v]"), this, &ThirdPerson);

	//Thirdpersonkey.SetFileId("m_thirdperson_key");
	//MainGroup.PlaceLabledControl3(2, XorStr("thirdperson key"), this, &Thirdpersonkey);

	ThirdPersonType.SetFileId(XorStr("m_thirdperson_type"));
	ThirdPersonType.AddItem(XorStr("fake"));
	ThirdPersonType.AddItem(XorStr("real"));
	MainGroup.PlaceLabledControl3(2, XorStr("thirdperson type"), this, &ThirdPersonType);

	Menu_Red.SetFileId(XorStr("m_color_menured"));
	Menu_Red.SetBoundaries(0, 255);
	Menu_Red.SetValue(255);
	MainGroup.PlaceLabledControl3(3, XorStr("red"), this, &Menu_Red);

	Menu_Green.SetFileId(XorStr("m_color_menugreen"));
	Menu_Green.SetBoundaries(0, 255);
	Menu_Green.SetValue(165);
	MainGroup.PlaceLabledControl3(3, XorStr("green"), this, &Menu_Green);

	Menu_Blue.SetFileId(XorStr("m_color_menublue"));
	Menu_Blue.SetBoundaries(0, 255);
	Menu_Blue.SetValue(0);
	MainGroup.PlaceLabledControl3(3, XorStr("blue"), this, &Menu_Blue);
}

void CMiscTab2::Setup()
{
	SetTitle(XorStr("m_cfg"));

	MainGroup.SetText(XorStr(""));
	MainGroup.SetSize(292, 335);
	MainGroup.SetPosition(86, -13);
	RegisterControl(&MainGroup);

	SaveConfig.SetText(XorStr("save"));
	SaveConfig.SetCallback(&SaveCallback);
	SaveConfig.SetPosition(292, 243);
	SaveConfig.SetSize(85, 16);
	RegisterControl(&SaveConfig);

	LoadConfig.SetText(XorStr("load"));
	LoadConfig.SetCallback(&LoadCallback);
	LoadConfig.SetPosition(192, 243);
	LoadConfig.SetSize(85, 16);
	RegisterControl(&LoadConfig);

	UnloadButton.SetText("unload");
	UnloadButton.SetCallback(UnLoadCallbk);
	UnloadButton.SetSize(85, 16);
	UnloadButton.SetPosition(92, 243);
	RegisterControl(&UnloadButton);

	Config.SetText(XorStr("configs"));
	Config.SetSize(247, 278);
	Config.SetPosition(86, -9);
	RegisterControl(&Config);

	Configs.SetSize(247, 248);
	Configs.SetPosition(86, -4);
	Configs.SetHeightInItems(15);
	Configs.AddItem("1");
	Configs.AddItem("2");
	Configs.AddItem("3");
	Configs.AddItem("4");
	Configs.AddItem("5");
	Configs.AddItem("6");
	RegisterControl(&Configs);
}

void ColorsTab::Setup()
{
	SetTitle("v_color");
#pragma region Visual Colors
	ColorsCT.SetText(XorStr(""));
	ColorsCT.SetSize(292, 335);
	ColorsCT.SetPosition(86, -13);
	ColorsCT.AddTab(CGroupTab("main", 1));
	RegisterControl(&ColorsCT);

	CTBoxR.SetFileId("ct_box_r");
	CTBoxR.SetBoundaries(0.f, 255.f);
	CTBoxR.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("ct red"), this, &CTBoxR);

	CTBoxG.SetFileId("ct_box_g");
	CTBoxG.SetBoundaries(0.f, 255.f);
	CTBoxG.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("ct green"), this, &CTBoxG);

	CTBoxB.SetFileId("ct_box_b");
	CTBoxB.SetBoundaries(0.f, 255.f);
	CTBoxB.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("ct blue"), this, &CTBoxB);

	CTBoxRnv.SetFileId("ct_box_h_r");
	CTBoxRnv.SetBoundaries(0.f, 255.f);
	CTBoxRnv.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("ct hidden red"), this, &CTBoxRnv);

	CTBoxGnv.SetFileId("ct_box_h_g");
	CTBoxGnv.SetBoundaries(0.f, 255.f);
	CTBoxGnv.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("ct hidden green"), this, &CTBoxGnv);

	CTBoxBnv.SetFileId("ct_box_h_b");
	CTBoxBnv.SetBoundaries(0.f, 255.f);
	CTBoxBnv.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("ct hidden blue"), this, &CTBoxBnv);

	TBoxR.SetFileId("t_box_r");
	TBoxR.SetBoundaries(0.f, 255.f);
	TBoxR.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("t red"), this, &TBoxR);

	TBoxG.SetFileId("t_box_g");
	TBoxG.SetBoundaries(0.f, 255.f);
	TBoxG.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("t green"), this, &TBoxG);

	TBoxB.SetFileId("t_box_b");
	TBoxB.SetBoundaries(0.f, 255.f);
	TBoxB.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("t blue"), this, &TBoxB);

	TBoxRnv.SetFileId("t_box_h_r");
	TBoxRnv.SetBoundaries(0.f, 255.f);
	TBoxRnv.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("t hidden red"), this, &TBoxRnv);

	TBoxGnv.SetFileId("t_box_h_g");
	TBoxGnv.SetBoundaries(0.f, 255.f);
	TBoxGnv.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("t hidden green"), this, &TBoxGnv);

	TBoxBnv.SetFileId("t_box_h_b");
	TBoxBnv.SetBoundaries(0.f, 255.f);
	TBoxBnv.SetValue(39.f);
	ColorsCT.PlaceLabledControl3(1, XorStr("t hidden blue"), this, &TBoxBnv);

#pragma endregion Visual Colors
}
void CSkinchangerTab::Setup()
{
	SetTitle("SKINS");

	SkinActive.SetPosition(16, 16);
	SkinActive.SetText("Active");
	RegisterControl(&SkinActive);

	SkinEnable.SetFileId("skin_enable");
	SkinEnable.SetPosition(66, 16);
	RegisterControl(&SkinEnable);

	SkinApply.SetText("Apply");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(408, 500);
	SkinApply.SetSize(250, 50);
	RegisterControl(&SkinApply);

	KnifeGroup.SetPosition(110, 24);
	KnifeGroup.SetText("Knife");
	KnifeGroup.SetSize(331, 80);
	RegisterControl(&KnifeGroup);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Falchion Knife");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl("Knife", this, &KnifeModel);

	KnifeSkin.SetFileId("knife_skin");
	KnifeSkin.AddItem("None");
	KnifeSkin.AddItem("Crimson Web");
	KnifeSkin.AddItem("Bone Mask");
	KnifeSkin.AddItem("Fade");
	KnifeSkin.AddItem("Night");
	KnifeSkin.AddItem("Blue Steel");
	KnifeSkin.AddItem("Stained");
	KnifeSkin.AddItem("Case Hardened");
	KnifeSkin.AddItem("Slaughter");
	KnifeSkin.AddItem("Safari Mesh");
	KnifeSkin.AddItem("Boreal Forest");
	KnifeSkin.AddItem("Ultraviolet");
	KnifeSkin.AddItem("Urban Masked");
	KnifeSkin.AddItem("Scorched");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Tiger Tooth");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Marble Fade");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Doppler Ruby");
	KnifeSkin.AddItem("Doppler Sapphire");
	KnifeSkin.AddItem("Doppler Blackpearl");
	KnifeSkin.AddItem("Doppler Phase 1");
	KnifeSkin.AddItem("Doppler Phase 2");
	KnifeSkin.AddItem("Doppler Phase 3");
	KnifeSkin.AddItem("Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Phase 1");
	KnifeSkin.AddItem("Gamma Doppler Phase 2");
	KnifeSkin.AddItem("Gamma Doppler Phase 3");
	KnifeSkin.AddItem("Gamma Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Emerald");
	KnifeSkin.AddItem("Lore");
	KnifeGroup.PlaceLabledControl("Skin", this, &KnifeSkin);
#pragma endregion

	GloveGroup.SetPosition(443, 24);
	GloveGroup.SetText("Gloves");
	GloveGroup.SetSize(331, 80);
	RegisterControl(&GloveGroup);
	GloveModel.SetFileId("glove_model");
	GloveModel.AddItem("OFF");
	GloveModel.AddItem("BloodHound");
	GloveModel.AddItem("HandWrap");
	GloveModel.AddItem("Driver");
	GloveModel.AddItem("Sport");
	GloveModel.AddItem("Motorcycle");
	GloveModel.AddItem("Specialist");
	GloveGroup.PlaceLabledControl("Glove Model", this, &GloveModel);

	GloveSkin.SetFileId("glove_skin");
	GloveSkin.AddItem("None");
	GloveSkin.AddItem("BloodHound - Snakebite"); //10007
	GloveSkin.AddItem("BloodHound - Charred");// 10006
	GloveSkin.AddItem("BloodHound - Guerrilla");// 10039
	GloveSkin.AddItem("BloodHound - Bronzed");// 10008
	GloveSkin.AddItem("HandWrap - Slaughter"); // 10021
	GloveSkin.AddItem("HandWrap - Badlands");// 10036
	GloveSkin.AddItem("HandWrap - Leather");// 10009
	GloveSkin.AddItem("HandWrap - Spruce DDPAT");// 10010
	GloveSkin.AddItem("Driver - Crimson Weave");// 10016
	GloveSkin.AddItem("Driver - Lunar Weave");// 10013
	GloveSkin.AddItem("Driver - Diamondback");// 10040
	GloveSkin.AddItem("Driver - Convoy");// 10015
	GloveSkin.AddItem("Sport - Pandora's Box");// 10037
	GloveSkin.AddItem("Sport - Hedge Maze");// 10038
	GloveSkin.AddItem("Sport - Superconductor");// 10018
	GloveSkin.AddItem("Sport - Arid");// 10019
	GloveSkin.AddItem("Motorcycle - Spearmint");// 10026
	GloveSkin.AddItem("Motorcycle - Cool Mint");// 10028
	GloveSkin.AddItem("Motorcycle - Boom");// 10027
	GloveSkin.AddItem("Motorcycle - Eclipse");// 10024
	GloveSkin.AddItem("Specialist - Crimson Kimono");// 10033
	GloveSkin.AddItem("Specialist - Emerald Web");// 10034
	GloveSkin.AddItem("Specialist - Foundation");// 10035
	GloveSkin.AddItem("Specialist - Forest DDPAT");// 10030
	GloveGroup.PlaceLabledControl("Skin", this, &GloveSkin);

#pragma region Snipers
	Snipergroup.SetPosition(110, 120);
	Snipergroup.SetText("Snipers");
	Snipergroup.SetSize(331, 155);
	RegisterControl(&Snipergroup);

	AWPSkin.SetFileId("awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Snake Camo");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Safari Mesh");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Worm God");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	Snipergroup.PlaceLabledControl("AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("sgg08_skin");
	SSG08Skin.AddItem("Lichen Dashed");
	SSG08Skin.AddItem("Dark Water");
	SSG08Skin.AddItem("Blue Spruce");
	SSG08Skin.AddItem("Sand Dune");
	SSG08Skin.AddItem("Palm");
	SSG08Skin.AddItem("Mayan Dreams");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Tropical Storm");
	SSG08Skin.AddItem("Acid Fade");
	SSG08Skin.AddItem("Slashed");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	SSG08Skin.AddItem("Necropos");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Dragonfire");
	Snipergroup.PlaceLabledControl("SGG 08", this, &SSG08Skin);

	SCAR20Skin.SetFileId("scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Storm");
	SCAR20Skin.AddItem("Contractor");
	SCAR20Skin.AddItem("Carbon Fiber");
	SCAR20Skin.AddItem("Sand Mesh");
	SCAR20Skin.AddItem("Palm");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Army Sheen");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Green Marine");
	SCAR20Skin.AddItem("Outbreak");
	SCAR20Skin.AddItem("Bloodsport");
	SCAR20Skin.AddItem("Slaughter");
	SCAR20Skin.AddItem("Tiger Tooth");
	SCAR20Skin.AddItem("MFade");
	Snipergroup.PlaceLabledControl("SCAR-20", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("g3sg1_skin");
	G3SG1Skin.AddItem("Desert Storm");
	G3SG1Skin.AddItem("Arctic Camo");
	G3SG1Skin.AddItem("Bone Mask");
	G3SG1Skin.AddItem("Contractor");
	G3SG1Skin.AddItem("Safari Mesh");
	G3SG1Skin.AddItem("Polar Camo");
	G3SG1Skin.AddItem("Jungle Dashed");
	G3SG1Skin.AddItem("VariCamo");
	G3SG1Skin.AddItem("Predator");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	G3SG1Skin.AddItem("Orange Crash");
	G3SG1Skin.AddItem("Slaughter");
	G3SG1Skin.AddItem("Tiger Tooth");
	G3SG1Skin.AddItem("MFade");
	Snipergroup.PlaceLabledControl("G3SG1", this, &G3SG1Skin);
#pragma endregion

#pragma region Rifles
	Riflegroup.SetPosition(110, 295);
	Riflegroup.SetText("Rifles");
	Riflegroup.SetSize(331, 200);
	RegisterControl(&Riflegroup);

	AK47Skin.SetFileId("ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Elite Build");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	AK47Skin.AddItem("Slaughter");
	AK47Skin.AddItem("Tiger Tooth");
	AK47Skin.AddItem("MFade");
	Riflegroup.PlaceLabledControl("AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Boreal Forest");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	M41SSkin.AddItem("Slaughter");
	M41SSkin.AddItem("Tiger Tooth");
	M41SSkin.AddItem("MFade");
	Riflegroup.PlaceLabledControl("M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	M4A4Skin.AddItem("Slaughter");
	M4A4Skin.AddItem("Tiger Tooth");
	M4A4Skin.AddItem("MFade");
	Riflegroup.PlaceLabledControl("M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	Riflegroup.PlaceLabledControl("AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("famas_skin");
	FAMASSkin.AddItem("Contrast Spray");
	FAMASSkin.AddItem("Colony");
	FAMASSkin.AddItem("Cyanospatter");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	Riflegroup.PlaceLabledControl("FAMAS", this, &FAMASSkin);

	GALILSkin.SetFileId("galil_skin");
	GALILSkin.AddItem("Forest DDPAT");
	GALILSkin.AddItem("Contrast Spray");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Winter Forest");
	GALILSkin.AddItem("Sage Spray");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Shattered");
	GALILSkin.AddItem("Kami");
	GALILSkin.AddItem("Blue Titanium");
	GALILSkin.AddItem("Urban Rubble");
	GALILSkin.AddItem("Hunting Blind");
	GALILSkin.AddItem("Sandstorm");
	GALILSkin.AddItem("Tuxedo");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Firefight");
	Riflegroup.PlaceLabledControl("GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	Riflegroup.PlaceLabledControl("SG552", this, &SG553Skin);
#pragma endregion

#pragma region Pistols
	PistolGroup.SetPosition(443, 150);
	PistolGroup.SetText("Pistols");
	PistolGroup.SetSize(331, 345);
	RegisterControl(&PistolGroup);

	GLOCKSkin.SetFileId("glock_skin");
	GLOCKSkin.AddItem("Groundwater");
	GLOCKSkin.AddItem("Candy Apple");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Night");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Twilight Galaxy");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Brass");
	GLOCKSkin.AddItem("Catacombs");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Steel Disruption");
	GLOCKSkin.AddItem("Blue Fissure");
	GLOCKSkin.AddItem("Death Rattle");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Bunsen Burner");
	GLOCKSkin.AddItem("Wraith");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Weasel");
	GLOCKSkin.AddItem("Wasteland Rebel");
	PistolGroup.PlaceLabledControl("Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("usps_skin");
	USPSSkin.AddItem("Forest Leaves");
	USPSSkin.AddItem("Dark Water");
	USPSSkin.AddItem("VariCamo");
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Night Ops");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	PistolGroup.PlaceLabledControl("USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Pilot");
	DEAGLESkin.AddItem("Midnight Storm");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Forest DDPAT");
	DEAGLESkin.AddItem("Crimson Web");
	DEAGLESkin.AddItem("Urban DDPAT");
	DEAGLESkin.AddItem("Night");
	DEAGLESkin.AddItem("Hypnotic");
	DEAGLESkin.AddItem("Mudder");
	DEAGLESkin.AddItem("VariCamo");
	DEAGLESkin.AddItem("Golden Koi");
	DEAGLESkin.AddItem("Cobalt Disruption");
	DEAGLESkin.AddItem("Urban Rubble");
	DEAGLESkin.AddItem("Naga");
	DEAGLESkin.AddItem("Hand Cannon");
	DEAGLESkin.AddItem("Heirloom");
	DEAGLESkin.AddItem("Meteorite");
	DEAGLESkin.AddItem("Conspiracy");
	DEAGLESkin.AddItem("Bronze Deco");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Corinthian");
	DEAGLESkin.AddItem("Kumicho Dragon");
	DEAGLESkin.AddItem("Slaughter");
	DEAGLESkin.AddItem("Tiger Tooth");
	DEAGLESkin.AddItem("MFade");
	PistolGroup.PlaceLabledControl("Deagle", this, &DEAGLESkin);

	R8Skin.SetFileId("deagle_skin");
	R8Skin.AddItem("Bone Mask");
	R8Skin.AddItem("Crimson Web");
	R8Skin.AddItem("Fade");
	R8Skin.AddItem("Amber Fade");
	R8Skin.AddItem("Slaughter");
	R8Skin.AddItem("Tiger Tooth");
	R8Skin.AddItem("MFade");
	PistolGroup.PlaceLabledControl("Deagle", this, &R8Skin);

	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Ossified");
	DUALSSkin.AddItem("Stained");
	DUALSSkin.AddItem("Contractor");
	DUALSSkin.AddItem("Colony");
	DUALSSkin.AddItem("Demolition");
	DUALSSkin.AddItem("Black Limba");
	DUALSSkin.AddItem("Red Quartz");
	DUALSSkin.AddItem("Cobalt Quartz");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Urban Shock");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Panther");
	DUALSSkin.AddItem("Retribution");
	DUALSSkin.AddItem("Briar");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Moon in Libra");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Cartel");
	DUALSSkin.AddItem("Ventilators");
	PistolGroup.PlaceLabledControl("Duals", this, &DUALSSkin);

	FIVESEVENSkin.SetFileId("fiveseven_skin");
	FIVESEVENSkin.AddItem("Candy Apple");
	FIVESEVENSkin.AddItem("Bone Mask");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Contractor");
	FIVESEVENSkin.AddItem("Forest Night");
	FIVESEVENSkin.AddItem("Orange Peel");
	FIVESEVENSkin.AddItem("Jungle");
	FIVESEVENSkin.AddItem("Nitro");
	FIVESEVENSkin.AddItem("Red Quartz");
	FIVESEVENSkin.AddItem("Anodized Gunmetal");
	FIVESEVENSkin.AddItem("Nightshade");
	FIVESEVENSkin.AddItem("Silver Quartz");
	FIVESEVENSkin.AddItem("Kami");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Hot Shot");
	FIVESEVENSkin.AddItem("Urban Hazard");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	PistolGroup.PlaceLabledControl("Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("tec9_skin");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Groundwater");
	TECNINESkin.AddItem("Forest DDPAT");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Urban DDPAT");
	TECNINESkin.AddItem("Ossified");
	TECNINESkin.AddItem("Hades");
	TECNINESkin.AddItem("Brass");
	TECNINESkin.AddItem("VariCamo");
	TECNINESkin.AddItem("Nuclear Threat");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Blue Titanium");
	TECNINESkin.AddItem("Army Mesh");
	TECNINESkin.AddItem("Titanium Bit");
	TECNINESkin.AddItem("Sandstorm");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Jambiya");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Fuel Injector");
	PistolGroup.PlaceLabledControl("Tec-9", this, &TECNINESkin);

	P2000Skin.SetFileId("p2000_skin");
	P2000Skin.AddItem("Grassland Leaves");
	P2000Skin.AddItem("Silver");
	P2000Skin.AddItem("Granite Marbleized");
	P2000Skin.AddItem("Forest Leaves");
	P2000Skin.AddItem("Ossified");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Fade");
	P2000Skin.AddItem("Scorpion");
	P2000Skin.AddItem("Grassland");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Pulse");
	P2000Skin.AddItem("Amber Fade");
	P2000Skin.AddItem("Red FragCam");
	P2000Skin.AddItem("Chainmail");
	P2000Skin.AddItem("Coach Class");
	P2000Skin.AddItem("Ivory");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Asterion");
	P2000Skin.AddItem("Pathfinder");
	P2000Skin.AddItem("Imperial");
	P2000Skin.AddItem("Oceanic");
	P2000Skin.AddItem("Imperial Dragon");
	PistolGroup.PlaceLabledControl("P2000", this, &P2000Skin);

	P250Skin.SetFileId("p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Metallic DDPAT");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Gunsmoke");
	P250Skin.AddItem("Modern Hunter");
	P250Skin.AddItem("Bone Mask");
	P250Skin.AddItem("Boreal Forest");
	P250Skin.AddItem("Sand Dune");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Facets");
	P250Skin.AddItem("Hive");
	P250Skin.AddItem("Muertos");
	P250Skin.AddItem("Steel Disruption");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Neon Kimono");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Contamination");
	P250Skin.AddItem("Cartel");
	P250Skin.AddItem("Valence");
	P250Skin.AddItem("Crimson Kimono");
	P250Skin.AddItem("Mint Kimono");
	P250Skin.AddItem("Wing Shot");
	P250Skin.AddItem("Asiimov");
	PistolGroup.PlaceLabledControl("P250", this, &P250Skin);

#pragma endregion

#pragma endregion other random options
}

void CPlayersTab::Setup()
{
	SetTitle("Players");
	RegisterControl(&Menu::Window.UserLabel);


	grpPList.SetPosition(110, 20);
	grpPList.SetSize(497, 419);
	grpPList.SetText("PlayerList");
	grpPList.SetColumns(2);
	RegisterControl(&grpPList);

	lstPlayers.SetPosition(120, 40);
	lstPlayers.SetSize(440, 50);
	lstPlayers.SetHeightInItems(20);
	RegisterControl(&lstPlayers);

	grpPList.PlaceLabledControlPLayerlist("Friendly", this, &PlayerFriendly, 0, 17);
	grpPList.PlaceLabledControlPLayerlist("Aim-Priorety", this, &PlayerAimPrio, 1, 17);
	grpPList.PlaceLabledControlPLayerlist("Callout Spam", this, &PlayerCalloutSpam, 0, 18);


}

DWORD GetPlayerListIndex(int EntId)
{
	player_info_t pinfo;
	I::Engine->GetPlayerInfo(EntId, &pinfo);

	// Bot
	if (pinfo.guid[0] == 'B' && pinfo.guid[1] == 'O')
	{
		char buf[64]; sprintf_s(buf, "BOT_420%sAY", pinfo.name);
		return CRC32(buf, 64);
	}
	else
	{
		return CRC32(pinfo.guid, 32);
	}
}

bool IsFriendly(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].Friendly;
	}

	return false;
}

bool IsAimPrio(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].AimPrio;
	}

	return false;
}

bool IsCalloutTarget(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].Callout;
	}

	return false;
}

void UpdatePlayerList()
{
	IClientEntity* pLocal = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());
	if (I::Engine->IsConnected() && I::Engine->IsInGame() && pLocal && pLocal->IsAlive())
	{
		Menu::Window.PlayersTab.lstPlayers.ClearItems();

		// Loop through all active entitys
		for (int i = 0; i < I::EntList->GetHighestEntityIndex(); i++)
		{
			// Get the entity

			player_info_t pinfo;
			if (i != I::Engine->GetLocalPlayer() && I::Engine->GetPlayerInfo(i, &pinfo))
			{
				IClientEntity* pEntity = I::EntList->GetClientEntity(i);
				int HP = 100; char* Location = "Unknown";
				char *Friendly = " ", *AimPrio = " ";

				DWORD plistId = GetPlayerListIndex(Menu::Window.PlayersTab.lstPlayers.GetValue());
				if (PlayerList.find(plistId) != PlayerList.end())
				{
					Friendly = PlayerList[plistId].Friendly ? "Friendly" : "";
					AimPrio = PlayerList[plistId].AimPrio ? "AimPrio" : "";
				}

				if (pEntity && !pEntity->IsDormant())
				{
					HP = pEntity->GetHealth();
					Location = pEntity->GetLastPlaceName();
				}

				char nameBuffer[512];
				sprintf_s(nameBuffer, "%-24s %-10s %-10s ", pinfo.name, IsFriendly(i) ? "Friend" : " ", IsAimPrio(i) ? "AimPrio" : " ", HP, Location);
				Menu::Window.PlayersTab.lstPlayers.AddItem(nameBuffer, i);

			}
			
		}

		DWORD meme = GetPlayerListIndex(Menu::Window.PlayersTab.lstPlayers.GetValue());

		// Have we switched to a different player?
		static int PrevSelectedPlayer = 0;
		if (PrevSelectedPlayer != Menu::Window.PlayersTab.lstPlayers.GetValue())
		{
			if (PlayerList.find(meme) != PlayerList.end())
			{
				Menu::Window.PlayersTab.PlayerFriendly.SetState(PlayerList[meme].Friendly);
				Menu::Window.PlayersTab.PlayerAimPrio.SetState(PlayerList[meme].AimPrio);
				Menu::Window.PlayersTab.PlayerCalloutSpam.SetState(PlayerList[meme].Callout);

			}
			else
			{
				Menu::Window.PlayersTab.PlayerFriendly.SetState(false);
				Menu::Window.PlayersTab.PlayerAimPrio.SetState(false);
				Menu::Window.PlayersTab.PlayerCalloutSpam.SetState(false);

			}
		}
		PrevSelectedPlayer = Menu::Window.PlayersTab.lstPlayers.GetValue();

		PlayerList[meme].Friendly = Menu::Window.PlayersTab.PlayerFriendly.GetState();
		PlayerList[meme].AimPrio = Menu::Window.PlayersTab.PlayerAimPrio.GetState();
		PlayerList[meme].Callout = Menu::Window.PlayersTab.PlayerCalloutSpam.GetState();

	}
}

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{
	GUI.Update();
	GUI.Draw();
	
}

// https://steamcommunity.com/id/simplerealistic/