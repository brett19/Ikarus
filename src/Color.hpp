#pragma once

namespace Halia
{
	class Color
	{
	public:
		Color( );
		Color( float r_, float g_, float b_ );
		Color( float a_, float r_, float g_, float b_ );
		Color( unsigned int color );

		unsigned int ToDword( ) const;

		float a, r, g, b;

		static const Color WHITE;
		static const Color BLACK;
		static const Color RED;
		static const Color BLUE;
		static const Color GREEN;
	};
};