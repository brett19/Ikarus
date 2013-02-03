#pragma once

namespace Halia
{
	class Quarternionf
	{
	public:
		Quarternionf( )
			: x( 0 ), y( 0 ), z( 0 ), w( 1 )
		{
		};

		Quarternionf( float x_, float y_, float z_, float w_ )
			: x( x_ ), y( y_ ), z( z_ ), w( w_ )
		{
		};

		Quarternionf( float s )
			: x( s ), y( s ), z( s ), w( s )
		{
		};

		float x, y, z, w;
	};
};