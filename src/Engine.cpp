#include "Engine.hpp"

namespace Halia
{
	Engine::Engine( )
		: mRenderer( 0 ), mWindow( 0 ), mScripter( 0 ), mOverlayer( 0 )
	{
		strcpy( mSFps[0], "FPS: [CALCULATING]" );
		strcpy( mSFps[1], "Average Prim Count: [CALCULATING]" );
		strcpy( mSFps[2], "Average Batch Count: [CALCULATING]" );
	};

	Engine::~Engine( )
	{
	};

	bool Engine::Init( RendererType::RendererType renderer, bool wndauto, std::string wndtitle, int width, int height )
	{
		HLOGMSG( "-- Engine starting! [RENDERER:%d,WIDTH:%d,HEIGHT:%d]", renderer, width, height );

		//:: Initialize scripting module
		mScripter = new ScriptManager( );
		mScripter->Init( );
		HLOGMSG( "Script Manager created" );

		//:: Initialize render window module
		mWindow = new RenderWindow( );
		mWindow->Init( wndtitle, width, height );
		HLOGMSG( "Render Window created" );

		//:: Initialize renderer
		mRType = renderer;
		if( renderer == RendererType::DX9 ) {
			mRenderer = new DX9RenderSystem( );
		} else if( renderer == RendererType::OpenGL ) {
			mRenderer = new OGLRenderSystem( );
		} else {
			return false;
		}
		mRenderer->Init( mWindow->GetHwnd( ), width, height, false, -1 );
		HLOGMSG( "Renderer initialized" );

		//:: Initialize overlay module
		mORenderer = new OverlayRenderer( );
		mORenderer->Init( width, height );
		mOverlayer = new OverlayManager( );
		mOverlayer->SetSize( width, height );
		mOverlayer->SetListener( this );
		HLOGMSG( "Overlay manager initialized" );

		//:: Initialize input module
		mInputer = new InputManager( );
		mInputer->Init( mWindow->GetHwnd( ), width, height );
		mInputer->SetListener( this );
		HLOGMSG( "Input manager initialized" );

		//:: Initialize resource management module
		mRManager = new ResourceManager( );
		HLOGMSG( "Resource manager initialized" );

		HLOGMSG( "-- Completed!" );

		// * Good loadin boyz
		return true;
	};

	void Engine::Destroy( )
	{
		delete mRManager;
		delete mInputer;
		delete mOverlayer;
		delete mORenderer;
		delete mRenderer;
		delete mWindow;
		delete mScripter;
	};

	void Engine::StartRendering( )
	{
		//:: Initialize frame rate counters
		mFrameCount = 0;
		mBatchCount = 0;
		mPrimCount = 0;
		mStartTime = GetTickCount( );

		//:: Start main render-loop
		while( 1 )
		{
			//:: Process window events
			if( !mWindow->Poll( ) )
				break;
			mInputer->Poll( );

			//:: Call preframe listener
			mFrameListener->PreFrameEvent( );

			//:: Clear the scene
			mRenderer->Clear( Color( 0xFFFFFFFF ) );
			mRenderer->BeginScene( );

			//:: Call frame listener
			mFrameListener->FrameEvent( );

			//:: Render the overlays
			///* Comment this to disable overlays
			mOverlayer->Update( );
			mOverlayer->Render( mORenderer );
			mORenderer->DrawString( 10, 10, mSFps[0] );
			mORenderer->DrawString( 10, 28, mSFps[1] );
			mORenderer->DrawString( 10, 46, mSFps[2] );
			mORenderer->Render( mRenderer );
			//*/

			//:: Finish rendering
			mRenderer->EndScene( );
			mRenderer->Present( );

			//:: Frame rate stuff
			mFrameCount++;
			mBatchCount += mRenderer->GetBatchCount( );
			mPrimCount += mRenderer->GetPrimCount( );
			if( GetTickCount( ) - mStartTime >= 3000 ) {
				float fps = (float)mFrameCount / ( GetTickCount( ) - mStartTime ) * 1000.0f;
				float abc = (float)mBatchCount / mFrameCount;
				float apc = (float)mPrimCount / mFrameCount;
				sprintf( mSFps[0], "FPS: %4.2f", fps );
				sprintf( mSFps[1], "Average Prim Count: %4.0f", apc );
				sprintf( mSFps[2], "Average Batch Count: %4.0f", abc );
				printf( "FPS: %4.2f\n", fps );
				mFrameCount = 0;
				mPrimCount = 0;
				mBatchCount = 0;
				mStartTime = GetTickCount( );
			}
		};
	};

	void Engine::Quit( )
	{
		PostQuitMessage( 0 );
	};

	bool Engine::EventHandler( EventBase* evt )
	{
		if( evt->GetType( ) > EventType::INPUTSTART && evt->GetType( ) < EventType::INPUTEND ) {
			mOverlayer->EventHandler( evt );
		}
		mEventListener->EventHandler( evt );
		
		return true;
	};

	void Engine::SetFrameListener( FrameListener* fm )
	{
		mFrameListener = fm;
	};

	void Engine::SetEventListener( EventListener* em )
	{
		mEventListener = em;
	};

	void Engine::SetScriptListener( ScriptListener* sm )
	{
		mScripter->SetListener( sm );
	};

	RendererType::RendererType& Engine::GetRendererType( )
	{
		return mRType;
	};

	RenderWindow*& Engine::GetWindow( )
	{
		return mWindow;
	};

	RenderSystem*& Engine::GetRenderer( )
	{
		return mRenderer;
	};

	OverlayManager*& Engine::GetOverlayer( )
	{
		return mOverlayer;
	};

	ScriptManager*& Engine::GetScripter( )
	{
		return mScripter;
	};

	InputManager*& Engine::GetInputer( )
	{
		return mInputer;
	};

	ResourceManager*& Engine::GetResourceManager( )
	{
		return mRManager;
	};

	Engine* EInstance = 0;
};