// dllmain.cpp : Defines the entry point for the DLL application.

#include <thread>
#include "framework.h"
#include "game/game.h"

DWORD WINAPI Setup( LPVOID parameter )
{
    try
    {
        Hikari::Logger = std::make_unique<Hikari::Console_t>();
        Hikari::Memory = std::make_unique<Hikari::Memory_t>();

        {
            Game::Modules::Client = std::make_unique<Game::Modules_t>( "client.dll" );
            Game::Modules::Engine = std::make_unique<Game::Modules_t>( "engine.dll" );
            Game::Modules::MaterialSystem = std::make_unique<Game::Modules_t>( "materialsystem.dll" );
            Game::Modules::StudioRender = std::make_unique<Game::Modules_t>( "studiorender.dll" );
            Game::Modules::VStdLib = std::make_unique<Game::Modules_t>( "vstdlib.dll" );
            Game::Modules::MatSurface = std::make_unique<Game::Modules_t>( "vguimatsurface.dll" );
            Game::Modules::Server = std::make_unique<Game::Modules_t>( "server.dll" );
            Game::Modules::VGui = std::make_unique<Game::Modules_t>( "vgui2.dll" );
            Game::Modules::shaderapidx9 = std::make_unique<Game::Modules_t>( "shaderapidx9.dll" );
        }

        Game::Addresses::Setup();
        Game::Interfaces = std::make_unique<Game::Interfaces_t>();

        Game::Hooks = std::make_unique<Game::Hook_t>();
    }
    catch ( std::exception& exp )
    {
        Hikari::Logger->Error( std::format( ( "{}" ), exp.what() ) );
        _RPT0( _CRT_ERROR, exp.what() );
        Hikari::Logger->Destory();

        FreeLibraryAndExitThread( static_cast<HMODULE>( parameter ), EXIT_SUCCESS );
    }
    return TRUE;
}

DWORD WINAPI Destroy( LPVOID parameter )
{
    while ( !GetAsyncKeyState( VK_F10 ) )
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

    Hikari::Logger->Destory();
    FreeLibraryAndExitThread( static_cast<HMODULE>( parameter ), EXIT_SUCCESS );
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
    if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
    {
        DisableThreadLibraryCalls( hModule );

        if ( const auto thread = CreateThread( nullptr, 0, Setup, hModule, 0UL, nullptr ); thread != nullptr )
            CloseHandle( thread );

        if ( const auto thread = CreateThread( nullptr, 0, Destroy, hModule, 0UL, nullptr ); thread != nullptr )
            CloseHandle( thread );

        return TRUE;
    }

    return TRUE;
}
