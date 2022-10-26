#include "../../framework.h"
#include "../game.h"

#define M_PI		3.14159265358979323846
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )

namespace Game::Math
{
    Vector3D_t Transform( Vector3D_t& tr, const matrix3x4_t& matrix )
    {
        return Vector3D_t( tr.DotProduct( matrix[ 0 ] ) + matrix[ 0 ][ 3 ],
            tr.DotProduct( matrix[ 1 ] ) + matrix[ 1 ][ 3 ], 
            tr.DotProduct( matrix[ 2 ] ) + matrix[ 2 ][ 3 ] );
    }

    void Transform( const Vector3D_t& tr, matrix3x4_t& matrix, Vector3D_t& out )
    {
        out.x = tr.DotProduct( matrix[ 0 ] ) + matrix[ 0 ][ 3 ];
        out.y = tr.DotProduct( matrix[ 1 ] ) + matrix[ 1 ][ 3 ];
        out.z = tr.DotProduct( matrix[ 2 ] ) + matrix[ 2 ][ 3 ];
    }

    void AngleVectors(const Angle_t& angles, Vector3D_t& out)
    {
        auto sine = Vector2D_t(), cosine = Vector2D_t();

        SinCos( Deg2Rad(angles.x ), sine.x, cosine.x );
        SinCos( Deg2Rad(angles.y ), sine.y, cosine.y );

        out.x = cosine.x * cosine.y;
        out.y = cosine.x * sine.y;
        out.z = -sine.x;
    }

    void AngleVectors( const Angle_t& ang, Vector3D_t& forward, Vector3D_t& right, Vector3D_t& up )
    {
        auto sine = Vector3D_t(), cosine = Vector3D_t();

        SinCos( Deg2Rad( ang.x ), sine.x, cosine.x );
        SinCos( Deg2Rad( ang.y ), sine.y, cosine.y );
        SinCos( Deg2Rad( ang.z ), sine.z, cosine.z );

        forward.x = cosine.x * cosine.y;
        forward.y = cosine.x * sine.y;
        forward.z = -sine.x;

        right.x = -1 * sine.z * sine.x * cosine.y + -1 * cosine.z * -sine.y;
        right.y = -1 * sine.z * sine.x * sine.y + -1 * cosine.z * cosine.y;
        right.z = -1 * sine.z * cosine.x;

        up.x = cosine.z * sine.x * cosine.y + -sine.z * -sine.y;
        up.y = cosine.z * sine.x * sine.y + -sine.z * cosine.y;
        up.z = cosine.z * cosine.x;
    }

    void VectorAngles( const Vector3D_t& vec, Angle_t& out )
    {
        if ( vec.x == 0.0f && vec.y == 0.0f )
        {
            out.x = ( vec.z > 0.0f ) ? 270.0f : 90.0f;
            out.y = 0.0f;
        } else
        {
            out.x = std::atan2( -vec.z, vec.Length2D() ) * ( -180.f / static_cast<float>( std::numbers::pi ) );
            out.y = std::atan2( vec.y, vec.x ) * ( 180.f / static_cast<float>( std::numbers::pi ) );

            if ( out.y > 90.f )
                out.y -= 180.f;
            else if ( out.y < 90.f )
                out.y += 180.f;
            else if ( out.y == 90.f )
                out.y = 0.f;
        }

        out.z = 0.f;
    }


    void VectorAngles( const Vector3D_t& vec, const Vector3D_t& up, Angle_t& out )
    {
        const Vector3D_t left = up.CrossProduct( vec );

        if ( const auto dist = vec.Length2D(); dist > 0.001f )
        {
            out.y = Rad2Deg( std::atan2( vec.y, vec.x ) );
            out.x = Rad2Deg( std::atan2( -vec.z, dist ) );
            out.z = Rad2Deg( std::atan2( left.z, left.y * vec.x - left.x * vec.y ) );
        } else
        {
            out.y = Rad2Deg( std::atan2( -left.x, left.y ) );
            out.x = Rad2Deg( std::atan2( -vec.z, dist ) );
            out.z = 0.f;
        }
    }

    void CorrectMovement( const Angle_t& wish_angle, CUserCmd* cmd )
    {
        Vector3D_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
        auto viewangles = cmd->viewangles;
        viewangles.Normalize();

        AngleVectors( wish_angle, view_fwd, view_right, view_up );
        AngleVectors( viewangles, cmd_fwd, cmd_right, cmd_up );

        const auto v8 = sqrtf( ( view_fwd.x * view_fwd.x ) + ( view_fwd.y * view_fwd.y ) );
        const auto v10 = sqrtf( ( view_right.x * view_right.x ) + ( view_right.y * view_right.y ) );
        const auto v12 = sqrtf( view_up.z * view_up.z );

        const Vector3D_t norm_view_fwd( ( 1.f / v8 ) * view_fwd.x, ( 1.f / v8 ) * view_fwd.y, 0.f );
        const Vector3D_t norm_view_right( ( 1.f / v10 ) * view_right.x, ( 1.f / v10 ) * view_right.y, 0.f );
        const Vector3D_t norm_view_up( 0.f, 0.f, ( 1.f / v12 ) * view_up.z );

        const auto v14 = sqrtf( ( cmd_fwd.x * cmd_fwd.x ) + ( cmd_fwd.y * cmd_fwd.y ) );
        const auto v16 = sqrtf( ( cmd_right.x * cmd_right.x ) + ( cmd_right.y * cmd_right.y ) );
        const auto v18 = sqrtf( cmd_up.z * cmd_up.z );

        const Vector3D_t norm_cmd_fwd( ( 1.f / v14 ) * cmd_fwd.x, ( 1.f / v14 ) * cmd_fwd.y, 0.f );
        const Vector3D_t norm_cmd_right( ( 1.f / v16 ) * cmd_right.x, ( 1.f / v16 ) * cmd_right.y, 0.f );
        const Vector3D_t norm_cmd_up( 0.f, 0.f, ( 1.f / v18 ) * cmd_up.z );

        const auto v22 = norm_view_fwd.x * cmd->forwardmove;
        const auto v26 = norm_view_fwd.y * cmd->forwardmove;
        const auto v28 = norm_view_fwd.z * cmd->forwardmove;
        const auto v24 = norm_view_right.x * cmd->sidemove;
        const auto v23 = norm_view_right.y * cmd->sidemove;
        const auto v25 = norm_view_right.z * cmd->sidemove;
        const auto v30 = norm_view_up.x * cmd->upmove;
        const auto v27 = norm_view_up.z * cmd->upmove;
        const auto v29 = norm_view_up.y * cmd->upmove;

        cmd->forwardmove = ( ( ( ( norm_cmd_fwd.x * v24 ) + ( norm_cmd_fwd.y * v23 ) ) + ( norm_cmd_fwd.z * v25 ) ) + ( ( ( norm_cmd_fwd.x * v22 ) + ( norm_cmd_fwd.y * v26 ) ) + ( norm_cmd_fwd.z * v28 ) ) ) + ( ( ( norm_cmd_fwd.y * v30 ) + ( norm_cmd_fwd.x * v29 ) ) + ( norm_cmd_fwd.z * v27 ) );
        cmd->sidemove = ( ( ( ( norm_cmd_right.x * v24 ) + ( norm_cmd_right.y * v23 ) ) + ( norm_cmd_right.z * v25 ) ) + ( ( ( norm_cmd_right.x * v22 ) + ( norm_cmd_right.y * v26 ) ) + ( norm_cmd_right.z * v28 ) ) ) + ( ( ( norm_cmd_right.x * v29 ) + ( norm_cmd_right.y * v30 ) ) + ( norm_cmd_right.z * v27 ) );
        cmd->upmove = ( ( ( ( norm_cmd_up.x * v23 ) + ( norm_cmd_up.y * v24 ) ) + ( norm_cmd_up.z * v25 ) ) + ( ( ( norm_cmd_up.x * v26 ) + ( norm_cmd_up.y * v22 ) ) + ( norm_cmd_up.z * v28 ) ) ) + ( ( ( norm_cmd_up.x * v30 ) + ( norm_cmd_up.y * v29 ) ) + ( norm_cmd_up.z * v27 ) );

        cmd->forwardmove = std::clamp( cmd->forwardmove, -450.f, 450.f );
        cmd->sidemove = std::clamp( cmd->sidemove, -450.f, 450.f );
        cmd->upmove = std::clamp( cmd->upmove, -320.f, 320.f );
    }

    float NormalizeYaw( float yaw )
    {
        if (yaw > 180.0f)
            yaw -= 360.0f;
        else if (yaw < -180.0f)
            yaw += 360.0f;

        return yaw;
    }
}
