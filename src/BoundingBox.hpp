#pragma once
#include "Frustum.hpp"
#include "Matrix4.hpp"
#include "Vector3.hpp"

namespace Halia
{
	class BoundingBox
	{
	public:
		BoundingBox( );
		BoundingBox( float p );
		BoundingBox( Halia::Vector3 p );
		BoundingBox( Halia::Vector3 mn, Halia::Vector3 mx );
		~BoundingBox( );

		BoundingBox operator/( float rhs ) const;
		void operator/=( float rhs );

		void AddPoint( const Vector3& vec );
		void AddTransformedBB( const BoundingBox& b, const Matrix4& mat );
		bool CheckOutsideFrustum( const Frustum& f, const Matrix4& mat ) const;

		Halia::Vector3 min, max;
	};
}