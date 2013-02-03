#pragma once
#include <windows.h>

namespace Halia
{
	namespace Sync
	{
		class CriticalSection
		{
		public:
			inline CriticalSection( )
			{
				InitializeCriticalSection( &mCs );
			};

			inline ~CriticalSection( )
			{
				DeleteCriticalSection( &mCs );
			};

			inline bool TryEnter( )
			{
				return TryEnterCriticalSection( &mCs ) == 0;
			};

			inline void Enter( )
			{
				EnterCriticalSection( &mCs );
			};

			inline void Leave( )
			{
				LeaveCriticalSection( &mCs );
			};

		private:
			CRITICAL_SECTION mCs;
		};
	};
};