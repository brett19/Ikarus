#include "Vector4.hpp"

namespace Halia
{
	Vector4::Vector4( )
		: x( 0 ), y( 0 ), z( 0 ), w( 0 )
	{
	};
	
	Vector4::Vector4( float x_, float y_, float z_, float w_ )
		: x( x_ ), y( y_ ), z( z_ ), w( w_ )
	{
	};

	Vector4::Vector4( float s )
		: x( s ), y( s ), z( s ), w( s )
	{
	};

	Vector4 Vector4::CreateCross( const Vector4& v1, const Vector4& v2, const Vector4& v3 )
	{
		Vector4 v;
		v.x = v1.y * ( v2.z * v3.w - v3.z * v2.w ) - v1.z * ( v2.y * v3.w - v3.y * v2.w ) + v1.w * ( v2.y * v3.z - v2.z * v3.y );
		v.y = -(v1.x * ( v2.z * v3.w - v3.z * v2.w ) - v1.z * ( v2.x * v3.w - v3.x * v2.w ) + v1.w * ( v2.x * v3.z - v3.x * v2.z ) );
		v.z = v1.x * ( v2.y * v3.w - v3.y * v2.w ) - v1.y * ( v2.x * v3.w - v3.x * v2.w ) + v1.w * ( v2.x * v3.y - v3.x * v2.y );
		v.w = -( v1.x * ( v2.y * v3.z - v3.y * v2.z ) - v1.y * ( v2.x * v3.z - v3.x * v2.z ) + v1.z * ( v2.x * v3.y - v3.x * v2.y ) );
		return v;
	};
};