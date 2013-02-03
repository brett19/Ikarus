#pragma once
#include "Plane.hpp"
#include "Matrix4.hpp"

namespace Halia
{
	class Frustum
	{
	public:
		Frustum( );
		~Frustum( );

		static Frustum CreateFromMatrix( const Matrix4& mat );

		Plane p[6];
	};
};