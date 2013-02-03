#pragma once
#include "Overlay.hpp"
#include "OverlayTexture.hpp"
#include "OverlayLabel.hpp"
#include "RoseWindow.hpp"

namespace RoseDialogType
{
	enum RoseDialogType
	{
		OKAY = 1,
		CANCEL = 2,
	};
};

class RoseDialog : public RoseWindow
{
public:
	RoseDialog( )
	{
	};

	RoseDialog( int x, int y, RoseGfxList* gfx )
		: RoseWindow( x, y, 189, 107 )
	{
		mBgTop = new Halia::OverlayImage( gfx->GetOTexture( "GID_MSGBOX_TOP" ), 0, 0, 189, 26 );
		mBgMiddle = new Halia::OverlayImage( gfx->GetOTexture( "GID_MSGBOX_MIDDLE" ), 0, 26, 189, 22 );
		mBgBottom = new Halia::OverlayImage( gfx->GetOTexture( "GID_MSGBOX_BOTTOM" ), 0, 48, 189, 59 );
		mOkButton = new Halia::OverlayButton( 61, 71, 66, 26, gfx->GetOTexture( "GID_MSGBOX_BTN_CONFIRM_NORMAL" ), gfx->GetOTexture( "GID_MSGBOX_BTN_CONFIRM_OVER" ), gfx->GetOTexture( "GID_MSGBOX_BTN_CONFIRM_DOWN" ) );
		mCancelButton = new Halia::OverlayButton( 61, 71, 66, 26, gfx->GetOTexture( "GID_MSGBOX_BTN_CANCEL_NORMAL" ), gfx->GetOTexture( "GID_MSGBOX_BTN_CANCEL_OVER" ), gfx->GetOTexture( "GID_MSGBOX_BTN_CANCEL_DOWN" ) );
		mMessageLabel = new Halia::OverlayLabel( 5, 26, 179, 30 );
		
		mOkButton->SetVisible( false );
		mCancelButton->SetVisible( false );

		AddChild( mBgTop );
		AddChild( mBgMiddle );
		AddChild( mBgBottom );
		AddChild( mOkButton );
		AddChild( mCancelButton );
		AddChild( mMessageLabel );
	};

	~RoseDialog( )
	{
	};

	int GetType( ) const
	{
		return RoseOverlayType::DIALOGWND;
	};

	virtual void Update( )
	{
		return Overlay::Update( );
	};

	virtual void Render( Halia::OverlayManager* mgr, Halia::OverlayRenderer* or )
	{
		or->DrawString( 4, 20, "TEST" );
		return Overlay::Render( mgr, or );
	};
	
	void SetType( int type )
	{
		if( type & RoseDialogType::OKAY && type & RoseDialogType::CANCEL ) {
			mOkButton->SetPosition( 112, 71 );
			mOkButton->SetVisible( true );
			mCancelButton->SetPosition( 12, 71 );
			mOkButton->SetVisible( true );
		} else if( type & RoseDialogType::OKAY ) {
			mOkButton->SetPosition( 62, 71 );
			mOkButton->SetVisible( true );
			mCancelButton->SetVisible( false );
		} else if( type & RoseDialogType::CANCEL ) {
			mCancelButton->SetPosition( 62, 71 );
			mCancelButton->SetVisible( true );
			mOkButton->SetVisible( false );
		}
	};

	void SetMessage( const std::string msg )
	{
		mMessageLabel->SetValue( msg );
	};

	void ProcEventDown( Halia::EventBase* evt )
	{
		if( evt->GetType( ) == Halia::EventType::OVERLAY_CLICKED ) {
			Halia::OverlayEvent* revt = (Halia::OverlayEvent*)evt;
			if( revt->sender == mOkButton ) {
				Halia::OverlayEvent nevt( RoseEventType::DIALOGOKAYCLICK, this );
				Halia::Overlay::ProcEventDown( &nevt );
				return;
			}
			if( revt->sender == mCancelButton ) {
				Halia::OverlayEvent nevt( RoseEventType::DIALOGCANCELCLICK, this );
				Halia::Overlay::ProcEventDown( &nevt );
				return;
			}
		}
		Halia::Overlay::ProcEventDown( evt );
	};

private:
	Halia::OverlayImage* mBgTop;
	Halia::OverlayImage* mBgMiddle;
	Halia::OverlayImage* mBgBottom;
	Halia::OverlayButton* mOkButton;
	Halia::OverlayButton* mCancelButton;
	Halia::OverlayLabel* mMessageLabel;
};