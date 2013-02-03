#include "Quarternion.hpp"

namespace Halia
{
	Quarternion::Quarternion( )
		: x( 0 ), y( 0 ), z( 0 ), w( 1 )
	{
	};

	Quarternion::Quarternion( float x_, float y_, float z_, float w_ )
		: x( x_ ), y( y_ ), z( z_ ), w( w_ )
	{
	};

	Quarternion::Quarternion( float s )
		: x( s ), y( s ), z( s ), w( s )
	{
	};
};