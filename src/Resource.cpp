#include "Resource.hpp"

namespace Halia
{
	Resource::Resource( )
		: mLoaded( false )
	{
	};

	Resource::~Resource( )
	{
	};

	bool Resource::IsLoaded( ) const
	{
		return mLoaded;
	};
};