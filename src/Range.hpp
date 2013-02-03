#pragma once

template< typename T >
class Range
{
public:
	Range( )
	{
	};

	Range( T val )
		: min( val ), max( val )
	{
	};

	Range( T min_, T max_ )
		: min( min_ ), max( max_ )
	{
	};

	~Range( )
	{
	};

	T min;
	T max;
};