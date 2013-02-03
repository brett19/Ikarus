#include "InputManager.hpp"
#include "LogManager.hpp"

namespace Halia
{
	InputManager::InputManager( )
		: mModifiers( 0x0000 )
	{
	};

	InputManager::~InputManager( )
	{
	};

	bool InputManager::Init( HWND hwnd, int width, int height )
	{
		OIS::ParamList pl;

		//:: Get stringed wnd handle
		std::ostringstream wnd;
		wnd << (size_t)hwnd;

		//:: Set up initialization params
		pl.insert( std::make_pair( std::string( "WINDOW" ), wnd.str( ) ) );
		pl.insert( std::make_pair( std::string( "w32_mouse" ), std::string( "DISCL_FOREGROUND" ) ) );
		pl.insert( std::make_pair( std::string( "w32_mouse" ), std::string( "DISCL_NONEXCLUSIVE" ) ) );

		//:: Create input manager
		mInputManager = OIS::InputManager::createInputSystem( pl );
		if( mInputManager == 0 )
		{
			HLOGERR( "Failed to initialize input system" );
			return false;
		}
		mInputManager->enableAddOnFactory( OIS::InputManager::AddOn_All );

		//:: Create keyboard device
		mKeyboard = (OIS::Keyboard*)mInputManager->createInputObject( OIS::OISKeyboard, true );
		if( mKeyboard == 0 )
		{
			HLOGERR( "Failed to initialize keyboard input" );
			return false;
		}
		mKeyboard->setEventCallback( this );

		//:: Create mouse device
		mMouse = (OIS::Mouse*)mInputManager->createInputObject( OIS::OISMouse, true );
		if( mMouse == 0 )
		{
			HLOGERR( "Failed to initialize mouse input" );
			return false;
		}
		mMouse->setEventCallback( this );

		const OIS::MouseState& ms = mMouse->getMouseState( );
		ms.width = width;
		ms.height = height;

		return true;
	};

	bool InputManager::Poll( )
	{
		mMouse->capture( );
		mKeyboard->capture( );
		return true;
	};

	bool InputManager::GetKeyDown( int keycode )
	{
		return mKeyboard->isKeyDown( (OIS::KeyCode)keycode );
	};

	Halia::Point InputManager::GetAbsMousePos( )
	{
		const OIS::MouseState& ms = mMouse->getMouseState( );
		return Halia::Point( ms.X.abs, ms.Y.abs );
	};

	Halia::Point InputManager::GetRelMousePos( )
	{
		const OIS::MouseState& ms = mMouse->getMouseState( );
		return Halia::Point( ms.X.rel, ms.Y.rel );
	};

	bool InputManager::GetMouseDownLeft( )
	{
		const OIS::MouseState& ms = mMouse->getMouseState( );
		return ms.buttonDown( (OIS::MouseButtonID)0 ); // MB_LEFT
	};

	bool InputManager::GetMouseDownRight( )
	{
		const OIS::MouseState& ms = mMouse->getMouseState( );
		return ms.buttonDown( (OIS::MouseButtonID)1 ); // MB_RIGHT
	};

	void InputManager::SetListener( EventListener* ls )
	{
		mListener = ls;
	};

	void InputManager::handleNonBufferedKeys( )
	{
	}

	void InputManager::handleNonBufferedMouse( )
	{
	}

	bool InputManager::keyPressed( const OIS::KeyEvent& arg )
	{
		if( arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT ) {
			mModifiers |= 0x0001;
			return true;
		}
		if( arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL ) {
			mModifiers |= 0x0002;
			return true;
		}

		/*
		KeyEvent evt(
			EventType::INPUT_KEYDOWN,
			arg.key,
			mModifiers
		);
		mListener->EventHandler( &evt );
		*/

		return true;
	}

	bool InputManager::keyReleased( const OIS::KeyEvent& arg )
	{ 
		if( arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT ) {
			mModifiers &= ~0x0001;
			return true;
		}
		if( arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL ) {
			mModifiers &= ~0x0002;
			return true;
		}

		/*
		KeyEvent evt(
			EventType::INPUT_KEYUP,
			arg.key,
			mModifiers
		);
		mListener->EventHandler( &evt );
		*/

		return true;
	}

	bool InputManager::mouseMoved( const OIS::MouseEvent& arg )
	{
		/*
		MouseEvent evt( EventType::INPUT_MOUSEMOVE, arg.state.X.abs, arg.state.Y.abs, arg.state.buttons );
		mListener->EventHandler( &evt );
		*/

		return true;
	}

	bool InputManager::mousePressed( const OIS::MouseEvent& arg, OIS::MouseButtonID id )
	{
		/*
		MouseEvent evt( EventType::INPUT_MOUSEDOWN, arg.state.X.abs, arg.state.Y.abs, arg.state.buttons );
		mListener->EventHandler( &evt );
		*/

		return true;
	}

	bool InputManager::mouseReleased( const OIS::MouseEvent& arg, OIS::MouseButtonID id )
	{
		/*
		MouseEvent evt( EventType::INPUT_MOUSEUP, arg.state.X.abs, arg.state.Y.abs, arg.state.buttons );
		mListener->EventHandler( &evt );
		*/

		return true;
	}
};