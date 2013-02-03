#pragma once
#include <math.h>
#include "Vector3.hpp"

namespace Halia
{
	class Plane
	{
	public:
		Plane( );
		Plane( float d );
		Plane( float a, float b, float c, float d );
		~Plane( );

		float Normalize( );
		float DotProductCoord( const Vector3& v ) const;

		float m[4];
	};
};