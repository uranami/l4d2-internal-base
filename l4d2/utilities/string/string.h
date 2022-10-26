#pragma once

namespace Hikari::String
{
    std::string ToUtf8( const std::wstring& str );
    std::wstring ToUnicode( const std::string& str );
    std::vector<std::string> Split( const std::string& str, const std::string& delimiter );
    std::string_view Lowercase( std::string& str );
    std::string_view Uppercase( std::string& str );
}
