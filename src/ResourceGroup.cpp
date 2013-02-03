#include "ResourceGroup.hpp"

namespace Halia
{
	ResourceGroup::ResourceGroup( )
	{
	};

	ResourceGroup::~ResourceGroup( )
	{
	};

	void ResourceGroup::AddResource( Resource* res )
	{
		res->AddRef( );
		mResources.push_back( res );
	};

	bool ResourceGroup::DelResource( Resource*& res )
	{
		for( std::vector< Resource* >::iterator i = mResources.begin( ); i != mResources.end( ); i++ )
		{
			if( (*i) == res )
			{
				mResources.erase( i );
				(*i)->DelRef( );
				return true;
			}
		}
		return false;
	};

	bool ResourceGroup::Load( )
	{
		bool retval = true;
		for( std::vector< Resource* >::iterator i = mResources.begin( ); i != mResources.end( ); i++ )
		{
			if( !(*i)->Load( ) )
				retval = false;
		}
		mLoaded = true;
		return retval;
	};

	void ResourceGroup::Unload( )
	{
		for( std::vector< Resource* >::iterator i = mResources.begin( ); i != mResources.end( ); i++ )
		{
			(*i)->Unload( );
		}
	};
};