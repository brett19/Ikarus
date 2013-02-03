#include "BoundingSphere.hpp"

namespace Halia
{
	BoundingSphere::BoundingSphere( )
		: radius( 0 )
	{
	};

	BoundingSphere::~BoundingSphere( )
	{
	};

	bool BoundingSphere::CheckOutsideFrustum( const Frustum& f, const Matrix4& mat ) const
	{
		Halia::Vector3 realcenter = mat.TransformCoord( center );

		for( int i = 0; i < 6; ++i )
		{
			if( f.p[i].DotProductCoord( realcenter ) < -radius )
				return true;
		}

		return false;
	};
};