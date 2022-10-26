#pragma once

typedef unsigned long HFont;

enum FontFlag_t
{
    FONTFLAG_NONE,
    FONTFLAG_ITALIC = 0x001,
    FONTFLAG_UNDERLINE = 0x002,
    FONTFLAG_STRIKEOUT = 0x004,
    FONTFLAG_SYMBOL = 0x008,
    FONTFLAG_ANTIALIAS = 0x010,
    FONTFLAG_GAUSSIANBLUR = 0x020,
    FONTFLAG_ROTARY = 0x040,
    FONTFLAG_DROPSHADOW = 0x080,
    FONTFLAG_ADDITIVE = 0x100,
    FONTFLAG_OUTLINE = 0x200,
    FONTFLAG_CUSTOM = 0x400,
    FONTFLAG_BITMAP = 0x800,
};


struct ISurface_t
{
    auto DrawSetColor( int r, int g, int b, int a = 255 )
    {
        return Hikari::Memory->VirtualCall<void, 11>( this, r, g, b, a );
    }

    auto DrawFilledRect( int x, int y, int w, int h )
    {
        return Hikari::Memory->VirtualCall<void, 12>( this, x, y, w, h );
    }

    auto DrawOutlinedRect(int x, int y, int w, int h)
    {
        return Hikari::Memory->VirtualCall<void, 14>(this, x, y, w, h);
    }

    auto DrawLine(int x, int y, int x2, int y2)
    {
        return Hikari::Memory->VirtualCall<void, 15>(this, x, y, x2, y2);
    }

    auto DrawSetTextFont(HFont Font)
    {
        return Hikari::Memory->VirtualCall<void, 17>(this, Font);
    }

    auto DrawSetTextColor(int r, int g, int b, int a)
    {
        return Hikari::Memory->VirtualCall<void, 19>(this, r, g, b, a);
    }

    auto DrawSetTextPos(int x, int y)
    {
        return Hikari::Memory->VirtualCall<void, 20>(this, x, y );
    }

    auto DrawPrintText(const wchar_t* text, int text_length)
    {
        return Hikari::Memory->VirtualCall<void, 22>(this, text, text_length, 0);
    }

    auto CreatFont()
    {
        return Hikari::Memory->VirtualCall<unsigned long, 63>(this);
    }

    auto SetFontGlyphSet(unsigned long font, const char* name, int size, int weight, int blur, int scanlines, int flags, int min_range, int max_range)
    {
        return Hikari::Memory->VirtualCall<bool, 64>(this, font, name, size, weight, blur, scanlines, flags, min_range, max_range);
    }

    auto GetTextSize(unsigned long font, const wchar_t* text, int& width, int& height)
    {
        return Hikari::Memory->VirtualCall<void, 72>(this, font, text, std::ref(width), std::ref(height));
    }

    auto DrawColoredTexted( unsigned long font, int x, int y, int r, int g, int b, int a, const char* fmt, ... )
    {
        return Hikari::Memory->VirtualCallCdecl<void, 153>( this, font, x, y, r, g, b, a, fmt );
    }
};
