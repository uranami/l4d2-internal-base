#pragma once

struct RecvProxyData;
struct RecvTable;
struct RecvProp;

using RecvVarProxy_t = void(__cdecl*)(const RecvProxyData*, void*, void*);

struct DVariant
{
    union
    {
        float m_Float;
        long m_Int;
        char* m_pString;
        void* m_pData;
        float m_Vector[3];
        __int64 m_Int64;
    };

    int m_Type;
};

struct RecvProxyData
{
    const RecvProp* m_pRecvProp;
    DVariant m_Value;
    int m_iElement;
    int m_ObjectID;
};

struct RecvTable
{
    RecvProp* m_pProps;
    int	 m_nProps;
    void* m_pDecoder;
    char* m_pNetTableName;
    bool m_bInitialized;
    bool m_bInMainList;
};

struct RecvProp
{
    char* m_pVarName;
    int m_RecvType;
    int m_Flags;
    int m_StringBufferSize;
    bool m_bInsideArray;
    const void* m_pExtraData;
    RecvProp* m_pArrayProp;
    void* m_ArrayLengthProxy;
    void* m_ProxyFn;
    void* m_DataTableProxyFn;
    RecvTable* m_pDataTable;
    int m_Offset;
    int m_ElementStride;
    int m_nElements;
    const char* m_pParentArrayPropName;
};

using CreateClientClass_t = void* (*)(int entnum, int serialNum);
using CreateEvent_t = void* (*)();

class ClientClass_t
{
public:
	void* m_pCreateFn;
	void* m_pCreateEventFn;
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
    ClientClass_t* m_pNext;
	int				m_ClassID;
};

enum ClassID
{
    CFlare = 0,
    CAI_BaseNPC = 0,
    CFuncMonitor = 0,
    Boomer = 0,
    CPropJeep = 0,
    CPlantedC4 = 0,
    CWeaponCycler = 0,
    CWeaponCubemap = 0,
    CPropVehicleChoreoGeneric = 0,
    CTeamTrainWatcher = 0,
    CAssaultRifle,
    CAutoShotgun,
    CBaseAbility,
    CBaseAnimating,
    CBaseAnimatingOverlay,
    CBaseAutoShotgun,
    CBaseBackpackItem,
    CBaseBeltItem,
    CBaseButton,
    CBaseCombatCharacter,
    CBaseCombatWeapon,
    CBaseCSGrenade,
    CBaseCSGrenadeProjectile,
    CBaseDoor,
    CBaseEntity,
    CBaseFlex,
    CBaseGrenade,
    CBaseMountedWeapon,
    CBaseParticleEntity,
    CBasePlayer,
    CBasePropDoor,
    CBaseRifle,
    CBaseShotgun,
    CBaseSniperRifle,
    CBaseTeamObjectiveResource,
    CBaseTempEntity,
    CBaseToggle,
    CBaseTrigger,
    CBaseUpgradeItem,
    CBaseViewModel,
    CBeam,
    CBeamSpotlight,
    CBoneFollower,
    CBoomerClaw,
    CBreakable,
    CBreakableProp,
    CBreakableSurface,
    CButtonTimed,
    CChainsaw,
    CCharge,
    CChargerClaw,
    CClaw,
    CClientPath,
    CColaBottles,
    CColorCorrection,
    CColorCorrectionVolume,
    CCSGameRulesProxy,
    CCSPlayer,
    CCSRagdoll,
    CCSTeam,
    CDynamicLight,
    CDynamicProp,
    CEmbers,
    CEntityDissolve,
    CEntityFlame,
    CEntityParticleTrail,
    CEnvDetailController,
    CEnvDOFController,
    CEnvParticleScript,
    CEnvPhysicsBlocker,
    CEnvProjectedTexture,
    CEnvQuadraticBeam,
    CEnvScreenEffect,
    CEnvScreenOverlay,
    CEnvTonemapController,
    CEnvWeaponFire,
    CEnvWind,
    CFinaleTrigger,
    CFireCrackerBlast,
    CFireSmoke,
    CFireTrail,
    CFireworkCrate,
    CFirstAidKit,
    CFish,
    CFogController,
    CFunc_Dust,
    CFunc_LOD,
    CFuncAreaPortalWindow,
    CFuncBrush,
    CFuncConveyor,
    CFuncElevator,
    CFuncLadder,
    CFuncMoveLinear,
    CFuncOccluder,
    CFuncPlayerGhostInfectedClip,
    CFuncPlayerInfectedClip,
    CFuncRagdollFader,
    CFuncReflectiveGlass,
    CFuncRotating,
    CFuncSimpleLadder,
    CFuncSmokeVolume,
    CFuncTrackTrain,
    CGameRulesProxy,
    CGasCan,
    CGnome,
    CGrenadeLauncher,
    CGrenadeLauncher_Projectile,
    CHandleTest,
    Charger,
    CHunterClaw,
    CInferno,
    CInfoLadderDismount,
    CInfoOverlayAccessor,
    CInsectSwarm,
    CItem_Adrenaline,
    CItem_VomitJar,
    CItemAmmoPack,
    CItemBaseUpgradePack,
    CItemDefibrillator,
    CItemUpgradePackExplosive,
    CItemUpgradePackIncendiary,
    CJockeyClaw,
    CLeap,
    CLightGlow,
    CLunge,
    CMagnumPistol,
    CMaterialModifyControl,
    CMolotov,
    CMolotovProjectile,
    COxygenTank,
    CPainPills,
    CParticleFire,
    CParticlePerformanceMonitor,
    CParticleSystem,
    CPhysBox,
    CPhysBoxMultiplayer,
    CPhysicsProp,
    CPhysMagnet,
    CPipeBomb,
    CPipeBombProjectile,
    CPistol,
    CPlasma,
    CPlayerResource,
    CPointCommentaryNode,
    CPointPropUseTarget,
    CPointScriptUseTarget,
    CPoseController,
    CPostProcessController,
    CPrecipitation,
    CPrecipitationBlocker,
    CPredictedViewModel,
    CPropaneTank,
    CPropDoorRotatingCheckpoint,
    CPropHealthCabinet,
    CPropMinigun,
    CPropMountedGun,
    CPropVehicleDriveable,
    CPumpShotgun,
    CRagdollManager,
    CRagdollProp,
    CRagdollPropAttached,
    CRifle_AK47,
    CRifle_Desert,
    CRifle_M60,
    CRifle_SG552,
    CRopeKeyframe,
    CScavengeProgressDisplay,
    CSceneEntity,
    CScriptBaseButton,
    CSelfDestruct,
    CShadowControl,
    CShotgun_Chrome,
    CShotgun_SPAS,
    CSlideshowDisplay,
    CSMG_MP5,
    CSMG_Silenced,
    CSmokerClaw,
    CSmokeStack,
    CSniper_AWP,
    CSniper_Military,
    CSniper_Scout,
    CSniperRifle,
    CSoundMixLayer,
    CSpitAbility,
    CSpitterClaw,
    CSpitterProjectile,
    CSpotlightEnd,
    CSprite,
    CSpriteOriented,
    CSpriteTrail,
    CSteamJet,
    CSubMachinegun,
    CSun,
    CSurvivorDeathModel,
    CSurvivorPosition,
    CSurvivorRescue,
    CTankClaw,
    CTeam,
    CTeamplayRoundBasedRulesProxy,
    CTEArmorRicochet,
    CTEBaseBeam,
    CTEBeamEntPoint,
    CTEBeamEnts,
    CTEBeamFollow,
    CTEBeamLaser,
    CTEBeamPoints,
    CTEBeamRing,
    CTEBeamRingPoint,
    CTEBeamSpline,
    CTEBloodSprite,
    CTEBloodStream,
    CTEBreakModel,
    CTEBSPDecal,
    CTEBubbles,
    CTEBubbleTrail,
    CTEBullets,
    CTEClientProjectile,
    CTEDecal,
    CTEDust,
    CTEDynamicLight,
    CTEEffectDispatch,
    CTEEnergySplash,
    CTEExplosion,
    CTEFizz,
    CTEFootprintDecal,
    CTEFoundryHelpers,
    CTEGaussExplosion,
    CTEGlowSprite,
    CTEImpact,
    CTEKillPlayerAttachments,
    CTELargeFunnel,
    CTEMetalSparks,
    CTEMuzzleFlash,
    CTEParticleSystem,
    CTEPhysicsProp,
    CTEPlayerAnimEvent,
    CTEPlayerDecal,
    CTEProjectedDecal,
    CTerrorGameRulesProxy,
    CTerrorGun,
    CTerrorMeleeWeapon,
    CTerrorPlayer,
    CTerrorPlayerResource,
    CTerrorViewModel,
    CTerrorWeapon,
    CTEShatterSurface,
    CTEShowLine,
    CTESmoke,
    CTESparks,
    CTESprite,
    CTESpriteSpray,
    CTest_ProxyToggle_Networkable,
    CTestTraceline,
    CTEWorldDecal,
    CThrow,
    CTongue,
    CTransitioningPlayer,
    CTriggerPlayerMovement,
    CTutorLesson,
    CVGuiScreen,
    CVomit,
    CVomitJarProjectile,
    CVoteController,
    CWaterBullet,
    CWaterLODControl,
    CWeaponAmmoSpawn,
    CWeaponCSBase,
    CWeaponCSBaseGun,
    CWeaponScavengeItemSpawn,
    CWeaponSpawn,
    CWorld,
    DustTrail,
    Hunter,
    Infected,
    Jockey,
    MovieExplosion,
    NextBotCombatCharacter,
    ParticleSmokeGrenade,
    RocketTrail,
    Smoker,
    SmokeTrail,
    Spitter,
    SporeExplosion,
    SporeTrail,
    SurvivorBot,
    Tank,
    Witch,
};


enum ClientFrameStage_t
{
    FRAME_UNDEFINED = -1,
    FRAME_START,
    FRAME_NET_UPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    FRAME_NET_UPDATE_END,
    FRAME_RENDER_START,
    FRAME_RENDER_END
};

struct IBaseClientDll_t
{
    auto GetAllClasses()
    {
        return Hikari::Memory->VirtualCall<ClientClass_t*, 7>(this);
    }
};
