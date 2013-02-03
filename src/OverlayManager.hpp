#pragma once
#include "OverlayRenderer.hpp"
#include "Overlay.hpp"
#include "EventListener.hpp"

namespace Halia
{
	class OverlayManager : public Overlay, public EventListener
	{
	public:
		OverlayManager( );
		~OverlayManager( );
		int GetType( ) const;
		void SetListener( EventListener* ol );

		virtual void Render( OverlayRenderer* mgr );

		virtual bool EventHandler( Event* evt );

	private:
		Overlay* mFocusedControl;
		EventListener* mListener;
	};
};