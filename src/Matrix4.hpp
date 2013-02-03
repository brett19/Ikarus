#pragma once
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quarternion.hpp"

namespace Halia
{
	class Matrix4
	{
	public:
		Matrix4( );
		Matrix4( float* s );
		Matrix4( float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44 );
		
		Matrix4 operator*( const Matrix4& rhs ) const;
		void operator*=( const Matrix4& rhs );

		float Determinant( ) const;
		Vector3 TransformCoord( const Vector3& v ) const;

		static Matrix4 CreateInverse( const Matrix4& in );
		static Matrix4 CreatePerspectiveRH( float w, float h, float zn, float zf );
		static Matrix4 CreatePerspectiveLH( float w, float h, float zn, float zf );
		static Matrix4 CreatePerspectiveFovRH( float fovy, float aspect, float zn, float zf );
		static Matrix4 CreateRotation( const Quarternion& rot );
		static Matrix4 CreateTranslation( Vector3 pos );
		static Matrix4 CreateTranslation2d( Vector3 pos );
		static Matrix4 CreateRotation( float yaw, float pitch, float roll );
		static Matrix4 CreateScaling( Vector3 scale );
		static Matrix4 CreateRotationX( float angle );
		static Matrix4 CreateRotationY( float angle );
		static Matrix4 CreateRotationZ( float angle );
		static Matrix4 CreateLookAtRH( const Vector3& eye, const Vector3& target, const Vector3& up );
		static Matrix4 CreateOrthoLH( float w, float h, float zn, float zf );

		float m[4][4];

		static const Matrix4 ZERO;
		static const Matrix4 IDENTITY;
	};
};