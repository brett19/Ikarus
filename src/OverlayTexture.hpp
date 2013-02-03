#pragma once
#include "Texture.hpp"
#include "OverlayRenderer.hpp"

namespace Halia
{
	class OverlayTexture
	{
	public:
		OverlayTexture( TextureBase* tex, int x, int y, int sx, int sy )
			: mTexture( tex ), mOffset( x, y ), mSize( sx, sy )
		{
		};

		~OverlayTexture( )
		{
		};

		void Render( OverlayRenderer* mgr, int x, int y, int sx, int sy )
		{
			mgr->DrawTexture( x, y, sx, sy, mOffset.x, mOffset.y, mSize.x, mSize.y, mTexture );
		};

	protected:
		TextureBase* mTexture;
		Point mOffset;
		Point mSize;
	};
};