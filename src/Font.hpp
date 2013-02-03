#pragma once
#include <windows.h>

namespace Halia
{
	class Font
	{
	public:
		struct Character
		{
			int x;
			int y;
			int width;
		};

		Font( )
			: mTex( 0 )
		{
		};

		~Font( )
		{
		};

		bool Init( std::string fontname, int fontsize, int fontweight = 400, int charsetstart = 32, int charsetend = 126 )
		{
			mTexWidth = 256;
			mTexHeight = 256;
			mCharSizeX = 24;
			mCharSizeY = 24;
			mCharsPerRow = mTexWidth / mCharSizeX;

			//:: Initialize stuff
			HDC fntdc = CreateCompatibleDC( GetDC( 0 ) );
			HBITMAP fntbmp = CreateCompatibleBitmap( GetDC( 0 ), mTexWidth, mTexHeight );
			SelectObject( fntdc, fntbmp );
			BitBlt( fntdc, 0, 0, mTexWidth, mTexHeight, 0, 0, 0, WHITENESS );
			HFONT fntobj = CreateFont( fontsize, 0, 0, 0, fontweight,
				FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				ANTIALIASED_QUALITY, DEFAULT_PITCH, fontname.c_str( ) );
			SelectObject( fntdc, fntobj );

			//:: Generate actual font data
			for( int i = 0, j = 0; i < 256; i++ )
			{
				if( i >= charsetstart && i <= charsetend ) {
					mChars[i].x = ( j % mCharsPerRow ) * mCharSizeX;
					mChars[i].y = ( j / mCharsPerRow ) * mCharSizeY;
					GetCharWidth32( fntdc, i, i, &mChars[i].width );

					RECT rect;
					rect.left = mChars[i].x; rect.right = rect.left + mCharSizeX;
					rect.top = mChars[i].y; rect.bottom = rect.top + mCharSizeY;
					DrawText( fntdc, (char*)&i, 1, &rect, DT_LEFT | DT_NOPREFIX );

					j++;
				} else {
					mChars[i].x = 0;
					mChars[i].y = 0;
					mChars[i].width = 0;
				}
			}

			//:: Create real texture!
			mTex = new Halia::TextureMemory( );
			mTex->Init( mTexWidth, mTexHeight, Halia::TextureFormat::A8 );
			char* texbuf = mTex->GetBuffer( );
			for( int iy = 0; iy < mTexWidth; iy++ )
			{
				for( int ix = 0; ix < mTexHeight; ix++ )
				{
					*texbuf++ = (char)~GetPixel( fntdc, ix, iy );
				}
			}

			//:: Clean shit up!
			DeleteObject( fntobj );
			DeleteObject( fntbmp );
			DeleteObject( fntdc );

			return true;
		};

		const int& GetCharWidth( char chr ) const
		{
			return mChars[chr].width;
		};

		const int& GetCharSizeX( ) const
		{
			return mCharSizeX;
		};

		const int& GetCharSizeY( ) const
		{
			return mCharSizeY;
		};

		const int& GetCharTexX( char chr ) const
		{
			return mChars[chr].x;
		};

		const int& GetCharTexY( char chr ) const
		{
			return mChars[chr].y;
		};

		Halia::TextureMemory* GetTexture( ) const
		{
			return mTex;
		};

	protected:
		Halia::TextureMemory* mTex;
		int mTexWidth;
		int mTexHeight;
		int mCharsPerRow;
		int mCharSizeX;
		int mCharSizeY;
		Character mChars[256];
	};
};