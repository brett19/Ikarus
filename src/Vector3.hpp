#pragma once

namespace Halia
{
	class Vector3
	{
	public:
		Vector3( );
		Vector3( float x_, float y_, float z_ );
		Vector3( float s );

		Vector3 operator+( const Vector3& rhs ) const;
		Vector3 operator-( const Vector3& rhs ) const;
		Vector3 operator*( const Vector3& rhs ) const;
		Vector3 operator/( const Vector3& rhs ) const;
		void operator+=( const Vector3& rhs );
		void operator-=( const Vector3& rhs );
		void operator*=( const Vector3& rhs );
		void operator/=( const Vector3& rhs );

		float Length( );
		float LengthSq( );
		float Normalize( );

		static Vector3 CreateCross( const Vector3& v1, const Vector3& v2 );
		static Vector3 CreateNormalized( const Vector3& in );
		static float CreateDotProduct( const Vector3& v1, const Vector3& v2 );

		float x, y, z;
	};
};