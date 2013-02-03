#pragma once
#include "Frustum.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"

namespace Halia
{
	class BoundingSphere
	{
	public:
		BoundingSphere( );
		~BoundingSphere( );

		bool CheckOutsideFrustum( const Frustum& f, const Matrix4& mat ) const;

		Halia::Vector3 center;
		float radius;
	};
}