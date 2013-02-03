#include "ResourceManager.hpp"

namespace Halia
{
	ResourceManager::ResourceManager( )
		: Thread( true )
	{
	};

	ResourceManager::~ResourceManager( )
	{
	};

	int ResourceManager::Execute( )
	{
		while( IsRunning( ) )
		{
			mHasQueued.WaitFor( );
			
			mQueueLock.Enter( );
			while( !mQueue.empty( ) )
			{
				Resource* res = mQueue.front( );
				mQueue.pop( );
				mQueueLock.Leave( );

				res->Load( );

				mQueueLock.Enter( );
			}
			mHasQueued.Reset( );
			mQueueLock.Leave( );
		}

		return 0;
	};

	void ResourceManager::QueueResource( Resource* res )
	{
		mQueueLock.Enter( );
		mQueue.push( res );
		mHasQueued.Set( );
		mQueueLock.Leave( );
	};
};