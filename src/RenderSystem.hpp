#pragma once
#include <windows.h>
#include "Color.hpp"
#include "Matrix4.hpp"
#include "Index.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "VertexDeclaration.hpp"
#include "VertexShader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace Halia
{
	namespace CompareMode
	{
		enum CompareMode
		{
			NEVER = 1,
			LESS = 2,
			EQUAL = 3,
			LESSEQUAL = 4,
			GREATER = 5,
			NOTEQUAL = 6,
			GREATEREQUAL = 7,
			ALWAYS = 8,
		};
	};

	namespace CullMode
	{
		enum CullMode
		{
			NONE = 0,
			CW = 1,
			CCW = 2
		};
	};

	namespace FillMode
	{
		enum FillMode
		{
			POINT,
			WIRE,
			SOLID
		};
	};

	namespace TextureState
	{
		enum TextureState
		{
			COLOROP = 1,
			COLORARG1 = 2,
			COLORARG2 = 3,
			ALPHAOP = 4,
			ALPHAARG1 = 5,
			ALPHAARG2 = 6,
			BUMPENVMAT00 = 7,
			BUMPENVMAT01 = 8,
			BUMPENVMAT10 = 9,
			BUMPENVMAT11 = 10,
			TEXCOORDINDEX = 11,
			BUMPENVLSCALE = 22,
			BUMPENVLOFFSET = 23,
			TEXTURETRANSFORMFLAGS = 24,
			COLORARG0 = 26,
			ALPHAARG0 = 27,
			RESULTARG = 28,
			CONSTANT = 32,
		};
	};

	namespace PrimitiveType
	{
		enum PrimitiveType
		{
			PointList = 1,
			LineList = 2,
			LineStrip = 3,
			TriangleList = 4,
			TriangleStrip = 5,
			TriangleFan = 6
		};
	};

	class RenderSystem
	{
	public:
		RenderSystem( );
		~RenderSystem( );

		virtual bool Init( HWND hwnd, int width, int height, bool fullscreen = false, int msquality = -1 ) = 0;
		virtual bool Clear( const Color& color ) = 0;
		virtual bool BeginScene( ) = 0;
		virtual bool EndScene( ) = 0;
		virtual bool Present( ) = 0;

		virtual void SetProjectionMatrix( const Matrix4& mat ) = 0;
		virtual void SetViewMatrix( const Matrix4& mat ) = 0;
		virtual void SetWorldMatrix( const Matrix4& mat ) = 0;
		virtual void SetFillMode( FillMode::FillMode fm ) = 0;
		virtual void SetCullMode( CullMode::CullMode cm ) = 0;
		virtual void SetAlphaBlendEnabled( bool enabled ) = 0;
		virtual void SetAlphaTestEnabled( bool enabled) = 0;
		virtual void SetAlphaFunction( CompareMode::CompareMode func ) = 0;
		virtual void SetAlphaReference( unsigned int ref ) = 0;
		virtual void SetDepthWriteEnabled( bool enabled ) = 0;
		virtual void SetDepthBufferEnabled( bool enabled ) = 0;
		virtual void SetSpecularEnabled( bool enabled ) = 0;
		virtual void SetLightingEnabled( bool enabled ) = 0;
		//virtual void SetTexture( int stageid, Texture* tex ) = 0;
		virtual int SetMaterial( Material* mat ) = 0;
		virtual int SetVertexDeclaration( VertexDeclaration* vdecl ) = 0;
		virtual int SetVertexShader( VertexShader* vshader ) = 0;
		virtual int SetStreamSource( int streamidx, VertexBufferBase* buffer, int start ) = 0;
		virtual int SetIndexSource( IndexBufferBase* buffer ) = 0;
		virtual bool DrawPrimitive( PrimitiveType::PrimitiveType type, int start, int count ) = 0;
		virtual bool DrawPrimitiveUP( PrimitiveType::PrimitiveType type, int count, void* data, int stride ) = 0;
		virtual bool DrawIndexedPrimitive( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count ) = 0;
		virtual bool DrawIndexedPrimitiveUP( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count, void* vdata, int vstride, void* idata, int istride ) = 0;
		virtual int GetBatchCount( ) = 0;
		virtual int GetPrimCount( ) = 0;

	protected:
		
	};
};