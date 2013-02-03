#pragma once
#include "RenderSystem.hpp"
#include "Font.hpp"
#include "ArrList.hpp"

namespace Halia
{
	struct OverlayVert
	{
		Vector3 pos;
		Vector2 uv;
		unsigned int color;
	};

	struct OverlayQuad
	{
		Halia::TextureBase* tex;
		int mattype;
		OverlayVert verts[4];
	};

	class OverlayRenderer
	{
	public:
		OverlayRenderer( )
		{
		};

		~OverlayRenderer( )
		{
		};

		bool Init( int width, int height )
		{
			mSize = Point( width, height );

			mDecl = new VertexDeclaration( );
			mDecl->AddElement( VertexElement( 0, VDeclSemantic::POSITION, VDeclType::FLOAT3, 0 ) );
			mDecl->AddElement( VertexElement( 0, VDeclSemantic::TEXCOORD, VDeclType::FLOAT2, 0 ) );
			mDecl->AddElement( VertexElement( 0, VDeclSemantic::COLOUR, VDeclType::COLOUR, 0 ) );

			mMat[0] = new Material( );
			mMat[0]->mAlphaBlend = true;
			mMat[0]->mAlphaTest = true;
			mMat[0]->mDepthWrite = true;
			mMat[0]->mDepthBuffer = false;
			mMat[0]->AddTextureStage(
				TextureStage(
					0,
					BlendMode( BlendOp::MODULATE, BlendArg::TEXTURE, BlendArg::DIFFUSE ),
					BlendMode( BlendOp::SELECTARGUMENT1, BlendArg::TEXTURE )
				)
			);

			mMat[1] = new Material( );
			mMat[1]->mAlphaBlend = true;
			mMat[1]->mAlphaTest = false;
			mMat[1]->mDepthWrite = true;
			mMat[1]->mDepthBuffer = false;
			mMat[1]->AddTextureStage(
				TextureStage(
					0,
					BlendMode( BlendOp::SELECTARGUMENT1, BlendArg::DIFFUSE ),
					BlendMode( BlendOp::SELECTARGUMENT1, BlendArg::TEXTURE )
				)
			);

			mDefaultFont = new Halia::Font( );
			mDefaultFont->Init( "Tahoma", 16 );

			return true;
		};

		bool DrawString( int x, int y, std::string str, unsigned int color = 0xFFFFFFFF, Font* fnt = 0, int caretpos = -1 )
		{
			if( fnt == 0 )
				fnt = mDefaultFont;

			int charpos = 0;
			int caretx = -1;
			for( int i = 0; i < (int)str.length( ); i++ )
			{
				char thischar = str[i];

				DrawTexture( x + charpos, y, fnt->GetCharSizeX( ), fnt->GetCharSizeY( ), fnt->GetCharTexX( thischar ), 
					fnt->GetCharTexY( thischar ), fnt->GetCharSizeX( ), fnt->GetCharSizeY( ), fnt->GetTexture( ), 
					color, true );

				if( i == caretx )
					caretx = charpos;

				charpos += fnt->GetCharWidth( thischar );
			}

			if( caretpos >= 0 && GetTickCount( ) % 1500 > 750 )
			{
				if( caretx < 0 )
					caretx = charpos;
				caretx -= ( fnt->GetCharWidth( '|' ) / 2 ) - 1;
				DrawTexture( x + caretx, y, fnt->GetCharSizeX( ), fnt->GetCharSizeY( ), fnt->GetCharTexX( '|' ),
					fnt->GetCharTexY( '|' ), fnt->GetCharSizeX( ), fnt->GetCharSizeY( ), fnt->GetTexture( ),
					color, true );
			}

			return true;
		};

		/*
			x,y - Where to draw on the screen
			tx,ty - Where to start in the TextureBase
			tsx,tsy - The size of the area to copy
		*/
		bool DrawTexture( int x, int y, int sx, int sy, int tx, int ty, int tsx, int tsy, 
			TextureBase* tex, unsigned int color = 0xFFFFFFFF, bool isfont = false )
		{
			// Get the place on the screen
			float fx = (float)(x) - (float)mSize.x / 2 - 0.5f;
			float fy = (float)(y) - (float)mSize.y / 2 - 0.5f;

			// Get the width for the screen
			float fsx = (float)sx;// / (float)mSize.x;
			float fsy = (float)sy;// / (float)mSize.y;

			// Get the place for the TextureBase
			float ftx = (float)tx / (float)tex->GetWidth( );
			float fty = (float)ty / (float)tex->GetHeight( );

			// Get the width for the TextureBase
			float ftsx = (float)tsx / (float)tex->GetWidth( );
			float ftsy = (float)tsy / (float)tex->GetHeight( );
			
			OverlayQuad q = {
				tex,
				isfont ? 1 : 0,
				{
					{ Vector3( fx, fy, 1 ), Vector2( ftx, fty ), color },
					{ Vector3( fx, fy+fsy, 1 ), Vector2( ftx, fty+ftsy ), color },
					{ Vector3( fx+fsx, fy, 1 ), Vector2( ftx+ftsx, fty ), color },
					{ Vector3( fx+fsx, fy+fsy, 1 ), Vector2( ftx+ftsx, fty+ftsy ), color }
				}
			};
			mQueue.PushBack( q );

			return true;
		};

		bool Render( RenderSystem* rs )
		{
			//:: Set up shit!
			rs->SetCullMode( CullMode::NONE );
			rs->SetFillMode( FillMode::SOLID );
			rs->SetProjectionMatrix( Matrix4::CreateOrthoLH( (float)mSize.x, -(float)mSize.y, 0.0f, 1.0f ) );
			rs->SetViewMatrix( Matrix4::IDENTITY );
			rs->SetWorldMatrix( Matrix4::IDENTITY );
			rs->SetVertexDeclaration( mDecl );

			//:: Draw the buffer
			for( ArrList<OverlayQuad>::Iter i = mQueue.Begin( ); i.IsValid( ); ++i )
			{
				Material*& mat = mMat[ (*i).mattype ];
				mat->GetTextureStage( 0 ).SetTexture( (*i).tex );
				rs->SetMaterial( mat );
				rs->DrawPrimitiveUP( PrimitiveType::TriangleStrip, 2, &(*i).verts, sizeof( OverlayVert ) );
			}
			mQueue.Clear( );

			return true;
		};

	private:
		VertexDeclaration* mDecl;
		Material* mMat[2];
		Font* mDefaultFont;
		ArrList<OverlayQuad> mQueue;
		Point mSize;
	};
};