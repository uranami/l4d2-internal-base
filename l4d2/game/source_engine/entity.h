#pragma once

struct IClientRenderable_t
{
    auto SetupBones( matrix3x4_t* pBoneToWorldOut, int MaxBones, int BoneMask, float CurrentTime )
    {
        return Hikari::Memory->VirtualCall<bool, 13>(this, pBoneToWorldOut, MaxBones, BoneMask, CurrentTime);
    }

    auto GetModel()
    {
        return Hikari::Memory->VirtualCall<const void*, 11>( this );
    }
};

struct IClientNetworkable_t
{
    auto IsDormant()
    {
        return Hikari::Memory->VirtualCall<bool, 4>( this );
    }

    auto GetClientClass()
    {
        return Hikari::Memory->VirtualCall<ClientClass_t*, 1>( this );
    }
};

struct CBaseEntity_t
{
    IClientRenderable_t* GetRenderable()
    {
        return reinterpret_cast<IClientRenderable_t*>( reinterpret_cast<std::uintptr_t>( this ) + sizeof( void* ) );
    }

    IClientNetworkable_t* GetNetworkable()
    {
        return reinterpret_cast<IClientNetworkable_t*>( reinterpret_cast<std::uintptr_t>( this ) + sizeof( void* ) * 2 );
    }


    auto GetAbsOrigin()
    {
        return Hikari::Memory->VirtualCall<Vector3D_t&, 11>( this );
    }

    auto GetAbsAngle()
    {
        return Hikari::Memory->VirtualCall<Angle_t&, 12>(this);
    }
};

struct CBaseAnimating_t : CBaseEntity_t
{

};

struct CBaseCombatWeapon_t : CBaseEntity_t
{

};

struct CBaseCombatCharacter_t : CBaseAnimating_t
{

};

struct CBasePlayer_t : CBaseCombatCharacter_t
{
    NETVAR(m_iHealth, "CBasePlayer", "m_iHealth", std::int32_t);
    NETVAR(m_fFlags, "CBasePlayer", "m_fFlags", bitflag_t);

    bool IsPlayerDead()
    {
        return Game::Addresses::IsPlayerDead.Cast<bool* (__thiscall*)(void*)>()(this);
    }

    auto IsAlive()
    {
        return !IsPlayerDead();
    }
};

struct CTerrorPlayer_t : CBasePlayer_t
{
    NETVAR(m_iMaxHealth, "CTerrorPlayer", "m_iMaxHealth", std::int32_t);
    NETVAR(m_isIncapacitated, "CTerrorPlayer", "m_isIncapacitated", std::int32_t);
    NETVAR(m_healthBuffer, "CTerrorPlayer", "m_healthBuffer", std::float_t);
};
