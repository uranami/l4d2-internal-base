#include "../../framework.h"
#include "interfaces.h"
#include "../modules.h"

namespace Game
{
    Interfaces_t::Interfaces_t()
    {
        #ifdef DUMP_SE
        auto dump = [&]( const std::string& name, void* list )
        {
            Hikari::Logger->Info( std::format( "Dumping {} interfaces", name ) );
            DumpInterfaces( list );
        };

        dump( "client.dll", Modules::Client->GetInterfaceList() );
        dump( "engine.dll", Modules::Engine->GetInterfaceList() );
        dump( "materialsystem.dll", Modules::MaterialSystem->GetInterfaceList() );
        dump( "studiorender.dll", Modules::StudioRender->GetInterfaceList() );
        #endif

        Engine = GetInterface<IEngineClient_t>( Modules::Engine->GetInterfaceList(), "VEngineClient0" );
        Client = GetInterface<IBaseClientDll_t>( Modules::Client->GetInterfaceList(), "VClient0" );
        EntityList = GetInterface<IClientEntityList_t>( Modules::Client->GetInterfaceList(), "VClientEntityList" );
        Surface = GetInterface<ISurface_t>( Modules::MatSurface->GetInterfaceList(), "VGUI_Surface0" );
        ClientMode = Modules::Client->FindPattern("E8 ? ? ? ? 8B 10 8B C8 8B 52 34").Relative().Cast<TerrorClientMode_t* (*)()>()();
        CInput = Modules::Client->FindPattern("8B 0D ? ? ? ? 8B 01 8B 40 0C 52 8B 55 08").Offset(2).Dereference(2).Cast<IInput_t*>();
        GlobalVars = **(CGlobalVars_t***)((*(std::uintptr_t**)Client)[10] + 0x11);

    }
}
