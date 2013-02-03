#pragma once
#include "Overlay.hpp"
#include "OverlayTexture.hpp"

namespace Halia
{
	class OverlayImage : public Overlay
	{
	public:
		OverlayImage( Halia::OverlayTexture* tex )
			: mTex( tex )
		{
		};

		OverlayImage( Halia::OverlayTexture* tex, int x, int y )
			: mTex( tex ), Overlay( x, y )
		{
		};

		OverlayImage( Halia::OverlayTexture* tex, int x, int y, int sx, int sy )
			: mTex( tex ), Overlay( x, y, sx, sy )
		{
		};

		~OverlayImage( )
		{
		};

		int GetType( ) const
		{
			return OverlayType::IMAGE;
		};

		virtual void Update( )
		{
			return Overlay::Update( );
		};

		virtual void Render( OverlayManager* mgr, OverlayRenderer* or )
		{
			mTex->Render( or, mGPosition.x, mGPosition.y, mSize.x, mSize.y );
			return Overlay::Render( mgr, or );
		};
		
	private:
		Halia::OverlayTexture* mTex;
	};
};