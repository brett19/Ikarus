#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "RenderSystem.hpp"

#define DX9MAXFRAMECACHE 100000

namespace Halia
{
	class DX9RenderSystem : public RenderSystem
	{
	public:
		DX9RenderSystem( );
		~DX9RenderSystem( );

		//:: Initialize crap
		bool Init( HWND hwnd, int width, int height, bool fullscreen = false, int msquality = -1 );

		//:: Scene functions
		bool Clear( const Color& color );
		bool BeginScene( );
		bool EndScene( );
		bool Present( );
		int GetBatchCount( );
		int GetPrimCount( );

		//:: Camera functions
		void SetProjectionMatrix( const Matrix4& mat );
		void SetViewMatrix( const Matrix4& mat );
		void SetWorldMatrix( const Matrix4& mat );
		
		//:: Render state stuff
		void SetFillMode( FillMode::FillMode fm );
		void SetCullMode( CullMode::CullMode cm );
		void SetAlphaBlendEnabled( bool enabled );
		void SetAlphaTestEnabled( bool enabled );
		void SetAlphaFunction( CompareMode::CompareMode func );
		void SetAlphaReference( unsigned int ref );
		void SetDepthWriteEnabled( bool enabled );
		void SetDepthBufferEnabled( bool enabled );
		void SetSpecularEnabled( bool enabled );
		void SetLightingEnabled( bool enabled );

		//:: Texture stuff
		void SetTextureMatrix( int stageid, const Matrix4& mat );
		void SetTextureStageState( int stageid, TextureState::TextureState state, int value );
		int SetTexture( int stageid, TextureBase* tex );

		//:: Master shit
		int SetMaterial( Material* mat );
		int SetVertexDeclaration( VertexDeclaration* vdecl );
		int SetVertexShader( VertexShader* vshader );
		int SetStreamSource( int streamidx, VertexBufferBase* buffer, int start );
		int SetIndexSource( IndexBufferBase* buffer );

		//:: Drawing stuff
		bool DrawPrimitive( PrimitiveType::PrimitiveType type, int start, int count );
		bool DrawPrimitiveUP( PrimitiveType::PrimitiveType type, int count, void* data, int stride );
		bool DrawIndexedPrimitive( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count );
		bool DrawIndexedPrimitiveUP( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count, void* vdata, int vstride, void* idata, int istride );
		
		//:: DirectX only
		void SetRenderState( D3DRENDERSTATETYPE state, unsigned int value );
		LPDIRECT3DVERTEXBUFFER9 GetVertexBufferCache( VertexBufferBase* vb );
		LPDIRECT3DINDEXBUFFER9 GetIndexBufferCache( IndexBufferBase* ib );
		LPDIRECT3DTEXTURE9 GetTextureCache( TextureBase* tex );
		LPDIRECT3DVERTEXDECLARATION9 GetVertexDeclCache( VertexDeclaration* decl );
		LPDIRECT3DVERTEXSHADER9 GetVertexShaderCache( VertexShader* shader );
		D3DXMATRIX* GetDXMatrix( const Matrix4& mat );
		void ResetStates( );

	private:
		LPDIRECT3D9 mD3d;
		LPDIRECT3DDEVICE9 mDevice;

		bool mRenderStateCached[ 256 ];
		unsigned int mRenderStates[ 256 ];
		bool mTextureStateCached[ 8 ][ 36 ];
		unsigned int mTextureStates[ 8 ][ 36 ];
		VertexDeclaration* mDecl;
		VertexBufferBase* mVBuffer[ 8 ];
		IndexBufferBase* mIBuffer;
		TextureBase* mTexture[ 8 ];
		unsigned int mBatchCount;
		unsigned int mPrimCount;
		unsigned int mCacheCount;
		Matrix4 mMatProj;
		Matrix4 mMatView;
		Matrix4 mMatWorld;
	};
};