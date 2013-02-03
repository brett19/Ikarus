#pragma once
#include "Overlay.hpp"
#include "OverlayTexture.hpp"

namespace Halia
{
	namespace ButtonState
	{
		enum ButtonState
		{
			NORMAL,
			OVER,
			DOWN
		};
	};

	class OverlayButton : public Overlay
	{
	public:
		OverlayButton( )
		{
		};

		OverlayButton( int x, int y, OverlayTexture* ot1, OverlayTexture* ot2, OverlayTexture* ot3 )
			: Overlay( x, y ), mStateTex1( ot1 ), mStateTex2( ot2 ), mStateTex3( ot3 ), mState( ButtonState::NORMAL )
		{
		};

		OverlayButton( int x, int y, int sx, int sy, OverlayTexture* ot1, OverlayTexture* ot2, OverlayTexture* ot3 )
			: Overlay( x, y, sx, sy ), mStateTex1( ot1 ), mStateTex2( ot2 ), mStateTex3( ot3 ), mState( ButtonState::NORMAL )
		{
		};

		~OverlayButton( )
		{
		};

		int GetType( ) const
		{
			return OverlayType::BUTTON;
		};

		virtual void Update( )
		{
			return Overlay::Update( );
		};

		virtual void Render( OverlayManager* mgr, OverlayRenderer* or )
		{
			if( mState == ButtonState::NORMAL ) {
				mStateTex1->Render( or, mGPosition.x, mGPosition.y, mSize.x, mSize.y );
			} else if( mState == ButtonState::OVER ) {
				mStateTex2->Render( or, mGPosition.x, mGPosition.y, mSize.x, mSize.y );
			} else if( mState == ButtonState::DOWN ) {
				mStateTex3->Render( or, mGPosition.x, mGPosition.y, mSize.x, mSize.y );
			}
			return Overlay::Render( mgr, or );
		};

		bool DoEvent( Event* evt )
		{
			switch( evt->GetType( ) )
			{
				case EventType::INPUT_MOUSEMOVE:
				{
					MouseEvent* revt = (MouseEvent*)evt;
					if( TestPoint( revt->x, revt->y ) ) {
						if( revt->buttons & 1 )
							mState = ButtonState::DOWN;
						else
							mState = ButtonState::OVER;
					} else {
						mState = ButtonState::NORMAL;
					}
					break;
				}
				case EventType::INPUT_MOUSEDOWN:
				{
					MouseEvent* revt = (MouseEvent*)evt;
					if( TestPoint( revt->x, revt->y ) )
					{
						mState = ButtonState::DOWN;
					};
					break;
				}
				case EventType::INPUT_MOUSEUP:
				{
					MouseEvent* revt = (MouseEvent*)evt;
					if( mState == ButtonState::DOWN )
						mState = ButtonState::NORMAL;
					if( TestPoint( revt->x, revt->y ) )
					{
						OverlayEvent evt( EventType::OVERLAY_CLICKED, this );
						RaiseEvent( &evt );
						mState = ButtonState::OVER;
					};
					break;
				}
			};

			return false;
		};

	private:
		OverlayTexture* mStateTex1;
		OverlayTexture* mStateTex2;
		OverlayTexture* mStateTex3;
		ButtonState::ButtonState mState;
	};
};