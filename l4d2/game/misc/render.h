#pragma once

namespace Game::Render
{
    void InitFont();

    void DrawFilledRect( int x, int y, int w, int h, Color col );

    void DrawOutlinedRect( int x, int y, int w, int h, Color col );

    void DrawLine( int x, int y, int w, int h, Color col );

    void DrawOutlinedBox( int x, int y, int w, int h, Color col, Color col2 = Color( 10, 10, 10, 190 ) );

    Vector2D_t GetTextSize( const DWORD& font, const std::string& text );

    void DrawText_( HFont font, const std::string& text, int x, int y, bool centered, Color col );
}

namespace Game::Fonts
{
    extern HFont ESP;
    extern HFont WaterMark;
}
