

#pragma once

#include "GUI.h"
#include "Controls.h"

class CLegitBotTab : public CTab
{
public:
	void Setup();

	// Master Switch

	// Aimbot Settings
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotAutoPistol;
	CSlider   AimbotInaccuracy;
	CKeyBind  AimbotDisableVis;

	// Main
	CGroupBox TriggerGroup;
	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CKeyBind  TriggerKeyBind;
	CSlider   TriggerDelay;

	// Main
	CGroupBox WeaponMainGroup;
	CSlider   WeaponMainSpeed;
	CSlider   WeaponMainFoV;
	CCheckBox WeaponMainRecoil;
	CSlider  WeaponMainRecoilAmountX;
	CSlider  WeaponMainRecoilAmountY;
	CCheckBox WeaponMainPSilent;
	CSlider   WeaponMainInacc;
	CComboBox WeaponMainHitbox;

	// Pistol
	CGroupBox WeaponPistGroup;
	CSlider   WeaponPistSpeed;
	CSlider   WeaponPistFoV;
	CCheckBox WeaponPistRecoil;
	CSlider  WeaponPistRecoilAmountX;
	CSlider  WeaponPistRecoilAmountY;
	CCheckBox WeaponPistPSilent;
	CSlider   WeaponPistInacc;
	CComboBox WeaponPistHitbox;

	// Sniper
	CCheckBox WeaponSnipEnable;
	CGroupBox WeaponSnipGroup;
	CSlider   WeaponSnipSpeed;
	CSlider   WeaponSnipFoV;
	CCheckBox WeaponSnipRecoil;
	CSlider  WeaponSnipRecoilAmountX;
	CSlider  WeaponSnipRecoilAmountY;
	CCheckBox WeaponSnipPSilent;
	CSlider   WeaponSnipInacc;
	CComboBox WeaponSnipHitbox;

	//More
	CGroupBox AimMoreGroup;
	CCheckBox Smokecheck;
	CSlider  HitboxIndexes;

};



class CMainRageTab : public CTab
{
public:
	void Setup();

	CGroupBox AimbotGroup;
	CCheckBox Enabled;
	CComboBox TargetSelection;

	CCheckBox WeaponMainRecoil;

	CCheckBox SyncHitChance;

	CSlider WeaponPistolHitchance;
	CSlider WeaponSniperHitchance;
	CSlider WeaponRifleHitchance;
	CSlider WeaponSMGHitchance;
	CSlider WeaponOtherHitchance;

	CCheckBox MultiPoint;
	CSlider MultipointType;

	CCheckBox AutoFire;
	CCheckBox AimbotAutoRevolver;

	CComboBox AutoFireMode;
	CComboBox AutoFireTarget;

	CMultiBox HitscanBones;

	CKeyBind AutoFireKey;
	CCheckBox AutoWall;
	CCheckBox SilentAim;

	CSlider MaximumFov;
	CSlider MinimumDamage;
	CCheckBox AutomaticScope;
	CCheckBox AutoCockRevolver;


	CCheckBox PerfectAccuracy;

	CGroupBox ResolverOptions;
	CComboBox ResolverMode;

	CLabel    Interplable;

	CCheckBox bInterpLagComp;


	CGroupBox AccuracyGroup;

	CCheckBox Edge;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoWall;
	CCheckBox AccuracyPrediction;
	CSlider	  AccuracyMinimumDamage;
	CCheckBox AccuracyAutoStop;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CSlider   AccuracyHitchance;

	CComboBox AccuracyResolverYaw;
	CSlider   AccuracySmart;



};

class CMiscRageTab : public CTab
{
public:
	void Setup();

	CGroupBox AimbotGroup;

	CCheckBox AntiAimKnife;

	

	CGroupBox DefaultAntiAimGroup;
	CComboBox AntiAimEnable;
	CLabel    Pitch;
	CComboBox AntiAimPitch;
	CLabel    Yaw;
	CComboBox AntiAimStandingYaw;
	CComboBox AntiAimMovingYaw;
	CComboBox AntiAimFakeYaw;
	CLabel    Resolvermodes;

	CLabel    ATTARGET;

	CLabel AntiAimknifeL;

	CCheckBox AntiAimTarget;

	CCheckBox RemoveRecoil;
	CCheckBox NoVisualRecoil;
	
	CCheckBox PSilentAim;
};

class CPlayersTab : public CTab
{
public:
	void Setup();
	CGroupBox grpPList;
	CListBox lstPlayers;

	CCheckBox PlayerFriendly;
	CCheckBox PlayerAimPrio;
	CCheckBox PlayerCalloutSpam;

};

class CVisualTab : public CTab
{
public:
	void Setup();

	CGroupBox EspGroup;
	CCheckBox Active;
	CCheckBox EnemyEspOnly;
	CComboBox EspMode;
	CCheckBox ShowHealth;
	CCheckBox FiltersWeapons;
	CCheckBox FiltersChickens;
	CCheckBox FiltersC4;
	CCheckBox HealthBar;
	CCheckBox ShowArmor;
	CComboBox ArmorLocation;
	CCheckBox ShowSkeletons;
	CCheckBox ShowPlayerNames;
	CCheckBox ShowWeaponNames;
	CCheckBox Money;
	CCheckBox OptionsWeapon;
	CCheckBox Grenades;
	CCheckBox AimSpot;
	CCheckBox Wireframe;
	CCheckBox CompRank;
	CCheckBox PlayerDefuse;
	CCheckBox OtherNoFlash;
	CCheckBox OtherNoSmoke;
	CCheckBox Antiscreenshot;
	CSlider BulletTraceLength;

	CCheckBox Info;

	CCheckBox BulletTrace;

	CComboBox OtherHandFX;

	CComboBox WeaponNames_Type;

	CComboBox OptionsOutline;
	CComboBox OptionsFill;
	CCheckBox OptionsName;
	CCheckBox OptionsMoney;
	CCheckBox Glow;
	CCheckBox OptionsVO;
	CCheckBox Distance;

	CCheckBox linesaa;

	CCheckBox Chams;

	CGroupBox ChamsGroup;
	CComboBox PlayerChamType;
	CComboBox ViewModelChamType;
	CSlider PlayerAlpha;
	CSlider ViewmodelAlpha;
	CComboBox WeaponsChamType;
	CComboBox DroppedWeaponsChamType;
	CSlider WeaponAlpha;
	CSlider DroppedWeaponAlpha;
	CComboBox OptionsChams;

	CGroupBox Filters;

	CCheckBox FiltersChamsAlpha;
	CCheckBox FilterSelf;

	CCheckBox aafakeorreal;
	CGroupBox Effects;
	CCheckBox WeaponScope;
	CCheckBox RemoveParticles;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox SpreadCrosshair;
	CCheckBox FovChanger;
	CComboBox	AmbientSkybox;
	CSlider FovChangerAmt;
	CCheckBox ViewmodelFovChanger;
	CSlider ViewmodelFovChangerAmt;
	CCheckBox ThirdpersonAA;
	CComboBox Thirdaa;
	CCheckBox  NoScope;
	CCheckBox OtherNoScope;
	CCheckBox NightMode;
	CCheckBox CustomSky;
	CCheckBox OptionsLBY;
	CCheckBox OtherScope;
	CCheckBox Hitmarkers;
	CCheckBox Hitmarkers_Sounds;

	CGroupBox OtherESP;
	CCheckBox Crosshair;
	CComboBox CrosshairType;
};

class CMoveTab : public CTab
{
public:
	void Setup();

	CGroupBox MainGroup;

	CCheckBox AutoStrafer;
};

class CMiscTab : public CTab
{
public:
	void Setup();

	CGroupBox MainGroup;
	CCheckBox BunnyHop;
	CCheckBox ZHop;
	CCheckBox CircleStrafe;
	CKeyBind OtherCircleKey;
	CKeyBind AirStuckKey;
	CCheckBox OtherCircleStrafe;
	CKeyBind OtherCircleStrafeKey;
	CCheckBox SpectatorList;
	CCheckBox ClanChanger;
	CCheckBox OtherLBY;
	CComboBox ClanChangerType;
	//CCheckBox ThirdPerson;
	CCheckBox NameChanger;
	CComboBox NameChangerType;
	CGroupBox MenuColors;
	CSlider Menu_Red;
	CSlider Menu_Green;
	CSlider Menu_Blue;
	CCheckBox ChatSpammer;
	CButton UnloadButton;

	CComboBox ThirdPersonType;
	CCheckBox ThirdPerson;
	CKeyBind Thirdpersonkey;

	CCheckBox Logs;

	CComboBox ChatSpammerType;
	CItemSelector ChatSpammerSpeed;
	CCheckBox RadioSpammer;
	CCheckBox KillMessage;
	CComboBox KillMessageType;
	CCheckBox bBreakLBY;
	CCheckBox bBreakResolver;

	CCheckBox BreakLBY;
	CCheckBox BreakResolver;

	CComboBox SelectedConfig;
	CCheckBox EnableUntrustedOptions;
	CCheckBox atrust;

	CGroupBox FlagGroup;
	CComboBox FlagType;
	CSlider FlagFactor;
	CCheckBox FlagRandomize;
	CCheckBox FlagInAir;

	CButton SaveConfig;
	CButton LoadConfig;

	CGroupBox Config;
	CListBox Configs;

	CCheckBox PostProcessing;
};

class CMiscTab2 : public CTab
{
public:
	void Setup();

	CGroupBox MainGroup;
	CCheckBox BunnyHop;
	//CCheckBox Autostrafe;
	CComboBox AutoStrafeType;
	CCheckBox ZHop;
	CCheckBox CircleStrafe;
	CKeyBind OtherCircleKey;
	CKeyBind AirStuckKey;
	CCheckBox OtherCircleStrafe;
	CKeyBind OtherCircleStrafeKey;
	CCheckBox SpectatorList;
	CCheckBox ClanChanger;
	CCheckBox OtherLBY;
	CComboBox ClanChangerType;
	//CCheckBox ThirdPerson;
	CCheckBox NameChanger;
	CComboBox NameChangerType;
	CGroupBox MenuColors;
	CSlider Menu_Red;
	CSlider Menu_Green;
	CSlider Menu_Blue;
	CCheckBox ChatSpammer;
	CButton UnloadButton;

	CComboBox ChatSpammerType;
	CItemSelector ChatSpammerSpeed;
	CCheckBox RadioSpammer;
	CCheckBox KillMessage;
	CComboBox KillMessageType;
	CCheckBox bBreakLBY;
	CCheckBox bBreakResolver;

	CCheckBox BreakLBY;
	CCheckBox BreakResolver;

	CComboBox SelectedConfig;
	CCheckBox EnableUntrustedOptions;
	CCheckBox atrust;

	CGroupBox FlagGroup;
	CComboBox FlagType;
	CSlider FlagFactor;
	CCheckBox FlagRandomize;
	CCheckBox FlagInAir;

	CButton SaveConfig;
	CButton LoadConfig;

	CGroupBox Config;
	CListBox Configs;

	CCheckBox PostProcessing;
};

class ColorsTab : public CTab
{
public:
	void Setup();
	// Box ESP Colour
	CGroupBox ColorsCT;

	CGroupBox ColorsGlow;

	CGroupBox ColorsT;

	CGroupBox CTChams;

	CGroupBox TChams;

	//Glow
	CSlider GlowR;
	CSlider GlowG;
	CSlider GlowB;
	CSlider GlowZ;

	//CTBox
	CSlider CTBoxR;
	CSlider CTBoxG;
	CSlider CTBoxB;

	//Chams CTV
	CSlider CTChamsR;
	CSlider CTChamsG;
	CSlider CTChamsB;

	//CTBoxNV
	CSlider CTBoxRnv;
	CSlider CTBoxGnv;
	CSlider CTBoxBnv;

	//ChamsCTNV
	CSlider CTChamRnv;
	CSlider CTChamGnv;
	CSlider CTChamBnv;

	//TBox
	CSlider TBoxR;
	CSlider TBoxG;
	CSlider TBoxB;

	//Chams TV
	CSlider TChamsR;
	CSlider TChamsG;
	CSlider TChamsB;

	//TBox NV
	CSlider TBoxRnv;
	CSlider TBoxGnv;
	CSlider TBoxBnv;

	//Chams T NV
	CSlider TChamsRnv;
	CSlider TChamsGnv;
	CSlider TChamsBnv;

};

class CSkinchangerTab : public CTab
{
public:
	void Setup();

	CLabel SkinActive;
	CCheckBox SkinEnable;
	CButton   SkinApply;
	CButton   GloveApply;
	CListBox lstWeapon;
	CListBox lstSkins;

	// Knife
	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;

	//Gloves
	CGroupBox GloveGroup;
	CComboBox GloveModel;
	CComboBox GloveSkin;

	// Pistols
	CGroupBox PistolGroup;
	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;
	CComboBox R8Skin;

	// MPs
	CGroupBox MPGroup;
	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;

	// Rifles
	CGroupBox Riflegroup;
	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;


	// Machineguns
	CGroupBox MachinegunsGroup;
	CComboBox NEGEVSkin;
	CComboBox M249Skin;

	// Snipers
	CGroupBox Snipergroup;
	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox AWPSkin;

	// Shotguns
	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

	// Skinsettings
	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;
	CTextField StatTrackAmount;
	CTextField SkinName;
	CTextField KnifeName;
};

class SuicideWindow : public CWindow
{
public:
	void Setup();

	CMainRageTab RageBotTab;
	CMiscRageTab RageBotTab4;
	CLegitBotTab LegitBotTab;
	CVisualTab VisualsTab;
	CPlayersTab PlayersTab;
	CComboBox SettingsFile;
	CLabel UserLabel;
	CMiscTab MiscTab;
	CMoveTab MoveTab;
	CMiscTab2 MiscTab2;
	ColorsTab ColorsTab;
	CComboBox ConfigBox;
	CSkinchangerTab SkinchangerTab;
};

namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern SuicideWindow Window;
};

struct PlayerListItem_t
{
	bool Friendly;
	bool AimPrio;
	bool Callout;
};

DWORD GetPlayerListIndex(int EntId);
bool IsFriendly(int EntId);
bool IsAimPrio(int EntId);
bool IsCalloutTarget(int EntId);


extern std::map<DWORD, PlayerListItem_t> PlayerList;