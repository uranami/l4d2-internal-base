#pragma once

#include "classes/bitflags.h"
#include "classes/color.h"
#include "datatypes/checksum_crc.h"
#include "datatypes/checksum_md5.h"
#include "datatypes/angle.h"
#include "datatypes/vectors.h"
#include "datatypes/matrix.h"
#include "datatypes/usercmd.h"
#include "datatypes/datamap.h"


#include "interfaces/engine_client.h"
#include "interfaces/base_client.h"
#include "interfaces/client_entity_list.h"
#include "interfaces/surface.h"
#include "interfaces/globalvars.h"
#include "interfaces/input.h"
#include "interfaces/clientmode.h"


namespace Game
{
    struct Interfaces_t
    {
        Interfaces_t();

        IEngineClient_t* Engine;
        IBaseClientDll_t* Client;
        IClientEntityList_t* EntityList;
        TerrorClientMode_t* ClientMode;
        ISurface_t* Surface;
        CGlobalVars_t* GlobalVars;
        IInput_t* CInput;


    private:
        class CInterfaceRegister
        {
            using InstantiateInterfaceFn = void* (__cdecl*)();
        public:
            InstantiateInterfaceFn pCreateFn;
            const char* szName;
            CInterfaceRegister* pNext;
        };

        template < typename T >
        T* GetInterface( void* intertface_list, std::string_view interface_name );

        void DumpInterfaces( void* interface_list ) const;
    };

    template < typename T >
    T* Interfaces_t::GetInterface( void* interface_list, std::string_view interface_name )
    {
        if ( !interface_list )
            throw std::runtime_error( ( "Invalid interface list" ) );

        if ( interface_name.empty() )
            throw std::runtime_error( ( "Empty interface name" ) );

        for ( auto i = static_cast<CInterfaceRegister*>( interface_list ); i != nullptr; i = i->pNext )
        {
            if ( std::string_view( i->szName ).compare( 0U, interface_name.length(), interface_name ) == 0 && std::atoi( i->szName + interface_name.length() ) > 0 || interface_name.compare( i->szName ) == 0 )
            {
                auto address = i->pCreateFn();
                Hikari::Logger->Success( std::format( ( "Found interface {} ==> {:#09X}" ), i->szName, reinterpret_cast<std::uintptr_t>( address ) ) );
                return static_cast<T*>( address );
            }
        }

        throw std::runtime_error( std::format( ( "Failed to get interface {}" ), interface_name ) );
    }

    inline void Interfaces_t::DumpInterfaces( void* interface_list ) const
    {
        if ( !interface_list )
            throw std::runtime_error( ( "Invalid interface list" ) );

        for ( auto i = static_cast<CInterfaceRegister*>( interface_list ); i != nullptr; i = i->pNext )
        {
            Hikari::Logger->Success( std::format( "Found interface: {}", i->szName ) );
        }
    }

    inline std::unique_ptr<Interfaces_t> Interfaces;
}
