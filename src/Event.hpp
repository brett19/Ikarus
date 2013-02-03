#pragma once
#include <windows.h>
#include <process.h>

namespace Halia
{
	namespace Sync
	{
		class Event
		{
		public:
			inline Event( )
			{
				mEvent = CreateEvent( 0, 1, 0, 0 );
			};

			inline ~Event( )
			{
				CloseHandle( mEvent );
			};

			inline void Set( )
			{
				SetEvent( mEvent );
			};

			inline void Reset( )
			{
				ResetEvent( mEvent );
			};

			inline void WaitFor( )
			{
				WaitForSingleObject( mEvent, INFINITE );
			};

		private:
			HANDLE mEvent;
		};
	};
};