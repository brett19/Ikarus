#include "Point.hpp"

namespace Halia
{
	Point::Point( )
		: x( 0 ), y( 0 )
	{
	};

	Point::Point( int s )
		: x( s ), y( s )
	{
	};

	Point::Point( int x_, int y_ )
		: x( x_ ), y( y_ )
	{
	};

	Point::~Point( )
	{
	};

	Point Point::operator+( const Point& rhs ) const
	{
		return Point( x + rhs.x, y + rhs.y );
	};
};