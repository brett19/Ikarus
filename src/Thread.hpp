#pragma once
#include <windows.h>
#include <process.h>

namespace Halia
{
	class Thread
	{
	public:
		Thread( bool autoexec = false )
		{
			mRunning = true;
			mActive = false;

			mThread = (HANDLE)_beginthreadex( 0, 0, &_Execute, this, CREATE_SUSPENDED, 0 );

			if( autoexec )
				Resume( );
		};

		~Thread( )
		{
#ifdef _DEBUG
			if( mRunning || mActive )
				printf( "Thread deleted prior to ending" );
#endif
			CloseHandle( mThread );
		};

		void Resume( )
		{
			if( mActive )
				return;
			mActive = true;
			ResumeThread( mThread );
		};

		void Suspend( )
		{
			if( !mActive )
				return;
			mActive = false;
			SuspendThread( mThread );
		};

		void Terminate( )
		{
			mRunning = false;
			WaitForSingleObject( mThread, INFINITE );
		};

		inline bool IsRunning( ) const
		{
			return mRunning;
		};

		virtual int Execute( )
		{
			return 0;
		};

		static unsigned int __stdcall _Execute( void* a )
		{
			return ((Thread*)a)->Execute( );
		};

	private:
		bool mRunning;
		bool mActive;
		HANDLE mThread;
	};
};