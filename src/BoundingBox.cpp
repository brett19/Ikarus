#include "BoundingBox.hpp"

namespace Halia
{
	BoundingBox::BoundingBox( )
	{
	};

	BoundingBox::BoundingBox( float p )
		: min( p ), max( p )
	{
	};

	BoundingBox::BoundingBox( Halia::Vector3 p )
		: min( p ), max( p )
	{
	};

	BoundingBox::BoundingBox( Halia::Vector3 mn, Halia::Vector3 mx )
		: min( mn ), max( mx )
	{
	};

	BoundingBox::~BoundingBox( )
	{
	};

	BoundingBox BoundingBox::operator/( float rhs ) const
	{
		return BoundingBox( min / rhs, max / rhs );
	};

	void BoundingBox::operator/=( float rhs )
	{
		(*this) = (*this) / rhs;
	};

	void BoundingBox::AddPoint( const Vector3& vec )
	{
		if( min.x > vec.x )
			min.x = vec.x;
		if( min.y > vec.y )
			min.y = vec.y;
		if( min.z > vec.z )
			min.z = vec.z;
		if( max.x < vec.x )
			max.x = vec.x;
		if( max.y < vec.y )
			max.y = vec.y;
		if( max.z < vec.z )
			max.z = vec.z;
	};

	void BoundingBox::AddTransformedBB( const BoundingBox& b, const Matrix4& mat )
	{
		AddPoint( mat.TransformCoord( Vector3( b.min.x, b.min.y, b.min.z ) ) );
		AddPoint( mat.TransformCoord( Vector3( b.min.x, b.max.y, b.min.z ) ) );
		AddPoint( mat.TransformCoord( Vector3( b.max.x, b.min.y, b.min.z ) ) );
		AddPoint( mat.TransformCoord( Vector3( b.min.x, b.min.y, b.max.z ) ) );
		AddPoint( mat.TransformCoord( Vector3( b.max.x, b.max.y, b.min.z ) ) );
		AddPoint( mat.TransformCoord( Vector3( b.max.x, b.min.y, b.max.z ) ) );
		AddPoint( mat.TransformCoord( Vector3( b.min.x, b.max.y, b.max.z ) ) );
		AddPoint( mat.TransformCoord( Vector3( b.max.x, b.max.y, b.max.z ) ) );
	};

	bool BoundingBox::CheckOutsideFrustum( const Frustum& f, const Matrix4& mat ) const
	{
		Halia::Vector3 v[] = 
		{
			mat.TransformCoord( Halia::Vector3( min.x, min.y, min.z ) ),
			mat.TransformCoord( Halia::Vector3( max.x, min.y, min.z ) ),
			mat.TransformCoord( Halia::Vector3( min.x, max.y, min.z ) ),
			mat.TransformCoord( Halia::Vector3( min.x, min.y, max.z ) ),
			mat.TransformCoord( Halia::Vector3( max.x, max.y, min.z ) ),
			mat.TransformCoord( Halia::Vector3( max.x, min.y, max.z ) ),
			mat.TransformCoord( Halia::Vector3( min.x, max.y, max.z ) ),
			mat.TransformCoord( Halia::Vector3( max.x, max.y, max.z ) )
		};

		for( int i = 0; i < 6; ++i )
		{
			int out = 0;
			int in = 0;
			for( int j = 0; j < 8; ++j )
			{
				if( f.p[i].DotProductCoord( v[j] ) < -2.0f )
					out++;
				else
					in++;
			}
			if( !in )
				return true;
			else if( out )
				return false;
		}
		return false;
	};
};