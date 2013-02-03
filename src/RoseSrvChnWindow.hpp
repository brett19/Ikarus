#pragma once
#include "Overlay.hpp"
#include "OverlayTexture.hpp"
#include "OverlayListBox.hpp"
#include "OverlayLabel.hpp"
#include "OverlayLBLabel.hpp"
#include "RoseWindow.hpp"

class RoseSrvChnWindow : public RoseWindow
{
public:
	RoseSrvChnWindow( )
	{
	};

	RoseSrvChnWindow( int x, int y, RoseGfxList* gfx )
		: RoseWindow( x, y, 514, 508 )
	{
		mBg = new Halia::OverlayImage( gfx->GetOTexture( "GID_SELECT_SERVER_BG" ), 0, 0, 514, 508 );
		mOkay = new Halia::OverlayButton( 463, 216, 74, 28, gfx->GetOTexture( "GID_BTN_CONFIRM_NORMAL" ), gfx->GetOTexture( "GID_BTN_CONFIRM_OVER" ), gfx->GetOTexture( "GID_BTN_CONFIRM_DOWN" ) );
		mCancel = new Halia::OverlayButton( 463, 273, 74, 28, gfx->GetOTexture( "GID_BTN_EXIT_NORMAL" ), gfx->GetOTexture( "GID_BTN_EXIT_OVER" ), gfx->GetOTexture( "GID_BTN_EXIT_DOWN" ) );
		mServerList = new Halia::OverlayListBox( 100, 235, 135, 22 );

		mServerList->AddItem( new Halia::OverlayLBLabel( "This is a test" ) );
		mServerList->AddItem( new Halia::OverlayLBLabel( "This is a test2" ) );
		mServerList->AddItem( new Halia::OverlayLBLabel( "This is a test3" ) );

		AddChild( mBg );
		AddChild( mOkay );
		AddChild( mCancel );
		AddChild( mServerList );
	};

	~RoseSrvChnWindow( )
	{
	};

	int GetType( ) const
	{
		return RoseOverlayType::SRVCHNWND;
	};

	virtual void Update( )
	{
		return Overlay::Update( );
	};

	virtual void Render( Halia::OverlayManager* mgr, Halia::OverlayRenderer* or )
	{
		return Overlay::Render( mgr, or );
	};
	
	bool DoEvent( Halia::EventBase* evt )
	{
		if( evt->GetType( ) == Halia::EventType::OVERLAY_CLICKED ) {
			Halia::OverlayEvent* revt = (Halia::OverlayEvent*)evt;
			if( revt->sender == mOkay ) {
				Halia::OverlayEvent nevt( RoseEventType::SVRCHNOKAYCLICK, this );
				Halia::Overlay::ProcEventDown( &nevt );
				return true;
			}
			if( revt->sender == mCancel ) {
				Halia::OverlayEvent nevt( RoseEventType::SVRCHNCANCELCLICK, this );
				Halia::Overlay::ProcEventDown( &nevt );
				return true;
			}
		}
		return false;
	};

private:
	Halia::OverlayImage* mBg;
	Halia::OverlayListBox* mServerList;
	Halia::OverlayButton* mOkay;
	Halia::OverlayButton* mCancel;
};