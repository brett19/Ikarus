#include "Math.hpp"
#include "Radian.hpp"

namespace Halia
{
	Radian::Radian( )
		: value( 0 )
	{
	};

	Radian::Radian( float value_ )
		: value( value_ )
	{
	};

	Radian Radian::operator-( ) const
	{
		return Radian( -value );
	};

	Radian Radian::operator-( const Radian& rhs ) const
	{
		return Radian( rhs.value - value );
	};

	void Radian::operator-=( const Radian& rhs )
	{
		*this = *this - rhs;
	};

	void Radian::Normalize( )
	{
		while( value >= 2 * Math::PI ) value -= 2 * Math::PI;
		while( value < 0 ) value += 2 * Math::PI;
	}; 
};