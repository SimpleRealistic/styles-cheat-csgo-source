
#pragma once

#include "MiscDefinitions.h"
#include "ClientRecvProps.h"
#include "offsets.h"
#include "Vector.h"
#include "XORstr.hpp"

#define TEAM_CS_T 2
#define TEAM_CS_CT 3

#define BONE_USED_BY_HITBOX			0x00000100

#ifdef NDEBUG
#define strenc( s ) std::string( cx_make_encrypted_string( s ) )
#define charenc( s ) strenc( s ).c_str()
#define wstrenc( s ) std::wstring( strenc( s ).begin(), strenc( s ).end() )
#define wcharenc( s ) wstrenc( s ).c_str()
#else
#define strenc( s ) ( s )
#define charenc( s ) ( s )
#define wstrenc( s ) ( s )
#define wcharenc( s ) ( s )
#endif

#define VirtualFn( cast )typedef cast( __thiscall* OriginalFn )

namespace Virtual
{
	inline PVOID*& getvtable(PVOID inst, size_t offset = 0)
	{
		return *reinterpret_cast< PVOID** >((size_t)inst + offset);
	}
	inline const PVOID* getvtable(const void* inst, size_t offset = 0)
	{
		return *reinterpret_cast< const PVOID** >((size_t)inst + offset);
	}
	template< typename Fn >
	inline Fn CallVirtual(const void* inst, size_t index, size_t offset = 0)
	{
		return reinterpret_cast< Fn >(getvtable(inst, offset)[index]);
	}
}

class IClientRenderable;
class IClientNetworkable;
class IClientUnknown;
class IClientThinkable;
class IClientEntity;
class CSWeaponInfo;

class CSWeaponInfo
{
public:
	virtual ~CSWeaponInfo() {};
	char*		consoleName;			// 0x0004
	char		pad_0008[12];			// 0x0008
	int			iMaxClip1;				// 0x0014
	int			iMaxClip2;				// 0x0018
	int			iDefaultClip1;			// 0x001C
	int			iDefaultClip2;			// 0x0020
	char		pad_0024[8];			// 0x0024
	char*		szWorldModel;			// 0x002C
	char*		szViewModel;			// 0x0030
	char*		szDroppedModel;			// 0x0034
	char		pad_0038[4];			// 0x0038
	char*		N0000023E;				// 0x003C
	char		pad_0040[56];			// 0x0040
	char*		szEmptySound;			// 0x0078
	char		pad_007C[4];			// 0x007C
	char*		szBulletType;			// 0x0080
	char		pad_0084[4];			// 0x0084
	char*		szHudName;				// 0x0088
	char*		szWeaponName;			// 0x008C
	char		pad_0090[56];			// 0x0090
	int 		WeaponType;				// 0x00C8
	int			iWeaponPrice;			// 0x00CC
	int			iKillAward;				// 0x00D0
	char*		szAnimationPrefix;		// 0x00D4
	float		flCycleTime;			// 0x00D8
	float		flCycleTimeAlt;			// 0x00DC
	float		flTimeToIdle;			// 0x00E0
	float		flIdleInterval;			// 0x00E4
	bool		bFullAuto;				// 0x00E8
	char		pad_0x00E5[3];			// 0x00E9
	int			iDamage;				// 0x00EC
	float		flArmorRatio;			// 0x00F0
	int			iBullets;				// 0x00F4
	float		flPenetration;			// 0x00F8
	float		flFlinchVelocityModifierLarge;	// 0x00FC
	float		flFlinchVelocityModifierSmall;	// 0x0100
	float		flRange;				// 0x0104
	float		flRangeModifier;		// 0x0108
	float		flThrowVelocity;		// 0x010C
	char		pad_0x010C[12];			// 0x0110
	bool		bHasSilencer;			// 0x011C
	char		pad_0x0119[3];			// 0x011D
	char*		pSilencerModel;			// 0x0120
	int			iCrosshairMinDistance;	// 0x0124
	int			iCrosshairDeltaDistance;// 0x0128 - iTeam?
	float		flMaxPlayerSpeed;		// 0x012C
	float		flMaxPlayerSpeedAlt;	// 0x0130
	float		flSpread;				// 0x0134
	float		flSpreadAlt;			// 0x0138
	float		flInaccuracyCrouch;		// 0x013C
	float		flInaccuracyCrouchAlt;	// 0x0140
	float		flInaccuracyStand;		// 0x0144
	float		flInaccuracyStandAlt;	// 0x0148
	float		flInaccuracyJumpInitial;// 0x014C
	float		flInaccuracyJump;		// 0x0150
	float		flInaccuracyJumpAlt;	// 0x0154
	float		flInaccuracyLand;		// 0x0158
	float		flInaccuracyLandAlt;	// 0x015C
	float		flInaccuracyLadder;		// 0x0160
	float		flInaccuracyLadderAlt;	// 0x0164
	float		flInaccuracyFire;		// 0x0168
	float		flInaccuracyFireAlt;	// 0x016C
	float		flInaccuracyMove;		// 0x0170
	float		flInaccuracyMoveAlt;	// 0x0174
	float		flInaccuracyReload;		// 0x0178
	int			iRecoilSeed;			// 0x017C
	float		flRecoilAngle;			// 0x0180
	float		flRecoilAngleAlt;		// 0x0184
	float		flRecoilAngleVariance;	// 0x0188
	float		flRecoilAngleVarianceAlt;	// 0x018C
	float		flRecoilMagnitude;		// 0x0190
	float		flRecoilMagnitudeAlt;	// 0x0194
	float		flRecoilMagnitudeVariance;	// 0x0198
	float		flRecoilMagnitudeVarianceAlt;	// 0x019C
	float		flRecoveryTimeCrouch;	// 0x01A0
	float		flRecoveryTimeStand;	// 0x01A4
	float		flRecoveryTimeCrouchFinal;	// 0x01A8
	float		flRecoveryTimeStandFinal;	// 0x01AC
	int			iRecoveryTransitionStartBullet;// 0x01B0 
	int			iRecoveryTransitionEndBullet;	// 0x01B4
	bool		bUnzoomAfterShot;		// 0x01B8
	bool		bHideViewModelZoomed;	// 0x01B9
	char		pad_0x01B5[2];			// 0x01BA
	char		iZoomLevels[3];			// 0x01BC
	int			iZoomFOV[2];			// 0x01C0
	float		fZoomTime[3];			// 0x01C4
	char*		szWeaponClass;			// 0x01D4
	float		flAddonScale;			// 0x01D8
	char		pad_0x01DC[4];			// 0x01DC
	char*		szEjectBrassEffect;		// 0x01E0
	char*		szTracerEffect;			// 0x01E4
	int			iTracerFrequency;		// 0x01E8
	int			iTracerFrequencyAlt;	// 0x01EC
	char*		szMuzzleFlashEffect_1stPerson; // 0x01F0
	char		pad_0x01F4[4];			 // 0x01F4
	char*		szMuzzleFlashEffect_3rdPerson; // 0x01F8
	char		pad_0x01FC[4];			// 0x01FC
	char*		szMuzzleSmokeEffect;	// 0x0200
	float		flHeatPerShot;			// 0x0204
	char*		szZoomInSound;			// 0x0208
	char*		szZoomOutSound;			// 0x020C
	float		flInaccuracyPitchShift;	// 0x0210
	float		flInaccuracySoundThreshold;	// 0x0214
	float		flBotAudibleRange;		// 0x0218
	char		pad_0x0218[8];			// 0x0220
	char*		pWrongTeamMsg;			// 0x0224
	bool		bHasBurstMode;			// 0x0228
	char		pad_0x0225[3];			// 0x0229
	bool		bIsRevolver;			// 0x022C
	bool		bCannotShootUnderwater;	// 0x0230
};


enum class CSGOClassID
{
	CAK47 = 1,
	CBaseAnimating = 2,
	CBaseAnimatingOverlay = 3,
	CBaseAttributableItem = 4,
	CBaseButton = 5,
	CBaseCombatCharacter = 6,
	CBaseCombatWeapon = 7,
	CBaseCSGrenade = 8,
	CBaseCSGrenadeProjectile = 9,
	CBaseDoor = 10,
	CBaseEntity = 11,
	CBaseFlex = 12,
	CBaseGrenade = 13,
	CBaseParticleEntity = 14,
	CBasePlayer = 15,
	CBasePropDoor = 16,
	CBaseTeamObjectiveResource = 17,
	CBaseTempEntity = 18,
	CBaseToggle = 19,
	CBaseTrigger = 20,
	CBaseViewModel = 21,
	CBaseVPhysicsTrigger = 22,
	CBaseWeaponWorldModel = 23,
	CBeam = 24,
	CBeamSpotlight = 25,
	CBoneFollower = 26,
	CBreakableProp = 27,
	CBreakableSurface = 28,
	CC4 = 29,
	CCascadeLight = 30,
	CChicken = 31,
	CColorCorrection = 32,
	CColorCorrectionVolume = 33,
	CCSGameRulesProxy = 34,
	CCSPlayer = 35,
	CCSPlayerResource = 36,
	CCSRagdoll = 37,
	CCSTeam = 38,
	CDEagle = 39,
	CDecoyGrenade = 40,
	CDecoyProjectile = 41,
	CDynamicLight = 42,
	CDynamicProp = 43,
	CEconEntity = 44,
	CEconWearable = 45,
	CEmbers = 46,
	CEntityDissolve = 47,
	CEntityFlame = 48,
	CEntityFreezing = 49,
	CEntityParticleTrail = 50,
	CEnvAmbientLight = 51,
	CEnvDetailController = 52,
	CEnvDOFController = 53,
	CEnvParticleScript = 54,
	CEnvProjectedTexture = 55,
	CEnvQuadraticBeam = 56,
	CEnvScreenEffect = 57,
	CEnvScreenOverlay = 58,
	CEnvTonemapController = 59,
	CEnvWind = 60,
	CFEPlayerDecal = 61,
	CFireCrackerBlast = 62,
	CFireSmoke = 63,
	CFireTrail = 64,
	CFish = 65,
	CFlashbang = 66,
	CFogController = 67,
	CFootstepControl = 68,
	CFunc_Dust = 69,
	CFunc_LOD = 70,
	CFuncAreaPortalWindow = 71,
	CFuncBrush = 72,
	CFuncConveyor = 73,
	CFuncLadder = 74,
	CFuncMonitor = 75,
	CFuncMoveLinear = 76,
	CFuncOccluder = 77,
	CFuncReflectiveGlass = 78,
	CFuncRotating = 79,
	CFuncSmokeVolume = 80,
	CFuncTrackTrain = 81,
	CGameRulesProxy = 82,
	CHandleTest = 83,
	CHEGrenade = 84,
	CHostage = 85,
	CHostageCarriableProp = 86,
	CIncendiaryGrenade = 87,
	CInferno = 88,
	CInfoLadderDismount = 89,
	CInfoOverlayAccessor = 90,
	CItem_Healthshot = 91,
	CItemDogtags = 92,
	CKnife = 93,
	CKnifeGG = 94,
	CLightGlow = 95,
	CMaterialModifyControl = 96,
	CMolotovGrenade = 97,
	CMolotovProjectile = 98,
	CMovieDisplay = 99,
	CParticleFire = 100,
	CParticlePerformanceMonitor = 101,
	CParticleSystem = 102,
	CPhysBox = 103,
	CPhysBoxMultiplayer = 104,
	CPhysicsProp = 105,
	CPhysicsPropMultiplayer = 106,
	CPhysMagnet = 107,
	CPlantedC4 = 108,
	CPlasma = 109,
	CPlayerResource = 110,
	CPointCamera = 111,
	CPointCommentaryNode = 112,
	CPointWorldText = 113,
	CPoseController = 114,
	CPostProcessController = 115,
	CPrecipitation = 116,
	CPrecipitationBlocker = 117,
	CPredictedViewModel = 118,
	CProp_Hallucination = 119,
	CPropDoorRotating = 120,
	CPropJeep = 121,
	CPropVehicleDriveable = 122,
	CRagdollManager = 123,
	CRagdollProp = 124,
	CRagdollPropAttached = 125,
	CRopeKeyframe = 126,
	CSCAR17 = 127,
	CSceneEntity = 128,
	CSensorGrenade = 129,
	CSensorGrenadeProjectile = 130,
	CShadowControl = 131,
	CSlideshowDisplay = 132,
	CSmokeGrenade = 133,
	CSmokeGrenadeProjectile = 134,
	CSmokeStack = 135,
	CSpatialEntity = 136,
	CSpotlightEnd = 137,
	CSprite = 138,
	CSpriteOriented = 139,
	CSpriteTrail = 140,
	CStatueProp = 141,
	CSteamJet = 142,
	CSun = 143,
	CSunlightShadowControl = 144,
	CTeam = 145,
	CTeamplayRoundBasedRulesProxy = 146,
	CTEArmorRicochet = 147,
	CTEBaseBeam = 148,
	CTEBeamEntPoint = 149,
	CTEBeamEnts = 150,
	CTEBeamFollow = 151,
	CTEBeamLaser = 152,
	CTEBeamPoints = 153,
	CTEBeamRing = 154,
	CTEBeamRingPoint = 155,
	CTEBeamSpline = 156,
	CTEBloodSprite = 157,
	CTEBloodStream = 158,
	CTEBreakModel = 159,
	CTEBSPDecal = 160,
	CTEBubbles = 161,
	CTEBubbleTrail = 162,
	CTEClientProjectile = 163,
	CTEDecal = 164,
	CTEDust = 165,
	CTEDynamicLight = 166,
	CTEEffectDispatch = 167,
	CTEEnergySplash = 168,
	CTEExplosion = 169,
	CTEFireBullets = 170,
	CTEFizz = 171,
	CTEFootprintDecal = 172,
	CTEFoundryHelpers = 173,
	CTEGaussExplosion = 174,
	CTEGlowSprite = 175,
	CTEImpact = 176,
	CTEKillPlayerAttachments = 177,
	CTELargeFunnel = 178,
	CTEMetalSparks = 179,
	CTEMuzzleFlash = 180,
	CTEParticleSystem = 181,
	CTEPhysicsProp = 182,
	CTEPlantBomb = 183,
	CTEPlayerAnimEvent = 184,
	CTEPlayerDecal = 185,
	CTEProjectedDecal = 186,
	CTERadioIcon = 187,
	CTEShatterSurface = 188,
	CTEShowLine = 189,
	CTesla = 190,
	CTESmoke = 191,
	CTESparks = 192,
	CTESprite = 193,
	CTESpriteSpray = 194,
	CTest_ProxyToggle_Networkable = 194,
	CTestTraceline = 196,
	CTEWorldDecal = 197,
	CTriggerPlayerMovement = 198,
	CTriggerSoundOperator = 199,
	CVGuiScreen = 200,
	CVoteController = 201,
	CWaterBullet = 202,
	CWaterLODControl = 203,
	CWeaponAug = 204,
	CWeaponAWP = 205,
	CWeaponBaseItem = 206,
	CWeaponBizon = 207,
	CWeaponCSBase = 208,
	CWeaponCSBaseGun = 209,
	CWeaponCycler = 210,
	CWeaponElite = 211,
	CWeaponFamas = 212,
	CWeaponFiveSeven = 213,
	CWeaponG3SG1 = 214,
	CWeaponGalil = 215,
	CWeaponGalilAR = 216,
	CWeaponGlock = 217,
	CWeaponHKP2000 = 218,
	CWeaponM249 = 219,
	CWeaponM3 = 220,
	CWeaponM4A1 = 221,
	CWeaponMAC10 = 222,
	CWeaponMag7 = 223,
	CWeaponMP5Navy = 224,
	CWeaponMP7 = 225,
	CWeaponMP9 = 226,
	CWeaponNegev = 227,
	CWeaponNOVA = 228,
	CWeaponP228 = 229,
	CWeaponP250 = 230,
	CWeaponP90 = 231,
	CWeaponSawedoff = 232,
	CWeaponSCAR20 = 233,
	CWeaponScout = 234,
	CWeaponSG550 = 235,
	CWeaponSG552 = 236,
	CWeaponSG556 = 237,
	CWeaponSSG08 = 238,
	CWeaponTaser = 239,
	CWeaponTec9 = 240,
	CWeaponTMP = 241,
	CWeaponUMP45 = 242,
	CWeaponUSP = 243,
	CWeaponXM1014 = 244,
	CWorld = 245,
	DustTrail = 246,
	MovieExplosion = 247,
	ParticleSmokeGrenade = 248,
	RocketTrail = 249,
	SmokeTrail = 250,
	SporeExplosion = 251,
	SporeTrail = 252
};

enum class CSGOHitboxID
{
	Head = 0,
	Neck,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm,
	Max,
};

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

class ScriptCreatedItem
{
public:
	CPNETVAR_FUNC(int*, ItemDefinitionIndex, 0xE67AB3B8); //m_iItemDefinitionIndex
	CPNETVAR_FUNC(int*, AccountID, 0x24abbea8); //m_iAccountID
	CPNETVAR_FUNC(int*, ItemIDHigh, 0x714778A); //m_iItemIDHigh
	CPNETVAR_FUNC(int*, ItemIDLow, 0x3A3DFC74); //m_iItemIDLow
	CPNETVAR_FUNC(char**, szCustomName, 0x13285ad9); //m_szCustomName
	CPNETVAR_FUNC(int*, EntityQuality, 0x110be6fe); //m_iEntityQuality
};

class AttributeContainer
{
public:
	CPNETVAR_FUNC(ScriptCreatedItem*, m_Item, 0x7E029CE5);
};

class CCacheAngle {
public:
	//Menu
	bool bCustomSetts = false;
	bool bCorrect = true;
	int iResolverType = 0;
	bool bPrioritize = false;
	int iHitbox = 1;
	bool bAutoBaim = false;
	int iAutoBaimAferShot = 2;
	int iHitboxAutoBaim = 4;
	bool bPrioritizeVis = true;
	float flMinDmg = 5.f;
	bool bInterpLagComp = false;

	//GlobalPlayer
	bool bFlip = false;
};

enum OffsetIndex
{
	OFFSETINDEX_GLOCK,
	OFFSETINDEX_FAMAS,
	OFFSETINDEX_M4A1,
	OFFSETINDEX_USP,
	OFFSETINDECES_MAX
};

enum ItemDefinitionIndex
{
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_USP = 6,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE_CT = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};

class CBaseCombatWeapon
{
	template <typename T>
	__forceinline T Member(void *base, DWORD offset = 0)
	{
		return (T)((DWORD)base + offset);
	}
public:
	CNETVAR_FUNC(float, GetNextPrimaryAttack, 0xDB7B106E); //m_flNextPrimaryAttack
	CNETVAR_FUNC(int, GetAmmoInClip, 0x97B6F70C); //m_iClip1
	CNETVAR_FUNC(HANDLE, GetOwnerHandle, 0xC32DF98D); //m_hOwner
	CNETVAR_FUNC(Vector, GetOrigin, 0x1231CE10); //m_vecOrigin
	CNETVAR_FUNC(int, GetZoomLevel, 0x26553F1A);
	CPNETVAR_FUNC(AttributeContainer*, m_AttributeManager, 0xCFFCE089);

	CPNETVAR_FUNC(int*, FallbackPaintKit, 0xADE4C870); // m_nFallbackPaintKit
	CPNETVAR_FUNC(int*, FallbackSeed, 0xC2D0683D); // m_nFallbackSeed
	CPNETVAR_FUNC(float*, FallbackWear, 0xA263576C); //m_flFallbackWear
	CPNETVAR_FUNC(int*, FallbackStatTrak, 0x1ED78768); //m_nFallbackStatTrak
	CPNETVAR_FUNC(int*, OriginalOwnerXuidLow, 0xd7bd6be2); //m_OriginalOwnerXuidLow
	CPNETVAR_FUNC(int*, OriginalOwnerXuidHigh, 0x1e95c16c); //m_OriginalOwnerXuidHigh
	CPNETVAR_FUNC(int*, ViewModelIndex, 0x7F7C89C1);
	CPNETVAR_FUNC(int*, OwnerXuidLow, 0xAD8D897F);
	CPNETVAR_FUNC(int*, OwnerXuidHigh, 0x90511E77);
	CPNETVAR_FUNC(int*, ModelIndex, 0x27016F83);
	CPNETVAR_FUNC(int*, WorldModelIndex, 0x4D8AD9F3);

	int GetWeaponId(void)
	{
		return ItemDefinitionIndex();
	}

	bool ChanceToHit(float flChance)
	{
		if (flChance >= 99.f)
			flChance = 99.f;

		if (flChance < 1)
			flChance = 1;

		float flSpread = this->GetInaccuracy() * 10;
		return ((((100.f - flChance) * 0.65f) * 0.01125) >= flSpread);
	}

	int GetBurstShotsRemaining(int offsetIndex)
	{
		static int offsets[OFFSETINDECES_MAX] = { 0, 0 };

		if (!offsets[OFFSETINDEX_GLOCK]
			|| !offsets[OFFSETINDEX_FAMAS])
		{
			offsets[OFFSETINDEX_GLOCK] = U::FindNet("DT_WeaponGlock", "m_iBurstShotsRemaining");
			offsets[OFFSETINDEX_FAMAS] = U::FindNet("DT_WeaponFamas", "m_iBurstShotsRemaining");
		}


		return *Member<int*>(this, offsets[offsetIndex]);
	}
	bool HasBurstShotsRemaining(int offsetIndex)
	{
		return (GetBurstShotsRemaining(offsetIndex) > 0);
	}
	bool IsSpecialMode(int offsetIndex)
	{
		static int offsets[OFFSETINDECES_MAX] = { 0, 0 };

		if (!offsets[OFFSETINDEX_GLOCK]
			|| !offsets[OFFSETINDEX_FAMAS]
			|| !offsets[OFFSETINDEX_M4A1]
			|| !offsets[OFFSETINDEX_USP])
		{
			offsets[OFFSETINDEX_GLOCK] = U::FindNet("DT_WeaponGlock", "m_bBurstMode");
			offsets[OFFSETINDEX_FAMAS] = U::FindNet("DT_WeaponFamas", "m_bBurstMode");
			offsets[OFFSETINDEX_M4A1] = U::FindNet("DT_WeaponM4A1", "m_bSilencerOn");
			offsets[OFFSETINDEX_USP] = U::FindNet("DT_WeaponUSP", "m_bSilencerOn");
		}


		return *Member<bool*>(this, offsets[offsetIndex]);
	}

	float GetInaccuracy()
	{
		typedef float(__thiscall* oInaccuracy)(PVOID);
		return call_vfunc< oInaccuracy >(this, 468)(this);
	}
	float GetSpread()
	{
		typedef float(__thiscall* oWeaponSpread)(PVOID);
		return call_vfunc< oWeaponSpread >(this, 437)(this);
	}
	void UpdateAccPenalty()
	{
		typedef void(__thiscall *OrigFn)(void *);
		return call_vfunc<OrigFn>(this, 469)(this);
	}
	void PreDataUpdate(int updateType)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
	}
	void SetModelIndex(int modelIndex) {
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(this, 75)(this, modelIndex);
	}
	int GetId()
	{
		typedef int(__thiscall* tGetId)(void*);
		return call_vfunc<tGetId>(this, 456)(this);
	}
	bool IsScoped(int x = 0)
	{
		return GetZoomLevel() > 0;
	}
	bool HitChance(float flChance)
	{
		if (flChance >= 99.f)
			flChance = 99.f;

		if (flChance < 1)
			flChance = 1;

		float flSpread = GetSpread();
		return((((100.f - flChance) * 0.65f) * 0.01125) >= flSpread);
	}

	ScriptCreatedItem* GetItem()
	{
		return m_AttributeManager()->m_Item();
	}
	int* GetItemDefinitionIndex()
	{
		return (int*)m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	}
	int* GetAccountID()
	{
		return (int*)m_AttributeManager()->m_Item()->AccountID();
	}
	int* GetItemIDHigh()
	{
		return (int*)m_AttributeManager()->m_Item()->ItemIDHigh();
	}
	int* GetItemIDLow()
	{
		return (int*)m_AttributeManager()->m_Item()->ItemIDLow();
	}
	char** GetCustomName()
	{
		return (char**)m_AttributeManager()->m_Item()->szCustomName();
	}
	int* GetEntityQuality()
	{
		return (int*)m_AttributeManager()->m_Item()->EntityQuality();
	}

	CSWeaponInfo * CBaseCombatWeapon::GetCSWpnData()
	{
		typedef CSWeaponInfo*(__thiscall* OriginalFn)(void*);
		return  call_vfunc<OriginalFn>(this, 445)(this);
	}
	bool IsKnife()
	{
		int* Index = GetItemDefinitionIndex();
		if (*Index == 42 || *Index == 59 || *Index >= 500)
			return true;
		return false;
	}
	bool IsGrenade()
	{ 
		int* Index = GetItemDefinitionIndex();
		if (*Index == 43 || *Index == 44 || *Index == 45 || *Index == 46 || *Index == 47 || *Index == 48)
			return true;
		return false;
	}
	bool IsC4()
	{
		int* Index = GetItemDefinitionIndex();
		return *Index == 49;
	}
	char* GetGunIcon()
	{
		int WeaponId = *this->GetItemDefinitionIndex();
		switch (WeaponId)
		{
		case WEAPON_KNIFE_CT:
		case WEAPON_KNIFE_T:
		case 500:
		case 505:
		case 506:
		case 507:
		case 508:
		case 509:
		case 512:
		case 514:
		case 515:
		case 516:
			return "]";
		case WEAPON_DEAGLE:
			return "A";
		case WEAPON_ELITE:
			return "B";
		case WEAPON_FIVESEVEN:
			return "C";
		case WEAPON_GLOCK:
			return "D";
		case WEAPON_HKP2000:
			return "E";
		case WEAPON_P250:
			return "F";
		case WEAPON_USP_SILENCER:
			return "G";
		case WEAPON_TEC9:
			return "H";
		case WEAPON_CZ75A:
			return "I";
		case WEAPON_REVOLVER:
			return "J";
		case WEAPON_MAC10:
			return "K";
		case WEAPON_UMP45:
			return "L";
		case WEAPON_BIZON:
			return "M";
		case WEAPON_MP7:
			return "N";
		case WEAPON_MP9:
			return "O";
		case WEAPON_P90:
			return "P";
		case WEAPON_GALILAR:
			return "Q";
		case WEAPON_FAMAS:
			return "R";
		case WEAPON_M4A1_SILENCER:
			return "S";
		case WEAPON_M4A1:
			return "T";
		case WEAPON_AUG:
			return "U";
		case WEAPON_SG556:
			return "V";
		case WEAPON_AK47:
			return "W";
		case WEAPON_G3SG1:
			return "X";
		case WEAPON_SCAR20:
			return "Y";
		case WEAPON_AWP:
			return "Z";
		case WEAPON_SSG08:
			return "a";
		case WEAPON_XM1014:
			return "b";
		case WEAPON_SAWEDOFF:
			return "c";
		case WEAPON_MAG7:
			return "d";
		case WEAPON_NOVA:
			return "e";
		case WEAPON_NEGEV:
			return "f";
		case WEAPON_M249:
			return "g";
		case WEAPON_TASER:
			return "h";
		case WEAPON_FLASHBANG:
			return "i";
		case WEAPON_HEGRENADE:
			return "j";
		case WEAPON_SMOKEGRENADE:
			return "k";
		case WEAPON_MOLOTOV:
			return "l";
		case WEAPON_DECOY:
			return "m";
		case WEAPON_INCGRENADE:
			return "n";
		case WEAPON_C4:
			return "o";
		default:
			return " ";
		}
	}
};

class CCSBomb
{
public:
	CNETVAR_FUNC(HANDLE, GetOwnerHandle, 0xC32DF98D); //m_hOwner 0x29BC
	CNETVAR_FUNC(float, GetC4BlowTime, 0xB5E0CA1C); //m_flC4Blow
	CNETVAR_FUNC(float, GetC4DefuseCountDown, 0xB959B4A6); //m_flDefuseCountDown
};

class CLocalPlayerExclusive
{
public:
	CNETVAR_FUNC(Vector, GetViewPunchAngle, 0x68F014C0);//m_viewPunchAngle
	CNETVAR_FUNC(Vector, GetAimPunchAngle, 0xBF25C290);//m_aimPunchAngle
	CNETVAR_FUNC(Vector, GetAimPunchAngleVel, 0x8425E045);//m_aimPunchAngleVel
};

class CollisionProperty
{
public:
	CNETVAR_FUNC(Vector, GetMins, 0xF6F78BAB);//m_vecMins
	CNETVAR_FUNC(Vector, GetMaxs, 0xE47C6FC4);//m_vecMaxs
	CNETVAR_FUNC(unsigned char, GetSolidType, 0xB86722A1);//m_nSolidType
	CNETVAR_FUNC(unsigned short, GetSolidFlags, 0x63BB24C1);//m_usSolidFlags
	CNETVAR_FUNC(int, GetSurroundsType, 0xB677A0BB); //m_nSurroundType

	bool IsSolid()
	{
		return (GetSolidType() != SOLID_NONE) && ((GetSolidFlags() & FSOLID_NOT_SOLID) == 0);
	}
};

class IClientRenderable
{
public:
	//virtual void*					GetIClientUnknown() = 0;
	virtual Vector const&			GetRenderOrigin(void) = 0;
	virtual Vector const&			GetRenderAngles(void) = 0;
	virtual bool					ShouldDraw(void) = 0;
	virtual bool					IsTransparent(void) = 0;
	virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool					UsesFullFrameBufferTexture() = 0;
	virtual void					GetShadowHandle() const = 0;
	virtual void*					RenderHandle() = 0;
	virtual const model_t*				GetModel() const = 0;
	virtual int						DrawModel(int flags) = 0;
	virtual int						GetBody() = 0;
	virtual void					ComputeFxBlend() = 0;

	bool SetupBones(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		typedef bool(__thiscall* oSetupBones)(PVOID, matrix3x4*, int, int, float);
		return call_vfunc< oSetupBones>(this, 13)(this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}


};

class IClientNetworkable
{
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual ClientClass*	GetClientClass() = 0;// FOR NETVARS FIND YOURSELF ClientClass* stuffs
	virtual void			NotifyShouldTransmit( /* ShouldTransmitState_t state*/) = 0;
	virtual void			OnPreDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			OnDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PreDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PostDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			unknown();
	virtual bool			IsDormant(void) = 0;
	virtual int				GetIndex(void) const = 0;
	virtual void			ReceiveMessage(int classID /*, bf_read &msg*/) = 0;
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities(void) = 0;
};

class IClientUnknown
{
public:
	virtual void*		GetCollideable() = 0;
	virtual IClientNetworkable*	GetClientNetworkable() = 0;
	virtual IClientRenderable*	GetClientRenderable() = 0;
	virtual IClientEntity*		GetIClientEntity() = 0;
	virtual IClientEntity*		GetBaseEntity() = 0;
	virtual IClientThinkable*	GetClientThinkable() = 0;
};

class IClientThinkable
{
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void*				GetThinkHandle() = 0;
	virtual void				SetThinkHandle(void* hThink) = 0;
	virtual void				Release() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
	template<class T>
	inline T GetFieldValue(int offset) {
		return *(T*)((DWORD)this + offset);
	}
	template<class T>
	T* GetFieldPointer(int offset) {
		return (T*)((DWORD)this + offset);
	}
public:
	Vector GetPredicted(Vector p0);

	Vector& GetAbsAngles(void)
	{
		VirtualFn(Vector&)(PVOID);
		return Virtual::CallVirtual< OriginalFn >(this, 11)(this);
	}

	//---                 NetVars                  ---//

	CPNETVAR_FUNC(CLocalPlayerExclusive*, localPlayerExclusive, 0x7177BC3E);// m_Local
	CPNETVAR_FUNC(CollisionProperty*, collisionProperty, 0xE477CBD0);//m_Collision
	CNETVAR_FUNC(HANDLE, GetActiveWeaponHandle, 0xB4FECDA3); //m_hActiveWeapon
	CNETVAR_FUNC(int, GetMoney, 0xF4B3E183); //m_iAccount

	bool InBuyZone() {
		static int m_bInBuyZone = GET_NETVAR(XorStr("DT_CSPlayer"), XorStr("m_bInBuyZone"));
		return GetFieldValue<bool>(m_bInBuyZone);
	}


	bool isMoving() {
		return GetVelocity().Length() > 0.1f;
	}


	float* GetLowerBodyYaw() // m_flLowerBodyYawTarget
	{
		return (float*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget"));
	}

	float GetSimulationTime() // m_flSimulationTime
	{
		return *(float*)((DWORD)this + GET_NETVAR("DT_BaseEntity", "m_flSimulationTime"));
	}

	bool IsScoped() // m_bIsScoped
	{
		return *(bool*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_bIsScoped"));
	}

	bool HasHelmet() // m_bHasHelmet
	{
		return *(bool*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_bHasHelmet"));
	}

	bool HasGunGameImmunity() // m_bGunGameImmunity
	{
		return *(bool*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_bGunGameImmunity"));
	}

	int ArmorValue() // m_ArmorValue
	{
		return *(int*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_ArmorValue"));
	}

	int GetTickBase() // m_nTickBase
	{
		return *(int*)((DWORD)this + GET_NETVAR("DT_BasePlayer", "localdata", "m_nTickBase"));
	}

	int* GetFlags() // m_nTickBase
	{
		return (int*)((DWORD)this + GET_NETVAR("DT_BasePlayer", "m_fFlags"));
	}

	int GetTeamNum() // m_nTickBase
	{
		return *(int*)((DWORD)this + GET_NETVAR("DT_BaseEntity", "m_iTeamNum"));
	}

	int GetHealth() // m_iHealth
	{
		return *(int*)((DWORD)this + GET_NETVAR("DT_BasePlayer", "m_iHealth"));
	}

	Vector GetVelocity() // m_vecVelocity[0]
	{
		return *(Vector*)((DWORD)this + GET_NETVAR("DT_BasePlayer", "localdata", "m_vecVelocity[0]"));
	}

	float* GetFallVelocity() // m_flFallVelocity
	{
		return (float*)((DWORD)this + GET_NETVAR("DT_BasePlayer", "localdata", "m_Local", "m_flFallVelocity"));
	}

	Vector GetBaseVelocity() //m_vecBaseVelocity
	{
		return *(Vector*)((DWORD)this + GET_NETVAR("DT_BasePlayer", "localdata", "m_vecBaseVelocity"));
	}

	Vector GetViewOffset() // m_vecViewOffset[0]
	{
		return *(Vector*)((DWORD)this + GET_NETVAR("DT_BasePlayer", "localdata", "m_vecViewOffset[0]"));
	}

	Vector GetOrigin() // m_vecOrigin
	{
		return *(Vector*)((DWORD)this + GET_NETVAR("DT_BaseEntity", "m_vecOrigin"));
	}

	bool GetLifeState() // m_lifeState
	{
		return *(bool*)((DWORD)this + GET_NETVAR("DT_BasePlayer", "m_lifeState"));
	}

	CPNETVAR_FUNC(Vector*, GetOriginPtr, 0x1231CE10); //m_vecOrigin

	

	Vector* GetEyeAngles();
	Vector* GetHeadRotation();
	bool IsValidESP();
	bool IsValidPlayer();
	CBaseCombatWeapon* GetWeapon();
	int GetObserverMode();
	char* GetLastPlaceName();
	bool IsTargetingLocal();
	Vector GetBonePos(int i);
	int GetChokedTicks();
	int GetMoveType();
	float GetMaxSpeed();
	bool IsInAir();
	bool IsOnGround();
	Vector GetVecOrigin();

	Vector GetAbsOrigin() {
		__asm {
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}

	Vector GetEyePosition(void)
	{
		return GetAbsOrigin() + GetViewOffset();
	}

	inline UINT* GetWeapons() {
		return (UINT*)((DWORD)this + 0x2DE8);
	}
	bool IsPlayer()
	{
		if (!this)
			return false;

		if (this->IsDormant())
			return false;

		if (this->GetTeamNum() == 0)
			return false;

		ClientClass* cClass = (ClientClass*)this->GetClientClass();

		if (cClass->m_ClassID != 35)
		{
			return false;
		}
		return true;
	}

	CNETVAR_FUNC(HANDLE, GetObserverTargetHandle, 0x8660FD83); //m_hObserverTarget
	inline float getnextattack() { return nigvar(float, this, 0x2D60); }
	// ----------------------------------------------//
	bool IsAlive()
	{
		return (GetLifeState() == LIFE_ALIVE && GetHealth() > 0);
	}

	Vector GetHeadPos()
	{
		return this->GetBonePos(8);
	}
};

class C_PlantedC4 : public IClientEntity
{
public:
	bool IsBombTicking()
	{
		return (bool)((uintptr_t)this + U::FindNet("DT_PlantedC4", "m_bBombTicking"));
	}

	float GetBombTime()
	{
		return *(float*)((uintptr_t)this + U::FindNet("DT_PlantedC4", "m_flC4Blow"));
	}

	bool IsBombDefused()
	{
		return *(bool*)((uintptr_t)this + U::FindNet("DT_PlantedC4", "m_bBombDefused"));
	}

	int GetBombDefuser()
	{
		return *(int*)((uintptr_t)this + U::FindNet("DT_PlantedC4", "m_hBombDefuser"));
	}
};

class CBaseViewModel : public IClientUnknown, public IClientRenderable, public IClientNetworkable {
public:
	inline int GetModelIndex() {
		// DT_BaseViewModel -> m_nModelIndex
		return *(int*)((DWORD)this + 0x254);
	}
	inline DWORD GetOwner() {
		// DT_BaseViewModel -> m_hOwner
		return *(PDWORD)((DWORD)this + 0x29BC);
	}
	inline DWORD GetWeapon() {
		// DT_BaseViewModel -> m_hWeapon
		return *(PDWORD)((DWORD)this + 0x29B8);
	}
	inline void SetWeaponModel(const char* Filename, IClientUnknown* Weapon) {
		return call_vfunc<void(__thiscall*)(void*, const char*, IClientUnknown*)>(this, 242)(this, Filename, Weapon);
	}
};