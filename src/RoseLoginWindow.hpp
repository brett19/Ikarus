#pragma once
#include "Overlay.hpp"
#include "OverlayTexture.hpp"
#include "RoseWindow.hpp"

class RoseLoginWindow : public RoseWindow
{
public:
	RoseLoginWindow( )
	{
	};

	RoseLoginWindow( int x, int y, RoseGfxList* gfx )
		: RoseWindow( x, y, 424, 372 )
	{
		mBg = new Halia::OverlayImage( gfx->GetOTexture( "GID_LOGIN_BG" ), 0, 0, 424, 372 );
		mPassword = new Halia::OverlayTextBox( 153-3, 281-6, 100, 20, true );
		mUsername = new Halia::OverlayTextBox( 153-3, 238-6, 100, 20 );
		mOkay = new Halia::OverlayButton( 294, 201, 74, 28, gfx->GetOTexture( "GID_BTN_CONFIRM_NORMAL" ), gfx->GetOTexture( "GID_BTN_CONFIRM_OVER" ), gfx->GetOTexture( "GID_BTN_CONFIRM_DOWN" ) );
		mCancel = new Halia::OverlayButton( 294, 258, 74, 28, gfx->GetOTexture( "GID_BTN_EXIT_NORMAL" ), gfx->GetOTexture( "GID_BTN_EXIT_OVER" ), gfx->GetOTexture( "GID_BTN_EXIT_DOWN" ) );
		
		AddChild( mBg );
		AddChild( mPassword );
		AddChild( mUsername );
		AddChild( mOkay );
		AddChild( mCancel );
	};

	~RoseLoginWindow( )
	{
	};

	int GetType( ) const
	{
		return RoseOverlayType::LOGINWND;
	};

	virtual void Update( )
	{
		return Overlay::Update( );
	};

	virtual void Render( Halia::OverlayManager* mgr, Halia::OverlayRenderer* or )
	{
		return Overlay::Render( mgr, or );
	};
	
	const std::string& GetUsername( )
	{
		return mUsername->GetValue( );
	};

	const std::string& GetPassword( )
	{
		return mPassword->GetValue( );
	};

	void SetUsername( const std::string value )
	{
		mUsername->SetValue( value );
	};

	void SetPassword( const std::string value )
	{
		mPassword->SetValue( value );
	};

	bool DoEvent( Halia::EventBase* evt )
	{
		if( evt->GetType( ) == Halia::EventType::OVERLAY_CLICKED ) {
			Halia::OverlayEvent* revt = (Halia::OverlayEvent*)evt;
			if( revt->sender == mOkay ) {
				Halia::OverlayEvent nevt( RoseEventType::LOGINOKAYCLICK, this );
				Halia::Overlay::ProcEventDown( &nevt );
				return true;
			}
			if( revt->sender == mCancel ) {
				Halia::OverlayEvent nevt( RoseEventType::LOGINCANCELCLICK, this );
				Halia::Overlay::ProcEventDown( &nevt );
				return true;
			}
		}
		return false;
	};

private:
	Halia::OverlayImage* mBg;
	Halia::OverlayTextBox* mUsername;
	Halia::OverlayTextBox* mPassword;
	Halia::OverlayButton* mOkay;
	Halia::OverlayButton* mCancel;
};