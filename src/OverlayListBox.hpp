#pragma once
#include "Overlay.hpp"
#include "OverlayTexture.hpp"

namespace Halia
{
	class OverlayListBox : public Overlay
	{
	public:
		OverlayListBox( )
		{
		};

		OverlayListBox( int x, int y )
			: Overlay( x, y )
		{
		};

		OverlayListBox( int x, int y, int sx, int sy )
			: Overlay( x, y, sx, sy )
		{
		};

		~OverlayListBox( )
		{
		};

		int GetType( ) const
		{
			return OverlayType::LISTBOX;
		};

		virtual void Update( )
		{
			int pos = 0;
			for( std::vector<Overlay*>::iterator i = mItems.begin( ); i != mItems.end( ); ++i )
			{
				(*i)->SetPosition( 0, pos );
				pos += (*i)->GetSize( ).y;
			}
			return Overlay::Update( );
		};

		virtual void Render( OverlayManager* mgr, OverlayRenderer* or )
		{
			return Overlay::Render( mgr, or );
		};

		void SetSelectedIndex( int sid )
		{
			if( mSelectedItem >= 0 && mSelectedItem < (int)mItems.size( ) ) {
				OverlayEvent* nevt = new OverlayEvent( EventType::OVERLAY_UNSELECTED, 0 );
				mItems[mSelectedItem]->DoEvent( nevt );
			}

			mSelectedItem = sid;
			OverlayEvent* nevt = new OverlayEvent( EventType::OVERLAY_SELECTED, 0 );
			mItems[mSelectedItem]->DoEvent( nevt );

			OverlayEvent evt( EventType::OVERLAY_SELIDXCHANGED, this );
			RaiseEvent( &evt );
		};

		bool DoEvent( EventBase* evt )
		{
			if( evt->GetType( ) == EventType::OVERLAY_SETSELECTED )
			{
				OverlayEvent* revt = (OverlayEvent*)evt;

				int sid = 0;
				for( std::vector<Overlay*>::iterator i = mItems.begin( ); i != mItems.end( ); ++i )
				{
					if( *i == revt->sender )
						break;
					sid++;
				}
				if( sid > (int)mItems.size( ) )
					return false;

				SetSelectedIndex( sid );
			}
			return false;
		};

		void AddItem( Overlay* o )
		{
			mItems.push_back( o );
			AddChild( o );
		};

	private:
		int mSelectedItem;
		std::vector< Overlay* > mItems;
	};
};