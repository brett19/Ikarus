#pragma once
#include "Overlay.hpp"

namespace Halia
{
	class OverlayLBLabel : public OverlayLabel
	{
	public:
		OverlayLBLabel( )
			: OverlayLabel( 0, 0, 150, 15 )
		{
		};

		OverlayLBLabel( std::string text )
			: OverlayLabel( 0, 0, 150, 15, text )
		{
		};

		~OverlayLBLabel( )
		{
		};

		int GetType( ) const
		{
			return OverlayType::LABEL;
		};

		virtual void Update( )
		{
			return OverlayLabel::Update( );
		};

		virtual void Render( OverlayManager* mgr, OverlayRenderer* or )
		{
			return OverlayLabel::Render( mgr, or );
		};

		bool DoEvent( EventBase* evt )
		{
			switch( evt->GetType( ) )
			{
				case EventType::INPUT_MOUSEUP:
				{
					MouseEvent* revt = (MouseEvent*)evt;
					if( TestPoint( revt->x, revt->y ) )
					{
						OverlayEvent evt( EventType::OVERLAY_SETSELECTED, this );
						RaiseEvent( &evt );
					};
					break;
				}
				case EventType::OVERLAY_SELECTED:
				{
					SetColor( 0xFF00FF00 );
					break;
				};
				case EventType::OVERLAY_UNSELECTED:
				{
					SetColor( 0xFFFFFFFF );
					break;
				};
			};
			return false;
		};

	private:

	};
};