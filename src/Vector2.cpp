#include "Vector2.hpp"

namespace Halia
{
	Vector2::Vector2( )
		: x( 0 ), y( 0 )
	{
	};
	
	Vector2::Vector2( float x_, float y_ )
		: x( x_ ), y( y_ )
	{
	};

	Vector2::Vector2( float s )
		: x( s ), y( s )
	{
	};
};