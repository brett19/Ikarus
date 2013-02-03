#include "Color.hpp"

namespace Halia
{
	Color::Color( )
		: a( 1.0f ), r( 0.0f ), g( 0.0f ), b( 0.0f )
	{
	};

	Color::Color( float r_, float g_, float b_ )
		: a( 1.0f ), r( r_ ), g( g_ ), b( b_ )
	{
	};

	Color::Color( float a_, float r_, float g_, float b_ )
		: a( a_ ), r( r_ ), g( g_ ), b( b_ )
	{
	};

	Color::Color( unsigned int color )
	{
		a = (float)( ( color >> 24 ) & 0xFF ) / 0xFF;
		r = (float)( ( color >> 16 ) & 0xFF ) / 0xFF;
		g = (float)( ( color >> 8 ) & 0xFF ) / 0xFF;
		b = (float)( ( color >> 0 ) & 0xFF ) / 0xFF;
	};

	unsigned int Color::ToDword( ) const
	{
		return ( (int)( a * 255 ) << 24 ) | ( (int)( r * 255 ) << 16 ) | ( (int)( g * 255 ) << 8 ) | ( (int)( b * 255 ) << 0 );
	};

	const Color Color::WHITE = Color( 1.0f, 1.0f, 1.0f );
	const Color Color::BLACK = Color( 0.0f, 0.0f, 0.0f );
	const Color Color::RED = Color( 1.0f, 0.0f, 0.0f );
	const Color Color::GREEN = Color( 0.0f, 1.0f, 0.0f );
	const Color Color::BLUE = Color( 0.0f, 0.0f, 1.0f );
};