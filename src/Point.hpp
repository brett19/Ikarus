#pragma once

namespace Halia
{
	class Point
	{
	public:
		Point( );
		Point( int s );
		Point( int x_, int y_ );
		~Point( );

		Point operator+( const Point& rhs ) const;

		int x, y;
	};
};