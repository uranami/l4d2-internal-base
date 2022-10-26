#pragma once

struct matrix3x4_t;
class Angle_t;

#define MAX_PLAYER_NAME_LENGTH 32
#define SIGNED_GUID_LEN 32

struct PlayerInfo_t
{
    PAD(0x08);
    char name[MAX_PLAYER_NAME_LENGTH];
    int userID;
    char guid[SIGNED_GUID_LEN + 1];
    uint32_t friendsID;
    char friendsName[MAX_PLAYER_NAME_LENGTH];
    bool fakeplayer;
    PAD(0x1B);
};

struct IEngineClient_t
{
    auto GetScreenSize( int& w, int& h )
    {
        return Hikari::Memory->VirtualCall<void, 5>( this, std::ref( w ), std::ref( h ) );
    }

    auto ServerCmd(const char* Command, bool ForceReliable)
    {
        return Hikari::Memory->VirtualCall<void, 6>(this, Command, ForceReliable);
    }

    auto GetPlayerInfo( int index, PlayerInfo_t* info )
    {
        return Hikari::Memory->VirtualCall<bool, 8>( this, index, info );
    }

    auto Con_IsVisible()
    {
        return Hikari::Memory->VirtualCall<bool, 11>(this);
    }

    auto GetLocalPlayer()
    {
        return Hikari::Memory->VirtualCall<int, 12>( this );
    }

    auto GetViewAngles( const Angle_t& angle )
    {
        return Hikari::Memory->VirtualCall<void, 19>( this, std::ref( angle ) );
    }

    auto SetViewAngles( const Angle_t& angle )
    {
        return Hikari::Memory->VirtualCall<void, 20>( this, std::ref( angle ) );
    }

    auto IsInGame()
    {
        return Hikari::Memory->VirtualCall<bool, 26>( this );
    }

    const auto& WorldToScreenMatrix()
    {
        return Hikari::Memory->VirtualCall<const matrix3x4_t&, 37>(this);
    }

    auto ClientCmd_Unrestricted(const char* szCmdString)
    {
        return Hikari::Memory->VirtualCall<void, 106>(this, szCmdString);
    }

};
