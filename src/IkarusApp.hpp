#pragma once
#include "Halia.hpp"
#include "RoseEvents.hpp"
#include "RoseNetwork.hpp"
#include "GSDebug.hpp"
#include "GSInitialLoad.hpp"
#include "GSLogin.hpp"

namespace GameState
{
	enum GameState
	{
		DEBUG,
		INITIALLOAD,
		LOGIN,
		LOADINGMAP,
		INGAME
	};
};

class IkarusApp
	: public Halia::FrameListener,
		public Halia::ScriptListener,
		public Halia::EventListener
{
public:
	int Go( )
	{
		//:: Startup the engine
		Halia::Init( );

		//:: Initialize the filesystem & logging
		HLOGGER->Open( "ikarus.log" );
		HFSM->AddFs( "master", new Halia::WindowsOSFileSystem( "C:\\RuffVFS\\" ) );
		//HFSM->AddFs( "master", new Halia::WindowsOSFileSystem( "C:\\Program Files\\Triggersoft\\RuffVFS\\" ) );
		//HFSM->AddFs( "rosevfs", new RoseVfsFileSystem( "data.idx" ) );

		//:: Initialize the engine
		HENGINE = new Halia::Engine( );
		HENGINE->Init( Halia::RendererType::DX9, true, "Halia Engine", 1024, 768 );
		HENGINE->SetScriptListener( this );
		HENGINE->SetEventListener( this );
		HENGINE->SetFrameListener( this );
		
		//:: Initialize scripting
		//HSCRIPTER->AddFunction( "test" );
		//HSCRIPTER->LoadFile( "C:\\test.lua" );

		//:: Set up global engine stuff
		HRENDERER->SetLightingEnabled( false );

		//:: Initialize the networking system
		IKNET->Init( );
		IKNET->SetLoginInfo( "74.86.136.196", 29000 );
		IKNET->SetListener( this );

		//:: Start Game
		mState = GameState::INITIALLOAD;
		mGSInitialLoad.Init( );
		while( !mGSInitialLoad.IsReady( ) ) Sleep( 0 );
		mGSInitialLoad.Enter( );

		//:: Start preparing the login screen
		mNextState = GameState::LOGIN;
		mGSLogin.Init( );

		//:: Start Rendering
		printf( "Done loading, rendering begun.\n" );
		HENGINE->StartRendering( );

		//:: Clean up!
		HENGINE->Destroy( );
		Halia::Destroy( );

		return 0;
	};

	bool PreFrameEvent( )
	{
		//:: Process Networking
		IKNET->Poll( );

		//:: TESTCODE - Ready the next state
		if( mState != GameState::LOGIN && mGSLogin.IsReady( ) )
		{
			mGSInitialLoad.Leave( );
			mGSLogin.Enter( );
			mState = GameState::LOGIN;
		}

		//:: Divert to proper state
		if( mState == GameState::DEBUG )
			return mGSDebug.PreFrameEvent( );
		else if( mState == GameState::INITIALLOAD )
			return mGSInitialLoad.PreFrameEvent( );
		else if( mState == GameState::LOGIN )
			return mGSLogin.PreFrameEvent( );
		else
			return false;
	};

	bool FrameEvent( )
	{
		if( mState == GameState::DEBUG )
			return mGSDebug.FrameEvent( );
		else if( mState == GameState::INITIALLOAD )
			return mGSInitialLoad.FrameEvent( );
		else if( mState == GameState::LOGIN )
			return mGSLogin.FrameEvent( );
		else
			return false;
	};

	bool EventHandler( Halia::EventBase* evt )
	{
		if( mState == GameState::DEBUG )
			return mGSDebug.EventHandler( evt );
		else if( mState == GameState::INITIALLOAD )
			return mGSInitialLoad.EventHandler( evt );
		else if( mState == GameState::LOGIN )
			return mGSLogin.EventHandler( evt );

		return true;
	};

	int ScriptEvent( lua_State* L, char* fname )
	{
		if( strcmp( fname, "test" ) == 0 ) {
		} else {
			printf( "Unknown script function called: '%s'\n", fname );
		}

		return 0;
	};

protected:
	GSInitialLoad mGSInitialLoad;
	GSDebug mGSDebug;
	GSLogin	mGSLogin;
	GameState::GameState mState;
	GameState::GameState mNextState;
};