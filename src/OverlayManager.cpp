#include "OverlayManager.hpp"

namespace Halia
{
	/*
		MAIN FUNCTIONS
	*/
	OverlayManager::OverlayManager( )
		: mFocusedControl( 0 )
	{
	};

	OverlayManager::~OverlayManager( )
	{
	};

	int OverlayManager::GetType( ) const
	{
		return OverlayType::MANAGER;
	};

	void OverlayManager::SetListener( EventListener* ol )
	{
		mListener = ol;
	};

	void OverlayManager::Render( OverlayRenderer* or )
	{
		Overlay::Render( this, or );
	};

	bool OverlayManager::EventHandler( EventBase* evt )
	{
		if( evt->GetType( ) > EventType::IKEYSTART && evt->GetType( ) < EventType::IKEYEND )
		{
			if( mFocusedControl )
				mFocusedControl->DoEvent( evt );
		}
		if( evt->GetType( ) > EventType::IMOUSESTART && evt->GetType( ) < EventType::IMOUSEEND )
		{
			ProcEvent( evt );
		}

		return true;
	};

	void OverlayManager::ProcEventDown( EventBase* evt )
	{
		if( evt->GetType( ) == EventType::OVERLAY_SETFOCUS )
		{
			OverlayEvent* revt = (OverlayEvent*)evt;

			if( mFocusedControl ) {
				OverlayEvent* nevt = new OverlayEvent( EventType::OVERLAY_UNFOCUSED, 0 );
				mFocusedControl->DoEvent( nevt );
			}

			mFocusedControl = revt->sender;
			OverlayEvent* nevt = new OverlayEvent( EventType::OVERLAY_FOCUSED, 0 );
			mFocusedControl->DoEvent( nevt );
		}

		mListener->EventHandler( evt );
	};
};