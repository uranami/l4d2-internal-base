#include "../lib.h"

#include <chrono>
#include <iostream>

namespace Hikari
{
    Console_t::Console_t()
    {
        Initialize();
    }

    Console_t::~Console_t()
    {
        this->Destory();
    }

    bool Console_t::Initialize()
    {
        if ( this->_initialized )
            return true;

        #ifdef WINDOWS
        if ( !AllocConsole() )
            throw std::runtime_error( ( "Failed to allocate a new console" ) );

        freopen_s( reinterpret_cast<_iobuf**>( stdout ), ( "CONOUT$" ), ( "w" ), stdout );
        SetConsoleTitleA( ( "Hikari" ) );

        this->_initialized = true;

        this->Success( ("Console initialized") );

        return true;
        #endif
    }

    void Console_t::Destory()
    {
        if ( !this->_initialized )
            return;

        this->_initialized = false;

        #ifdef WINDOWS
        fclose( stdin );
        fclose( stdout );
        fclose( stderr );
        FreeConsole();
        #endif
    }
        
    void Console_t::Log( std::string_view str )
    {
        if ( !this->_initialized )
            return;

        const auto& timezones = std::chrono::get_tzdb();
        const auto& current_timezone = timezones.current_zone();

        const auto time = std::format( ( "[{:%H:%M:%S}]" ), current_timezone->to_local( std::chrono::system_clock::now() ) );
        // #ifdef _DEBUG
        std::cout << std::format( ( "{} {} \n" ), time, str );
        // #endif
    }

    void Console_t::Info( std::string_view str )
    {
        if ( !this->_initialized )
            return;

        // #ifdef _DEBUG
        this->Log( std::format( ( "[-] {}" ), str ) );
        // #endif
    }

    void Console_t::Error( std::string_view str )
    {
        if ( !this->_initialized )
            return;

        // #ifdef _DEBUG
        this->Log( std::format( ( "[x] {}" ), str ) );
        // #endif
    }

    void Console_t::Success( std::string_view str )
    {
        if ( !this->_initialized )
            return;

        // #ifdef _DEBUG
        this->Log( std::format( ( "[+] {}" ), str ) );
        // #endif
    }
}
