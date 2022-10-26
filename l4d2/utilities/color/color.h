#pragma once
#include <compare>
#include <cstdint>

namespace Hikari
{
    struct Color_t
    {
        uint8_t r{}, g{}, b{}, a{};

        Color_t() = default;

        Color_t( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255 ) : r{ r }, g{ g }, b{ b }, a{ a }
        {
        }

        Color_t( int r, int g, int b, int a = 255 ) : r{ static_cast<std::uint8_t>( r ) }, g{ static_cast<std::uint8_t>( g ) }, b{ static_cast<std::uint8_t>( b ) }, a{ static_cast<std::uint8_t>( a ) }
        {
        }

        Color_t( float r, float g, float b, float a = 255 ) : r{ static_cast<std::uint8_t>( r ) }, g{ static_cast<std::uint8_t>( g ) }, b{ static_cast<std::uint8_t>( b ) }, a{ static_cast<std::uint8_t>( a ) }
        {
        }

        Color_t( const int* arr ) : r{ static_cast<std::uint8_t>( arr[ 0 ] ) }, g{ static_cast<std::uint8_t>( arr[ 1 ] ) }, b{ static_cast<std::uint8_t>( arr[ 2 ] ) }, a{ static_cast<std::uint8_t>( arr[ 3 ] ) }
        {
        }

        Color_t( const float* arr ) : r{ static_cast<std::uint8_t>( arr[ 0 ] * 255.f ) }, g{ static_cast<std::uint8_t>( arr[ 1 ] * 255.f ) }, b{ static_cast<std::uint8_t>( arr[ 2 ] * 255.f ) }, a{ static_cast<std::uint8_t>( arr[ 3 ] * 255.f ) }
        {
        }

        Color_t( const std::uint8_t* arr ) : r{ arr[ 0 ] }, g{ arr[ 1 ] }, b{ arr[ 2 ] }, a{ arr[ 3 ] }
        {
        }

        constexpr auto operator<=>( const Color_t& ) const = default;
        constexpr bool operator==( const Color_t& ) const = default;
    };
}
