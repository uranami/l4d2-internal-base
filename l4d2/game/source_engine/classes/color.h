#pragma once

#pragma warning(push)
#pragma warning(disable:4333 4244)

#define DEFCOLOR_SRC(name, r, g, b) static Color name##(uint8_t a = 255){ \
                                                       return Color(r, g, b, a); \
                                                  }

class Color
{
public:
    // constructors
    Color()
    {
        *reinterpret_cast<int*>( this ) = 0;
    }

    Color( uint8_t r, uint8_t g, uint8_t b )
    {
        SetColor( r, g, b, 255 );
    }

    Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
    {
        SetColor( r, g, b, a );
    }

    void SetColor( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0 )
    {
        _color[ 0 ] = r;
        _color[ 1 ] = g;
        _color[ 2 ] = b;
        _color[ 3 ] = a;
    }

    void GetColor( uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a ) const
    {
        r = _color[ 0 ];
        g = _color[ 1 ];
        b = _color[ 2 ];
        a = _color[ 3 ];
    }

    void GetColor( int& _r, int& _g, int& _b, int& _a ) const
    {
        _r = _color[ 0 ];
        _g = _color[ 1 ];
        _b = _color[ 2 ];
        _a = _color[ 3 ];
    }

    static Color FromARGB( unsigned long color )
    {
        int a = ( color & 0xFF000000 ) >> 24;
        int r = ( color & 0x00FF0000 ) >> 16;
        int g = ( color & 0x0000FF00 ) >> 8;
        int b = ( color & 0x000000FF );

        return Color( r, g, b, a );
    }

    void SetARGB( unsigned long color )
    {
        int a = ( color & 0xFF000000 ) >> 24;
        int r = ( color & 0x00FF0000 ) >> 16;
        int g = ( color & 0x0000FF00 ) >> 8;
        int b = ( color & 0x000000FF );

        SetColor( r, g, b, a );
    }

    DWORD GetARGB() const
    {
        const int a = ( _color[ 3 ] ) >> 24;
        const int r = ( _color[ 0 ] ) >> 16;
        const int g = ( _color[ 1 ] ) >> 8;
        const int b = ( _color[ 2 ] );

        return a | r | g | b;
    }

    static Color FromHSB( const float hue, const float saturation, const float brightness )
    {
        const float h = hue == 1.0f ? 0 : hue * 6.0f;
        const float f = h - static_cast<int>( h );
        const float p = brightness * ( 1.0f - saturation );
        const float q = brightness * ( 1.0f - saturation * f );
        const float t = brightness * ( 1.0f - ( saturation * ( 1.0f - f ) ) );

        if ( h < 1 )
        {
            return Color( static_cast<unsigned char>( brightness * 255 ), static_cast<unsigned char>( t * 255 ), static_cast<unsigned char>( p * 255 ) );
        }

        if ( h < 2 )
        {
            return Color( static_cast<unsigned char>( q * 255 ), static_cast<unsigned char>( brightness * 255 ), static_cast<unsigned char>( p * 255 ) );
        }

        if ( h < 3 )
        {
            return Color( static_cast<unsigned char>( p * 255 ), static_cast<unsigned char>( brightness * 255 ), static_cast<unsigned char>( t * 255 ) );
        }

        if ( h < 4 )
        {
            return Color( static_cast<unsigned char>( p * 255 ), static_cast<unsigned char>( q * 255 ), static_cast<unsigned char>( brightness * 255 ) );
        }

        if ( h < 5 )
        {
            return Color( static_cast<unsigned char>( t * 255 ), static_cast<unsigned char>( p * 255 ), static_cast<unsigned char>( brightness * 255 ) );
        }

        return Color( static_cast<unsigned char>( brightness * 255 ), static_cast<unsigned char>( p * 255 ), static_cast<unsigned char>( q * 255 ) );
    }

    uint8_t& operator[]( int index )
    {
        return _color[ index ];
    }

    const uint8_t& operator[]( const int& index ) const
    {
        return _color[ index ];
    }

    bool operator ==( const Color& rhs ) const
    {
        return ( *( ( int* )this ) == *( ( int* )&rhs ) );
    }

    bool operator !=( const Color& rhs ) const
    {
        return !( operator==( rhs ) );
    }

    int r() const
    {
        return _color[ 0 ];
    }

    int g() const
    {
        return _color[ 1 ];
    }

    int b() const
    {
        return _color[ 2 ];
    }

    int a() const
    {
        return _color[ 3 ];
    }

    void SetR( const uint8_t& _i )
    {
        _color[ 0 ] = _i;
    }

    void SetG( const uint8_t& _i )
    {
        _color[ 1 ] = _i;
    }

    void SetB( const uint8_t& _i )
    {
        _color[ 2 ] = _i;
    }

    void SetA( const uint8_t& _i )
    {
        _color[ 3 ] = _i;
    }

    Color& operator=( const Color& rhs )
    {
        *reinterpret_cast<int*>( &_color[ 0 ] ) = *( int* )&rhs._color[ 0 ];
        return *this;
    }

    Color operator+( const Color& rhs ) const
    {
        int red = _color[ 0 ] + rhs._color[ 0 ];
        int green = _color[ 1 ] + rhs._color[ 1 ];
        int blue = _color[ 2 ] + rhs._color[ 2 ];
        int alpha = _color[ 3 ] + rhs._color[ 3 ];

        red = red > 255 ? 255 : red;
        green = green > 255 ? 255 : green;
        blue = blue > 255 ? 255 : blue;
        alpha = alpha > 255 ? 255 : alpha;

        return Color( red, green, blue, alpha );
    }

    Color operator-( const Color& rhs ) const
    {
        int red = _color[ 0 ] - rhs._color[ 0 ];
        int green = _color[ 1 ] - rhs._color[ 1 ];
        int blue = _color[ 2 ] - rhs._color[ 2 ];
        int alpha = _color[ 3 ] - rhs._color[ 3 ];

        red = red < 0 ? 0 : red;
        green = green < 0 ? 0 : green;
        blue = blue < 0 ? 0 : blue;
        alpha = alpha < 0 ? 0 : alpha;
        return Color( red, green, blue, alpha );
    }

    float* Base() const
    {
        float clr[ 3 ];

        clr[ 0 ] = _color[ 0 ] / 255.0f;
        clr[ 1 ] = _color[ 1 ] / 255.0f;
        clr[ 2 ] = _color[ 2 ] / 255.0f;

        return &clr[ 0 ];
    }

    float rBase() const
    {
        return _color[ 0 ] / 255.0f;
    }

    float gBase() const
    {
        return _color[ 1 ] / 255.0f;
    }

    float bBase() const
    {
        return _color[ 2 ] / 255.0f;
    }

    float aBase() const
    {
        return _color[ 3 ] / 255.0f;
    }

    operator const uint8_t*() const
    {
        return &_color[ 0 ];
    }

    DEFCOLOR_SRC( Black, 0, 0, 0 );
    DEFCOLOR_SRC( White, 255, 255, 255 );
    DEFCOLOR_SRC( Red, 255, 0, 0 );
    DEFCOLOR_SRC( Green, 0, 128, 0 );
    DEFCOLOR_SRC( Blue, 0, 0, 255 );
    DEFCOLOR_SRC( Lime, 0, 255, 0 );
    DEFCOLOR_SRC( Yellow, 255, 255, 0 );
    DEFCOLOR_SRC( Cyan, 0, 255, 255 );
    DEFCOLOR_SRC( Magenta, 255, 0, 255 );
    DEFCOLOR_SRC( Silver, 192, 192, 192 );
    DEFCOLOR_SRC( Gray, 128, 128, 128 );
    DEFCOLOR_SRC( Maroon, 128, 0, 0 );
    DEFCOLOR_SRC( Olive, 128, 128, 0 );
    DEFCOLOR_SRC( Purple, 128, 0, 128 );
    DEFCOLOR_SRC( Teal, 0, 128, 128 );
    DEFCOLOR_SRC( Navy, 0, 0, 128 );
    DEFCOLOR_SRC( DarkRed, 139, 0, 0 );
    DEFCOLOR_SRC( Brown, 165, 42, 42 );
    DEFCOLOR_SRC( Firebrick, 178, 34, 34 );
    DEFCOLOR_SRC( Crimson, 220, 20, 60 );
    DEFCOLOR_SRC( IndianRed, 205, 92, 92 );
    DEFCOLOR_SRC( LightCoral, 240, 128, 128 );
    DEFCOLOR_SRC( DarkSalmon, 233, 150, 122 );
    DEFCOLOR_SRC( Salmon, 250, 128, 114 );
    DEFCOLOR_SRC( LightSalmon, 255, 160, 122 );
    DEFCOLOR_SRC( OrangeRed, 255, 69, 0 );
    DEFCOLOR_SRC( DarkOrange, 255, 140, 0 );
    DEFCOLOR_SRC( Orange, 255, 165, 0 );
    DEFCOLOR_SRC( Gold, 255, 215, 0 );
    DEFCOLOR_SRC( DarkGoldenRod, 184, 134, 11 );
    DEFCOLOR_SRC( GoldenRod, 218, 165, 32 );
    DEFCOLOR_SRC( YellowGreen, 154, 205, 50 );
    DEFCOLOR_SRC( DarkOliveGreen, 85, 107, 47 );
    DEFCOLOR_SRC( OliveDrab, 107, 142, 35 );
    DEFCOLOR_SRC( LawnGreen, 124, 252, 0 );
    DEFCOLOR_SRC( ChartReuse, 127, 255, 0 );
    DEFCOLOR_SRC( GreenYellow, 173, 255, 47 );
    DEFCOLOR_SRC( DarkGreen, 0, 100, 0 );
    DEFCOLOR_SRC( ForestGreen, 34, 139, 34 );
    DEFCOLOR_SRC( LimeGreen, 50, 205, 50 );
    DEFCOLOR_SRC( DarkCyan, 0, 139, 139 );
    DEFCOLOR_SRC( Aqua, 0, 255, 255 );
    DEFCOLOR_SRC( LightCyan, 224, 255, 255 );
    DEFCOLOR_SRC( DarkTurquoise, 0, 206, 209 );
    DEFCOLOR_SRC( Turquoise, 64, 224, 208 );
    DEFCOLOR_SRC( MediumTurquoise, 72, 209, 204 );
    DEFCOLOR_SRC( PaleTurquoise, 175, 238, 238 );
    DEFCOLOR_SRC( Aquamarine, 127, 255, 212 );
    DEFCOLOR_SRC( PowderBlue, 176, 224, 230 );
    DEFCOLOR_SRC( DodgerBlue, 30, 144, 255 );
    DEFCOLOR_SRC( Lightblue, 173, 216, 230 );
    DEFCOLOR_SRC( SkyBlue, 135, 206, 235 );
    DEFCOLOR_SRC( LightSkyBlue, 135, 206, 250 );
    DEFCOLOR_SRC( MidnightBlue, 25, 25, 112 );
    DEFCOLOR_SRC( DarkBlue, 0, 0, 139 );
    DEFCOLOR_SRC( MediumBlue, 0, 0, 205 );
    DEFCOLOR_SRC( RoyalBlue, 65, 105, 225 );
    DEFCOLOR_SRC( BlueViolet, 138, 43, 226 );
    DEFCOLOR_SRC( Indigo, 75, 0, 130 );
    DEFCOLOR_SRC( DarkSlateBlue, 72, 61, 139 );
    DEFCOLOR_SRC( SlateBlue, 106, 90, 205 );
    DEFCOLOR_SRC( MediumSlateBlue, 123, 104, 238 );
    DEFCOLOR_SRC( MediumPurple, 147, 112, 219 );
    DEFCOLOR_SRC( Darkmagenta, 139, 0, 139 );
    DEFCOLOR_SRC( Darkviolet, 148, 0, 211 );
    DEFCOLOR_SRC( DarkGray, 169, 169, 169 );
    DEFCOLOR_SRC( LightGray, 211, 211, 211 );
    DEFCOLOR_SRC( Gainsboro, 220, 220, 220 );
private:
    uint8_t _color[ 4 ];
};

#pragma warning(pop)
