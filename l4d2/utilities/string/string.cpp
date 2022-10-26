#include "../lib.h"
#include "string.h"

#include <Windows.h>

namespace Hikari
{
    std::string String::ToUtf8( const std::wstring& str )
    {
        if ( str.empty() )
            return std::string();

        const int size = WideCharToMultiByte( CP_UTF8, 0, &str[ 0 ], str.size(), nullptr, 0, nullptr, nullptr );

        std::string out( size, 0 );
        WideCharToMultiByte( CP_UTF8, 0, &str[ 0 ], str.size(), &out[ 0 ], size, nullptr, nullptr );

        return out;
    }

    std::wstring String::ToUnicode( const std::string& str )
    {
        if ( str.empty() )
            return std::wstring();

        const auto size = MultiByteToWideChar( CP_UTF8, 0, &str[ 0 ], str.size(), nullptr, 0 );

        std::wstring out( size, 0 );
        MultiByteToWideChar( CP_UTF8, 0, &str[ 0 ], str.size(), &out[ 0 ], size );

        return out;
    }

    std::vector<std::string> String::Split( const std::string& str, const std::string& delimiter )
    {
        std::vector<std::string> vec;
        size_t last = 0;
        size_t next;

        while ( ( next = str.find( delimiter, last ) ) != std::string::npos )
        {
            if ( auto s = str.substr( last, next - last ); !s.empty() )
                vec.push_back( s );
            last = next + 1;
        }

        if ( const auto s = str.substr( last ); !s.empty() )
            vec.push_back( s );

        return vec;
    }

    std::string_view String::Lowercase( std::string& str )
    {
        std::ranges::transform( str.begin(), str.end(), str.begin(), []( const unsigned char c )
        {
            return std::tolower( c );
        } );

        return str;
    }

    std::string_view String::Uppercase( std::string& str )
    {
        std::ranges::transform( str.begin(), str.end(), str.begin(), []( const unsigned char c )
        {
            return std::toupper( c );
        } );

        return str;
    }
}
