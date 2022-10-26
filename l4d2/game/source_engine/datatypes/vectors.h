#pragma once

// used: isfinite, fmodf, sqrtf
#include <cmath>
// used: numeric_limits
#include <limits>

class Vector2D_t
{
public:
    constexpr Vector2D_t( float x = 0.f, float y = 0.f ) : x( x ), y( y )
    {
    }

    [[nodiscard]] bool IsZero() const
    {
        return ( std::fpclassify( this->x ) == FP_ZERO && std::fpclassify( this->y ) == FP_ZERO );
    }

    float x, y;
};

class Vector3D_t
{
public:
    constexpr Vector3D_t( float x = 0.f, float y = 0.f, float z = 0.f ) : x( x ), y( y ), z( z )
    {
    }

    constexpr Vector3D_t( const float* arrVector ) : x( arrVector[ 0 ] ), y( arrVector[ 1 ] ), z( arrVector[ 2 ] )
    {
    }

    constexpr Vector3D_t( const Vector2D_t& vecBase2D ) : x( vecBase2D.x ), y( vecBase2D.y ), z( 0.0f )
    {
    }

    [[nodiscard]] bool IsValid() const
    {
        return std::isfinite( this->x ) && std::isfinite( this->y ) && std::isfinite( this->z );
    }

    constexpr void Invalidate()
    {
        this->x = this->y = this->z = std::numeric_limits<float>::infinity();
    }

    [[nodiscard]] float* data()
    {
        return reinterpret_cast<float*>( this );
    }

    [[nodiscard]] const float* data() const
    {
        return reinterpret_cast<float*>( const_cast<Vector3D_t*>( this ) );
    }

    float& operator[]( const std::size_t i )
    {
        return this->data()[ i ];
    }

    const float& operator[]( const std::size_t i ) const
    {
        return this->data()[ i ];
    }

    bool operator==( const Vector3D_t& vecBase ) const
    {
        return this->IsEqual( vecBase );
    }

    bool operator!=( const Vector3D_t& vecBase ) const
    {
        return !this->IsEqual( vecBase );
    }

    constexpr Vector3D_t& operator=( const Vector3D_t& vecBase )
    {
        this->x = vecBase.x;
        this->y = vecBase.y;
        this->z = vecBase.z;
        return *this;
    }

    constexpr Vector3D_t& operator=( const Vector2D_t& vecBase2D )
    {
        this->x = vecBase2D.x;
        this->y = vecBase2D.y;
        this->z = 0.0f;
        return *this;
    }

    constexpr Vector3D_t& operator+=( const Vector3D_t& vecBase )
    {
        this->x += vecBase.x;
        this->y += vecBase.y;
        this->z += vecBase.z;
        return *this;
    }

    constexpr Vector3D_t& operator-=( const Vector3D_t& vecBase )
    {
        this->x -= vecBase.x;
        this->y -= vecBase.y;
        this->z -= vecBase.z;
        return *this;
    }

    constexpr Vector3D_t& operator*=( const Vector3D_t& vecBase )
    {
        this->x *= vecBase.x;
        this->y *= vecBase.y;
        this->z *= vecBase.z;
        return *this;
    }

    constexpr Vector3D_t& operator/=( const Vector3D_t& vecBase )
    {
        this->x /= vecBase.x;
        this->y /= vecBase.y;
        this->z /= vecBase.z;
        return *this;
    }

    constexpr Vector3D_t& operator+=( const float flAdd )
    {
        this->x += flAdd;
        this->y += flAdd;
        this->z += flAdd;
        return *this;
    }

    constexpr Vector3D_t& operator-=( const float flSubtract )
    {
        this->x -= flSubtract;
        this->y -= flSubtract;
        this->z -= flSubtract;
        return *this;
    }

    constexpr Vector3D_t& operator*=( const float flMultiply )
    {
        this->x *= flMultiply;
        this->y *= flMultiply;
        this->z *= flMultiply;
        return *this;
    }

    constexpr Vector3D_t& operator/=( const float flDivide )
    {
        this->x /= flDivide;
        this->y /= flDivide;
        this->z /= flDivide;
        return *this;
    }

    Vector3D_t operator+( const Vector3D_t& vecAdd ) const
    {
        return Vector3D_t( this->x + vecAdd.x, this->y + vecAdd.y, this->z + vecAdd.z );
    }

    Vector3D_t operator-( const Vector3D_t& vecSubtract ) const
    {
        return Vector3D_t( this->x - vecSubtract.x, this->y - vecSubtract.y, this->z - vecSubtract.z );
    }

    Vector3D_t operator*( const Vector3D_t& vecMultiply ) const
    {
        return Vector3D_t( this->x * vecMultiply.x, this->y * vecMultiply.y, this->z * vecMultiply.z );
    }

    Vector3D_t operator/( const Vector3D_t& vecDivide ) const
    {
        return Vector3D_t( this->x / vecDivide.x, this->y / vecDivide.y, this->z / vecDivide.z );
    }

    Vector3D_t operator+( const float flAdd ) const
    {
        return Vector3D_t( this->x + flAdd, this->y + flAdd, this->z + flAdd );
    }

    Vector3D_t operator-( const float flSubtract ) const
    {
        return Vector3D_t( this->x - flSubtract, this->y - flSubtract, this->z - flSubtract );
    }

    Vector3D_t operator*( const float flMultiply ) const
    {
        return Vector3D_t( this->x * flMultiply, this->y * flMultiply, this->z * flMultiply );
    }

    Vector3D_t operator/( const float flDivide ) const
    {
        return Vector3D_t( this->x / flDivide, this->y / flDivide, this->z / flDivide );
    }

    [[nodiscard]] bool IsEqual( const Vector3D_t& vecEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon() ) const
    {
        return ( std::fabsf( this->x - vecEqual.x ) < flErrorMargin && std::fabsf( this->y - vecEqual.y ) < flErrorMargin && std::fabsf( this->z - vecEqual.z ) < flErrorMargin );
    }

    [[nodiscard]] bool IsZero() const
    {
        return ( std::fpclassify( this->x ) == FP_ZERO && std::fpclassify( this->y ) == FP_ZERO && std::fpclassify( this->z ) == FP_ZERO );
    }

    [[nodiscard]] Vector2D_t ToVector2D() const
    {
        return Vector2D_t( this->x, this->y );
    }

    [[nodiscard]] float Length() const
    {
        return std::sqrtf( this->LengthSqr() );
    }

    [[nodiscard]] constexpr float LengthSqr() const
    {
        return DotProduct( *this );
    }

    [[nodiscard]] float Length2D() const
    {
        return std::sqrtf( this->Length2DSqr() );
    }

    [[nodiscard]] constexpr float Length2DSqr() const
    {
        return ( this->x * this->x + this->y * this->y );
    }

    [[nodiscard]] float DistTo( const Vector3D_t& vecEnd ) const
    {
        return ( *this - vecEnd ).Length();
    }

    [[nodiscard]] constexpr float DistToSqr( const Vector3D_t& vecEnd ) const
    {
        return ( *this - vecEnd ).LengthSqr();
    }

    [[nodiscard]] Vector3D_t Normalized() const
    {
        Vector3D_t vecOut = *this;
        vecOut.NormalizeInPlace();
        return vecOut;
    }

    float NormalizeInPlace()
    {
        const float flLength = this->Length();
        const float flRadius = 1.0f / ( flLength + std::numeric_limits<float>::epsilon() );

        this->x *= flRadius;
        this->y *= flRadius;
        this->z *= flRadius;

        return flLength;
    }

    [[nodiscard]] constexpr float DotProduct( const Vector3D_t& vecDot ) const
    {
        return ( this->x * vecDot.x + this->y * vecDot.y + this->z * vecDot.z );
    }

    [[nodiscard]] constexpr Vector3D_t CrossProduct( const Vector3D_t& vecCross ) const
    {
        return Vector3D_t( this->y * vecCross.z - this->z * vecCross.y, this->z * vecCross.x - this->x * vecCross.z, this->x * vecCross.y - this->y * vecCross.x );
    }



    float x, y, z;
};

class Vector4D_t
{
public:
    constexpr Vector4D_t( float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f ) : x( x ), y( y ), z( z ), w( w )
    {
    }

    float x, y, z, w;
};

class __declspec(align(16)) VectorAligned_t : public Vector3D_t
{
public:
    VectorAligned_t() = default;

    explicit VectorAligned_t( const Vector3D_t& vecBase )
    {
        this->x = vecBase.x;
        this->y = vecBase.y;
        this->z = vecBase.z;
        this->w = 0.f;
    }

    constexpr VectorAligned_t& operator=( const Vector3D_t& vecBase )
    {
        this->x = vecBase.x;
        this->y = vecBase.y;
        this->z = vecBase.z;
        this->w = 0.f;
        return *this;
    }

    float w;
};
