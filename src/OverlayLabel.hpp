#pragma once
#include "Overlay.hpp"

namespace Halia
{
	class OverlayLabel : public Overlay
	{
	public:
		OverlayLabel( )
			: mColor( 0xFFFFFFFF )
		{
		};

		OverlayLabel( std::string text )
			: mText( text ), mColor( 0xFFFFFFFF )
		{
		};

		OverlayLabel( int x, int y )
			: Overlay( x, y ), mColor( 0xFFFFFFFF )
		{
		};

		OverlayLabel( int x, int y, int sx, int sy )
			: Overlay( x, y, sx, sy ), mColor( 0xFFFFFFFF )
		{
		};

		OverlayLabel( int x, int y, int sx, int sy, std::string text )
			: Overlay( x, y, sx, sy ), mText( text ), mColor( 0xFFFFFFFF )
		{
		};

		~OverlayLabel( )
		{
		};

		int GetType( ) const
		{
			return OverlayType::LABEL;
		};

		virtual void Update( )
		{
			return Overlay::Update( );
		};

		virtual void Render( OverlayManager* mgr, OverlayRenderer* or )
		{
			or->DrawString( mGPosition.x + 2, mGPosition.y + 2, mText, mColor );
			return Overlay::Render( mgr, or );
		};

		void SetValue( const std::string& str )
		{
			mText = str;
		};

		void SetColor( unsigned int color )
		{
			mColor = color;
		};

		const std::string& GetValue( )
		{
			return mText;
		};

		bool DoEvent( EventBase* evt )
		{
			return false;
		};

	private:
		std::string mText;
		unsigned int mColor;
	};
};