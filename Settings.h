#pragma once

#include "Controls.h"

struct RagebotConfig
{
	bool bEnabled;
	bool bBacktrack;
	int iTargetSelection;

	bool bMultiPoint;
	int iMultipointType = 1;
	float flPointscale;
	bool bAutoFire;
	int iAutoFireMode;
	int iAutoFireHitbox;

	int iAutoFireKey;
	bool bAutoWall;
	bool bSilent;
	bool bPSilent;

	bool bSyncHitchance;


	float flMaxFov;
	float flMinDmg;
	bool bAutoScope;

	bool bRemoveRecoil;
};

struct HitscanConfig
{
	std::vector<MultiBoxItem> hitboxes;


};

struct AAConfig
{
	int iRealPitch;
	int iRealYaw;
	int iFakeYaw;
	bool bDormantCheck;
	bool bBreakResolver;
	bool bBreakLBY;
	int iRealYawDirection;


	bool bBreakLowerbody;
};

struct ResolverConfig
{
	int iResolverMode;
	bool bBacktrack;
	bool bInterpLagComp;
};

struct VisualsConfig
{
	bool bEnemyEspOnly;
	int iEspMode;
	bool bHealth;
	int iHealthBar;
	bool bArmor;
	int iArmorLocation;
	bool bSkeletons;
	bool bBullet;
	bool bPlayerNames;
	bool bWeaponNames;
	int iWeaponNamesType;
	
	bool bWeaponScope;
	bool bRemoveParticles;
	bool bNoVisualRecoil;
	bool bFoV;
	float flFoVAmt;
	bool bViewmodelFoV;
	float flViewmodelFoVAmt;
	bool bThirdpersonAA;
	bool bThirdPerson;
	bool bNightMode;
	bool bHitmarkers;
		bool bHitmarkers_Sounds;

	int iPlayerChamType;
	int iViewmodelChamType;
	float flPlayerAlpha;
	float flViewmodelAlpha;

	bool bCrosshair;
	int iCrosshairType;
};

struct MiscConfig
{
	bool bAutoJump;
//	bool bAutoStrafe;
	int iAutoStrafeType;
	bool bZStrafe;
	bool bCircleStrafer;
	int iAirStuckKey;
	bool bSpectatorList;
	bool bFlagEnabled;
	bool bClanChanger;
	int iClanChangerTag;
	bool bNameChanger;
	int iNameChangerType;
	bool bChatSpammer;
	int iChatSpammerType;
	int iChatSpammerSpeed;
	bool bRadioSpammer;
	bool bKillMessage;
	int iKillMessageType;
	bool bBreakResolvers;
	bool bEnableUntrustedOptions;

	int iFlagType;
	int iFlagFactor;
	bool bFlagRandomize;
	bool bFlagInAir;

	float iMenuRed;
	float iMenuGreen;
	float iMenuBlue;
};

struct SkinChangerConfig
{
	int iKnife;
};

extern RagebotConfig ragebotconfig;
extern HitscanConfig hitscanconfig;
extern AAConfig antiaimconfig;
extern ResolverConfig resolverconfig;
extern VisualsConfig visualconfig;
extern MiscConfig miscconfig;
extern SkinChangerConfig skinconfig;

struct Settings
{
	void Update();
};

extern Settings settings;