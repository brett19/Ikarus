#pragma once
#include <windows.h>
#include <OISInputManager.h>
#include <OISException.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISJoyStick.h>
#include <OISEvents.h>
#include <sstream>
#include "InputConstants.hpp"
#include "Events.hpp"
#include "EventListener.hpp"
#include "Point.hpp"

namespace Halia
{
	class InputManager : public OIS::KeyListener, public OIS::MouseListener
	{
	public:
		InputManager( );
		~InputManager( );

		bool Init( HWND hwnd, int width, int height );
		bool Poll( );
		bool GetKeyDown( int keycode );
		Halia::Point GetAbsMousePos( );
		Halia::Point GetRelMousePos( );
		bool GetMouseDownLeft( );
		bool GetMouseDownRight( );
		void SetListener( EventListener* ls );

		void handleNonBufferedKeys( );
		void handleNonBufferedMouse( );
		bool keyPressed( const OIS::KeyEvent& arg );
		bool keyReleased( const OIS::KeyEvent& arg );
		bool mouseMoved( const OIS::MouseEvent& arg );
		bool mousePressed( const OIS::MouseEvent& arg, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent& arg, OIS::MouseButtonID id );

	private:
		EventListener* mListener;
		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse* mMouse;
		unsigned int mModifiers;
	};
};