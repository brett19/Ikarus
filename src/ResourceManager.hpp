#pragma once
#include <queue>
#include "Event.hpp"
#include "Thread.hpp"
#include "CriticalSection.hpp"
#include "Resource.hpp"
#include "ResourceGroup.hpp"

namespace Halia
{
	class ResourceManager : public Thread
	{
	public:
		ResourceManager( );
		~ResourceManager( );

		int Execute( );
		void QueueResource( Resource* res );

	protected:
		std::queue< Resource* > mQueue;
		Sync::Event mHasQueued;
		Sync::CriticalSection mQueueLock;
	};
};