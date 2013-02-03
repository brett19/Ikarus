#include <math.h>
#include "Vector3.hpp"

namespace Halia
{
	Vector3::Vector3( )
		: x( 0 ), y( 0 ), z( 0 )
	{
	};

	Vector3::Vector3( float x_, float y_, float z_ )
		: x( x_ ), y( y_ ), z( z_ )
	{
	};

	Vector3::Vector3( float s )
		: x( s ), y( s ), z( s )
	{
	};

	Vector3 Vector3::operator+( const Vector3& rhs ) const
	{
		return Vector3( x + rhs.x, y + rhs.y, z + rhs.z );
	};

	Vector3 Vector3::operator-( const Vector3& rhs ) const
	{
		return Vector3( x - rhs.x, y - rhs.y, z - rhs.z );
	};

	Vector3 Vector3::operator*( const Vector3& rhs ) const
	{
		return Vector3( x * rhs.x, y * rhs.y, z * rhs.z );
	};

	Vector3 Vector3::operator/( const Vector3& rhs ) const
	{
		return Vector3( x / rhs.x, y / rhs.y, z / rhs.z );
	};

	void Vector3::operator+=( const Vector3& rhs )
	{
		(*this) = (*this) + rhs;
	};

	void Vector3::operator-=( const Vector3& rhs )
	{
		(*this) = (*this) - rhs;
	};

	void Vector3::operator*=( const Vector3& rhs )
	{
		(*this) = (*this) * rhs;
	};

	void Vector3::operator/=( const Vector3& rhs )
	{
		(*this) = (*this) / rhs;
	};

	float Vector3::Length( )
	{
		return sqrt( x * x + y * y + z * z );
	};

	float Vector3::LengthSq( )
	{
		return x * x + y * y + z * z;
	};

	float Vector3::Normalize( )
	{
		float norm  = Length( );

		if( !norm ) {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		} else {
			x = x / norm;
			y = y / norm;
			z = z / norm;
		}

		return norm;
	};

	Vector3 Vector3::CreateCross( const Vector3& v1, const Vector3& v2 )
	{
		Vector3 v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
	};

	Vector3 Vector3::CreateNormalized( const Vector3& in )
	{
		Vector3 vec = in;
		vec.Normalize( );
		return vec;
	};

	float Vector3::CreateDotProduct( const Vector3& v1, const Vector3& v2 )
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	};
};