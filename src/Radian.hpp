#pragma once

namespace Halia
{
	class Radian
	{
	public:
		Radian( );
		Radian( float value_ );

		Radian operator-( ) const;
		Radian operator-( const Radian& rhs ) const;
		void operator-=( const Radian& rhs );

		void Normalize( );

		float value;
	};
};