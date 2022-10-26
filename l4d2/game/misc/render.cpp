#include "../../framework.h"
#include "../game.h"

namespace Game::Render
{
    void InitFont()
    {
        Interfaces->Surface->SetFontGlyphSet( Fonts::ESP = Interfaces->Surface->CreatFont(), "Tahoma", 12, 700, 0, 0, FONTFLAG_DROPSHADOW, 0, 0 );
        Interfaces->Surface->SetFontGlyphSet( Fonts::WaterMark = Interfaces->Surface->CreatFont(), "Tahoma", 13, 700, 0, 0, FONTFLAG_DROPSHADOW, 0, 0 );
    }

    void DrawFilledRect( const int x, const int y, const int w, const int h, Color col )
    {
        Interfaces->Surface->DrawSetColor( col.r(), col.g(), col.b(), col.a() );
        Interfaces->Surface->DrawFilledRect( x, y, x + w, y + h );
    }

    void DrawOutlinedRect( const int x, const int y, const int w, const int h, Color col )
    {
        Interfaces->Surface->DrawSetColor( col.r(), col.g(), col.b(), col.a() );
        Interfaces->Surface->DrawOutlinedRect( x, y, x + w, y + h );
    }

    void DrawLine( const int x, const int y, const int w, const int h, Color col )
    {
        Interfaces->Surface->DrawSetColor( col.r(), col.g(), col.b(), col.a() );
        Interfaces->Surface->DrawLine( x, y, w, h );
    }

    void DrawOutlinedBox( int x, int y, int w, int h, Color col, Color col2 )
    {
        DrawOutlinedRect( x - 1, y - 1, w + 2, h + 2, col2 );
        DrawOutlinedRect( x, y, w, h, col );
        DrawOutlinedRect( x + 1, y + 1, w - 2, h - 2, col2 );
    }

    Vector2D_t GetTextSize( const DWORD& font, const std::string& text )
    {
        const auto buf = Hikari::String::ToUnicode( text );
        if ( buf.empty() )
            return {};

        int width, height;
        Interfaces->Surface->GetTextSize( font, buf.data(), width, height );

        return { static_cast<float>( width ), static_cast<float>( height ) };
    }

    void DrawText_( const HFont font, const std::string& text, const int x, const int y, const bool centered, Color col )
    {
        int _x = x;
        const int _y = y;

        const auto buf = Hikari::String::ToUnicode( text );
        if ( buf.empty() )
            return;

        int width, height;
        Interfaces->Surface->GetTextSize( font, buf.data(), width, height );

        if ( centered )
            _x -= width / 2;

        Interfaces->Surface->DrawSetTextFont( font );
        Interfaces->Surface->DrawSetTextColor( col.r(), col.g(), col.b(), col.a() );
        Interfaces->Surface->DrawSetTextPos( _x, _y );
        Interfaces->Surface->DrawPrintText( buf.c_str(), buf.length() );
    }
}

namespace Game::Fonts
{
    HFont ESP = NULL;
    HFont WaterMark = NULL;
}
