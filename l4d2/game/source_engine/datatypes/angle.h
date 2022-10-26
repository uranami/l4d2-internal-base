#pragma once

#include <algorithm>
#include <cmath>

class Angle_t
{
public:
    float x, y, z;

    constexpr Angle_t( float x = 0.f, float y = 0.f, float z = 0.f ) : x( x ), y( y ), z( z )
    {
    }

    constexpr Angle_t( const float* arrAngles ) : x( arrAngles[ 0 ] ), y( arrAngles[ 1 ] ), z( arrAngles[ 2 ] )
    {
    }

    bool operator==( const Angle_t& angBase ) const
    {
        return this->IsEqual( angBase );
    }

    bool operator!=( const Angle_t& angBase ) const
    {
        return !this->IsEqual( angBase );
    }

    constexpr Angle_t& operator=( const Angle_t& angBase )
    {
        this->x = angBase.x;
        this->y = angBase.y;
        this->z = angBase.z;
        return *this;
    }

    constexpr Angle_t& operator+=( const Angle_t& angBase )
    {
        this->x += angBase.x;
        this->y += angBase.y;
        this->z += angBase.z;
        return *this;
    }

    constexpr Angle_t& operator-=( const Angle_t& angBase )
    {
        this->x -= angBase.x;
        this->y -= angBase.y;
        this->z -= angBase.z;
        return *this;
    }

    constexpr Angle_t& operator*=( const Angle_t& angBase )
    {
        this->x *= angBase.x;
        this->y *= angBase.y;
        this->z *= angBase.z;
        return *this;
    }

    constexpr Angle_t& operator/=( const Angle_t& angBase )
    {
        this->x /= angBase.x;
        this->y /= angBase.y;
        this->z /= angBase.z;
        return *this;
    }

    constexpr Angle_t& operator+=( const float flAdd )
    {
        this->x += flAdd;
        this->y += flAdd;
        this->z += flAdd;
        return *this;
    }

    constexpr Angle_t& operator-=( const float flSubtract )
    {
        this->x -= flSubtract;
        this->y -= flSubtract;
        this->z -= flSubtract;
        return *this;
    }

    constexpr Angle_t& operator*=( const float flMultiply )
    {
        this->x *= flMultiply;
        this->y *= flMultiply;
        this->z *= flMultiply;
        return *this;
    }

    constexpr Angle_t& operator/=( const float flDivide )
    {
        this->x /= flDivide;
        this->y /= flDivide;
        this->z /= flDivide;
        return *this;
    }

    Angle_t operator+( const Angle_t& angAdd ) const
    {
        return Angle_t( this->x + angAdd.x, this->y + angAdd.y, this->z + angAdd.z );
    }

    Angle_t operator-( const Angle_t& angSubtract ) const
    {
        return Angle_t( this->x - angSubtract.x, this->y - angSubtract.y, this->z - angSubtract.z );
    }

    Angle_t operator*( const Angle_t& angMultiply ) const
    {
        return Angle_t( this->x * angMultiply.x, this->y * angMultiply.y, this->z * angMultiply.z );
    }

    Angle_t operator/( const Angle_t& angDivide ) const
    {
        return Angle_t( this->x / angDivide.x, this->y / angDivide.y, this->z / angDivide.z );
    }

    Angle_t operator+( const float flAdd ) const
    {
        return Angle_t( this->x + flAdd, this->y + flAdd, this->z + flAdd );
    }

    Angle_t operator-( const float flSubtract ) const
    {
        return Angle_t( this->x - flSubtract, this->y - flSubtract, this->z - flSubtract );
    }

    Angle_t operator*( const float flMultiply ) const
    {
        return Angle_t( this->x * flMultiply, this->y * flMultiply, this->z * flMultiply );
    }

    Angle_t operator/( const float flDivide ) const
    {
        return Angle_t( this->x / flDivide, this->y / flDivide, this->z / flDivide );
    }

    [[nodiscard]] bool IsEqual( const Angle_t& angEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon() ) const
    {
        return ( std::fabsf( this->x - angEqual.x ) < flErrorMargin && std::fabsf( this->y - angEqual.y ) < flErrorMargin && std::fabsf( this->z - angEqual.z ) < flErrorMargin );
    }

    [[nodiscard]] bool IsZero() const
    {
        return ( std::fpclassify( this->x ) == FP_ZERO && std::fpclassify( this->y ) == FP_ZERO && std::fpclassify( this->z ) == FP_ZERO );
    }

    Angle_t Clamp()
    {
        this->x = std::clamp( this->x, -89.f, 89.f );
        this->y = std::clamp( this->y, -180.f, 180.f );
        this->z = std::clamp( this->z, -50.f, 50.f );
        return *this;
    }

    Angle_t Normalize()
    {
        this->x = std::isfinite( this->x ) ? std::remainderf( this->x, 360.f ) : 0.f;
        this->y = std::isfinite( this->y ) ? std::remainderf( this->y, 360.f ) : 0.f;
        this->z = std::clamp( this->z, -50.f, 50.f );
        return *this;
    }

    Angle_t Mod( const float flValue )
    {
        this->x = std::fmodf( this->x, flValue );
        this->y = std::fmodf( this->y, flValue );
        this->z = std::fmodf( this->z, flValue );
        return *this;
    }
};
