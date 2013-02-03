#pragma once

namespace Halia
{
	class Vector4
	{
	public:
		Vector4( );
		Vector4( float x_, float y_, float z_, float w_ );
		Vector4( float s );

		static Vector4 CreateCross( const Vector4& v1, const Vector4& v2, const Vector4& v3 );

		float x, y, z, w;
	};
};