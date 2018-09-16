#include "Settings.h"
#include "Menu.h"

RagebotConfig ragebotconfig;
HitscanConfig hitscanconfig;
AAConfig antiaimconfig;
ResolverConfig resolverconfig;
VisualsConfig visualconfig;
MiscConfig miscconfig;
SkinChangerConfig skinconfig;
Settings settings;

void Settings::Update()
{
	//Ragebot
	ragebotconfig.bEnabled = Menu::Window.RageBotTab.Enabled.GetState();
	ragebotconfig.iTargetSelection = Menu::Window.RageBotTab.TargetSelection.GetIndex();

	ragebotconfig.bMultiPoint = Menu::Window.RageBotTab.MultiPoint.GetState();
	ragebotconfig.iMultipointType = Menu::Window.RageBotTab.MultipointType.GetValue();

	ragebotconfig.bAutoFire = Menu::Window.RageBotTab.AutoFire.GetState();
	ragebotconfig.iAutoFireMode = Menu::Window.RageBotTab.AutoFireMode.GetIndex();
	ragebotconfig.iAutoFireHitbox = Menu::Window.RageBotTab.AutoFireTarget.GetIndex();

	ragebotconfig.iAutoFireKey = Menu::Window.RageBotTab.AutoFireKey.GetKey();
	ragebotconfig.bAutoWall = Menu::Window.RageBotTab.AutoWall.GetState();
	ragebotconfig.bSilent = Menu::Window.RageBotTab.SilentAim.GetState();
	ragebotconfig.bPSilent = Menu::Window.RageBotTab4.PSilentAim.GetState();

	ragebotconfig.bSyncHitchance = Menu::Window.RageBotTab.SyncHitChance.GetState();

	ragebotconfig.flMaxFov = Menu::Window.RageBotTab.MaximumFov.GetValue();
	ragebotconfig.flMinDmg = Menu::Window.RageBotTab.MinimumDamage.GetValue();
	ragebotconfig.bAutoScope = Menu::Window.RageBotTab.AutomaticScope.GetState();

	ragebotconfig.bRemoveRecoil = Menu::Window.RageBotTab4.RemoveRecoil.GetState();

	//Hitscan
	hitscanconfig.hitboxes = Menu::Window.RageBotTab.HitscanBones.items;

	//Anti Aim
	/*antiaimconfig.iRealPitch = Menu::Window.RageBotTab.RealPitch.GetIndex();
	antiaimconfig.iRealYaw = Menu::Window.RageBotTab.RealYaw.GetIndex();
	antiaimconfig.iFakeYaw = Menu::Window.RageBotTab.FakeYaw.GetIndex();
	antiaimconfig.bDormantCheck = Menu::Window.RageBotTab.DormantCheck.GetState();
	antiaimconfig.iRealYawDirection = Menu::Window.RageBotTab.RealYawDirection.GetIndex();

	antiaimconfig.bBreakLowerbody = Menu::Window.RageBotTab.BreakLBY.GetState();*/

	//Resolver
	resolverconfig.iResolverMode = Menu::Window.RageBotTab.ResolverMode.GetIndex();
	resolverconfig.bInterpLagComp = Menu::Window.RageBotTab.bInterpLagComp.GetState();


	//Visuals
	visualconfig.bEnemyEspOnly = Menu::Window.VisualsTab.EnemyEspOnly.GetState();
	visualconfig.iEspMode = Menu::Window.VisualsTab.EspMode.GetIndex();
	visualconfig.bHealth = Menu::Window.VisualsTab.ShowHealth.GetState();
	visualconfig.iHealthBar = Menu::Window.VisualsTab.HealthBar.GetState();
	visualconfig.bArmor = Menu::Window.VisualsTab.ShowArmor.GetState();
	visualconfig.iArmorLocation = Menu::Window.VisualsTab.ArmorLocation.GetIndex();
	visualconfig.bSkeletons = Menu::Window.VisualsTab.ShowSkeletons.GetState();
	visualconfig.bPlayerNames = Menu::Window.VisualsTab.ShowPlayerNames.GetState();
	visualconfig.bWeaponNames = Menu::Window.VisualsTab.ShowWeaponNames.GetState();
	visualconfig.iWeaponNamesType = Menu::Window.VisualsTab.WeaponNames_Type.GetIndex();

	visualconfig.bWeaponScope = Menu::Window.VisualsTab.WeaponScope.GetState();
	visualconfig.bRemoveParticles = Menu::Window.VisualsTab.RemoveParticles.GetState();
	visualconfig.bNoVisualRecoil = Menu::Window.RageBotTab4.NoVisualRecoil.GetState();
	visualconfig.bFoV = Menu::Window.VisualsTab.FovChanger.GetState();
	visualconfig.flFoVAmt = Menu::Window.VisualsTab.FovChangerAmt.GetValue();
	visualconfig.bViewmodelFoV = Menu::Window.VisualsTab.ViewmodelFovChanger.GetState();
	visualconfig.flViewmodelFoVAmt = Menu::Window.VisualsTab.ViewmodelFovChangerAmt.GetValue();
	visualconfig.bThirdpersonAA = Menu::Window.VisualsTab.ThirdpersonAA.GetState();
	visualconfig.bNightMode = Menu::Window.VisualsTab.NightMode.GetState();
	visualconfig.bHitmarkers = Menu::Window.VisualsTab.Hitmarkers.GetState();
	visualconfig.bHitmarkers_Sounds = Menu::Window.VisualsTab.Hitmarkers_Sounds.GetState();

	visualconfig.iPlayerChamType = Menu::Window.VisualsTab.PlayerChamType.GetIndex();
	visualconfig.iViewmodelChamType = Menu::Window.VisualsTab.ViewModelChamType.GetIndex();
	visualconfig.flPlayerAlpha = Menu::Window.VisualsTab.PlayerAlpha.GetValue();
	visualconfig.flViewmodelAlpha = Menu::Window.VisualsTab.ViewmodelAlpha.GetValue();

	visualconfig.bCrosshair = Menu::Window.VisualsTab.Crosshair.GetState();
	visualconfig.iCrosshairType = Menu::Window.VisualsTab.CrosshairType.GetIndex();

	//Misc
	miscconfig.bAutoJump = Menu::Window.MiscTab.BunnyHop.GetState();
//	miscconfig.bAutoStrafe = Menu::Window.MiscTab.Autostrafe.GetState();
	miscconfig.bZStrafe = Menu::Window.MiscTab.ZHop.GetState();
	miscconfig.bCircleStrafer = Menu::Window.MiscTab.CircleStrafe.GetState();
	miscconfig.iAirStuckKey = Menu::Window.MiscTab.AirStuckKey.GetKey();
	miscconfig.bSpectatorList = Menu::Window.MiscTab.SpectatorList.GetState();
	miscconfig.bClanChanger = Menu::Window.MiscTab.ClanChanger.GetState();
	miscconfig.iClanChangerTag = Menu::Window.MiscTab.ClanChangerType.GetIndex();
	miscconfig.bNameChanger = Menu::Window.MiscTab.NameChanger.GetState();
	miscconfig.iNameChangerType = Menu::Window.MiscTab.NameChangerType.GetIndex();
	miscconfig.bChatSpammer = Menu::Window.MiscTab.ChatSpammer.GetState();
	miscconfig.iChatSpammerType = Menu::Window.MiscTab.ChatSpammerType.GetIndex();
	miscconfig.iChatSpammerSpeed = Menu::Window.MiscTab.ChatSpammerSpeed.GetInt();
	miscconfig.bRadioSpammer = Menu::Window.MiscTab.RadioSpammer.GetState();
	miscconfig.bKillMessage = Menu::Window.MiscTab.KillMessage.GetState();
	miscconfig.iKillMessageType = Menu::Window.MiscTab.KillMessageType.GetIndex();
	miscconfig.bEnableUntrustedOptions = Menu::Window.MiscTab.EnableUntrustedOptions.GetState();

	miscconfig.bFlagEnabled = Menu::Window.MiscTab.FlagType.GetIndex() != 0;
	miscconfig.iFlagType = Menu::Window.MiscTab.FlagType.GetIndex();
	miscconfig.iFlagFactor = Menu::Window.MiscTab.FlagFactor.GetValue();
	miscconfig.bFlagRandomize = Menu::Window.MiscTab.FlagRandomize.GetState();
	miscconfig.bFlagInAir = Menu::Window.MiscTab.FlagInAir.GetState();

	miscconfig.iMenuRed = Menu::Window.MiscTab.Menu_Red.GetValue();
	miscconfig.iMenuGreen = Menu::Window.MiscTab.Menu_Green.GetValue();
	miscconfig.iMenuBlue = Menu::Window.MiscTab.Menu_Blue.GetValue();

	//Skins
	//skinconfig.iKnife = Menu::Window.SkinchangerTab.KnifeSelection.GetIndex();
}