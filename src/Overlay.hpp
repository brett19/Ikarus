#pragma once
#include <vector>
#include "Point.hpp"
#include "OverlayRenderer.hpp"
#include "InputManager.hpp"

namespace Halia
{
	namespace OverlayType
	{
		enum OverlayType
		{
			MANAGER,
			BASE,
			LABEL,
			IMAGE,
			BUTTON,
			TEXTBOX,
			LISTBOX,
			CUSTOM
		};
	};

	class OverlayManager;
	
	class Overlay
	{
		friend OverlayManager;
	public:
		Overlay( );
		Overlay( int x, int y );
		Overlay( int x, int y, int sx, int sy );
		~Overlay( );

		void AddChild( Overlay* o );
		const Point& GetPosition( );
		void SetPosition( int x, int y );
		void SetSize( int x, int y );
		void SetVisible( bool visible = true );
		const Point& GetSize( ) const;
		bool TestPoint( int x, int y );
		void CenterChild( Overlay* child ) const;

		virtual int GetType( ) const;
		virtual bool DoEvent( Event* evt );
		virtual bool ProcEvent( Event* evt );
		virtual void RaiseEvent( Event* evt );
		virtual void ProcEventDown( Event* evt );
		virtual void Update( );
		virtual void Render( OverlayManager* mgr, OverlayRenderer* or );

	protected:
		Overlay* mParent;
		std::vector< Overlay* > mChildren;
		Point mPosition;
		Point mSize;
		Point mGPosition;
		bool mVisible;
		bool mFocused;
	};
};