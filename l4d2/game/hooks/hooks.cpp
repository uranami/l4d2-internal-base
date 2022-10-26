#include "../../framework.h"
#include "../game.h"
#include "hooks.h"
#include <intrin.h>
#include <mutex>


#include "../../thirdparty/minhook/MinHook.h"


namespace Game
{
    Hook_t::Detour_t::Detour_t() : _source( nullptr ), _target( nullptr ), _hooked( false )
    {
    }

    Hook_t::Detour_t::~Detour_t()
    {
        Unhook();
    }

    void Hook_t::Detour_t::Hook( void* source, void* target, void** original, const std::string& name )
    {
        if ( this->_hooked )
            return;

        this->_name = name;
        if ( !source )
            throw std::runtime_error( std::format( "Invalid source function for {}", name ) );

        if ( !target )
            throw std::runtime_error( std::format( "Invalid target function for {}", name ) );

        this->_source = source;
        this->_target = target;

        if ( const auto status = MH_CreateHook( this->_source, this->_target, original ); status != MH_OK )
            throw std::runtime_error( std::format( "Failed to create hook for {}. r:{}", name, MH_StatusToString( status ) ) );

        if ( MH_EnableHook( this->_source ) != MH_OK )
            throw std::runtime_error( std::format( "Failed to enable hook for {}", name ) );

        this->_hooked = true;
        Hikari::Logger->Success( std::format( "Hooked {}. {:#09X} --> {:#09X}", name, reinterpret_cast<std::uintptr_t>( this->_source ), reinterpret_cast<std::uintptr_t>( this->_target ) ) );
    }

    WNDPROC _WndProc;

    void Hook_t::Detour_t::Unhook()
    {
        if ( !this->_hooked )
            return;

        if ( MH_RemoveHook( this->_source ) != MH_OK )
            throw std::runtime_error( std::format( "Failed to remove hook for {}", this->_name ) );

        this->_hooked = false;

        Hikari::Logger->Success( std::format( "Unhooked {}. {:#09X} --> {:#09X}", this->_name, reinterpret_cast<std::uintptr_t>( this->_target ), reinterpret_cast<std::uintptr_t>( this->_source ) ) );
    }

    Hook_t::Hook_t()
    {
        if ( MH_Initialize() != MH_OK )
            throw std::runtime_error( "Failed to initialize hook" );

        Render::InitFont();

        _Paint.Hook( Addresses::Paint.Cast<void*>(), Paint::Hook, reinterpret_cast<void**>( &Paint::Original ), "CEngineVGui::Paint" );

        _CHLCreateMove.Hook( reinterpret_cast<void*>( Hikari::Memory->VirtualFunction<20>( Interfaces->Client ) ), CHLCreateMove::Hook, reinterpret_cast<void**>( &CHLCreateMove::Original ), "CHLClient::CreateMove" );
        _LevelInitPostEntity.Hook( reinterpret_cast<void*>( Hikari::Memory->VirtualFunction<5>( Interfaces->Client ) ), LevelInitPostEntity::Hook, reinterpret_cast<void**>( &LevelInitPostEntity::Original ), "CHLClient::LevelInitPostEntity" );
        _LevelShutdown.Hook( reinterpret_cast<void*>( Hikari::Memory->VirtualFunction<6>( Interfaces->Client ) ), LevelShutdown::Hook, reinterpret_cast<void**>( &LevelShutdown::Original ), "CHLClient::LevelShutdown" );
    }

    Hook_t::~Hook_t()
    {

    }

    void __fastcall  CHLCreateMove::Hook(void* _ecx, void* _edx, int sequence_number, float input_sample_frametime, bool active)
    {
        CHLCreateMove::Original(Interfaces->Client, nullptr, sequence_number, input_sample_frametime, active);

        DWORD pep;
        __asm mov pep, ebp;
        auto* cmd = &Interfaces->CInput->m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
        auto* VerifiedCmd = &Interfaces->CInput->m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
        bool* BSendPacket = reinterpret_cast<bool*>(*reinterpret_cast<char**>(pep) - 0x1D);

        if (!cmd || !VerifiedCmd || !active)
            return;

            _local = Interfaces->EntityList->Get<CTerrorPlayer_t*>( Interfaces->Engine->GetLocalPlayer() );

            if ( _local && _local->IsAlive() )
            {
                const auto oldAngles = cmd->viewangles;
                //do ur epic stuff here.
                if (cmd->buttons.HasFlag(IN_JUMP) && !_local->m_fFlags().HasFlag((1 << 0)))
                    cmd->buttons.RemoveFlag(IN_JUMP);


                Math::CorrectMovement( oldAngles, cmd );
            }

        VerifiedCmd->m_cmd = *cmd;
        VerifiedCmd->m_crc = cmd->GetCheckSum();
    }

    void __fastcall Paint::Hook( void* ecx, void* edx, unsigned mode )
    {
        Original( ecx, edx, mode );

        static auto StartDrawing = Addresses::StartDrawing.Cast<void(__thiscall*)( void* )>();
        static auto FinishDrawing = Addresses::FinishDrawing.Cast<void(__thiscall*)( void* )>();

        if ( mode & 1 )
        {
    
            StartDrawing( Interfaces->Surface );


            static int w, h;
            Interfaces->Engine->GetScreenSize( w, h );
            Interfaces->Surface->DrawColoredTexted( Fonts::WaterMark, w * 0.17, 20, 255, 192, 203, 255, "Left 4 Dead 2 Internal Base By Rico1662" );
  

            FinishDrawing( Interfaces->Surface );
        }
    }

    void __fastcall LevelInitPostEntity::Hook( void* ecx, void* edx )
    {

        Original( ecx, edx );
    }

    void __fastcall LevelShutdown::Hook( void* ecx, void* edx )
    {
        Original( ecx, edx );
        _local = nullptr;
    }
}
