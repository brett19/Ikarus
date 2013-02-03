#pragma once
#include "Events.hpp"

namespace Halia
{
	class EventListener
	{
	public:
		EventListener( )
		{
		};

		~EventListener( )
		{
		};

		virtual bool EventHandler( Event* evt ) = 0;

	private:

	};
};