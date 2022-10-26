#pragma once

#include <numbers>

namespace Game::Math
{
    Vector3D_t Transform( Vector3D_t& tr, const matrix3x4_t& matrix );
    void Transform( const Vector3D_t& tr, matrix3x4_t& matrix, Vector3D_t& out );
    void AngleVectors( const Angle_t& angles, Vector3D_t& out );
    void AngleVectors( const Angle_t& angles, Vector3D_t& forward, Vector3D_t& right, Vector3D_t& up );
    void VectorAngles( const Vector3D_t& vec, Angle_t& out );
    void VectorAngles( const Vector3D_t& vec, const Vector3D_t& up, Angle_t& out );
    void CorrectMovement( const Angle_t& wish_angle, CUserCmd* cmd );
    float NormalizeYaw( float yaw );

    inline float Rad2Deg( const float rad )
    {
        return static_cast<float>( rad * ( 180.f / std::numbers::pi ) );
    }

    inline float Deg2Rad( const float deg )
    {
        return static_cast<float>( deg * ( std::numbers::pi / 180.f ) );
    }

    inline void SinCos( const float rad, float& sine, float& cosine )
    {
        sine = std::sin( rad );
        cosine = std::cos( rad );
    }
}
