#pragma once
#include <vector>
#include "Events.hpp"

namespace Halia
{
	class EventListener;

	class EventDispatcher
	{
		struct ListenData
		{
			int mEventType;
			EventListener* mListener;
		};

	public:
		EventDispatcher( );
		~EventDispatcher( );

		void DispatchEvent( Event* evt );
		void AddListener( int type, EventListener* handler );

	private:
		std::vector<ListenData> mListeners;
	};
};