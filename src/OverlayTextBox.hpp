#pragma once
#include "Overlay.hpp"
#include "OverlayTexture.hpp"

namespace Halia
{
	class OverlayTextBox : public Overlay
	{
	public:
		OverlayTextBox( )
			: mPassword( false ), mCaretPos( -1 )
		{
		};

		OverlayTextBox( int x, int y, bool ispassword = false )
			: Overlay( x, y ), mPassword( ispassword ), mCaretPos( -1 )
		{
		};

		OverlayTextBox( int x, int y, int sx, int sy, bool ispassword = false )
			: Overlay( x, y, sx, sy ), mPassword( ispassword ), mCaretPos( -1 )
		{
		};

		~OverlayTextBox( )
		{
		};

		int GetType( ) const
		{
			return OverlayType::TEXTBOX;
		};

		virtual void Update( )
		{
			return Overlay::Update( );
		};

		virtual void Render( OverlayManager* mgr, OverlayRenderer* or )
		{
			if( !mPassword ) {
				or->DrawString( mGPosition.x + 2, mGPosition.y + 2, mText, 0xFFFFFFFF, 0, mCaretPos );
			} else {
				std::string newtext( mText.length( ), '*' );
				or->DrawString( mGPosition.x + 2, mGPosition.y + 2, newtext, 0xFFFFFFFF, 0, mCaretPos );
			}
			return Overlay::Render( mgr, or );
		};

		void SetValue( const std::string& str )
		{
			mText = str;
		};

		const std::string& GetValue( )
		{
			return mText;
		};

		bool DoEvent( EventBase* evt )
		{
			switch( evt->GetType( ) )
			{
				case EventType::OVERLAY_FOCUSED:
				{
					printf( "Focus event: %08x\n", this );
					mCaretPos = mText.length( );
					break;
				};

				case EventType::OVERLAY_UNFOCUSED:
				{
					printf( "Unfocus event: %08x\n", this );
					mCaretPos = -1;
					break;
				};
				case EventType::INPUT_MOUSEUP:
				{
					MouseEvent* revt = (MouseEvent*)evt;
					if( TestPoint( revt->x, revt->y ) ) {
						OverlayEvent evt( EventType::OVERLAY_SETFOCUS, this );
						RaiseEvent( &evt );
						return true;
					}
					break;
				}
				case EventType::INPUT_KEYDOWN:
				{
					KeyEvent* revt = (KeyEvent*)evt;

					if( revt->key == KeyCode::BACK )
					{
						if( ~revt->modifiers & 1 )
							mText = mText.substr( 0, mText.length( ) - 1 );
						else
							mText.clear( );
						return true;
					}

					char c = 0;
					if( ~revt->modifiers & 1 )
						c = KeyCode::TOCHARDN[ revt->key ];
					else
						c = KeyCode::TOCHARUP[ revt->key ];

					if( c != 0 ) {
						mText.push_back( c );
						OverlayEvent evt( EventType::OVERLAY_TEXTCHANGED, this );
						RaiseEvent( &evt );
					}

					return true;
					break;
				}
			};

			return false;
		};

	private:
		ButtonState::ButtonState mState;
		std::string mText;
		int mCaretPos;
		bool mPassword;
	};
};