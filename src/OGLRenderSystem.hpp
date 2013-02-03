#pragma once
#include "RenderSystem.hpp"
#include <gl\gl.h>
#include <gl\glext.h>
#include <gl\wglext.h>

namespace Halia
{
	class OGLRenderSystem : public RenderSystem
	{
	public:
		OGLRenderSystem( );
		~OGLRenderSystem( );

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

	private:
		int GetVertexCount( PrimitiveType::PrimitiveType type, int count );
		bool ApplyVertexDeclaration( int stride, char* data = 0 );
		void ResetClientState( );

		GLuint GetTextureCache( TextureBase* tex );
		GLuint GetVertexBufferCache( VertexBufferBase* vb );
		GLuint GetIndexBufferCache( IndexBufferBase* ib );

		void BlendArgToGL( BlendArg::BlendArg arg, BOOL isAlpha, GLenum* source, GLenum* operand );
		void ApplyBlend( BlendOp::BlendOp op, BlendArg::BlendArg arg1, BlendArg::BlendArg arg2, BOOL isAlpha );

	private:
		HDC mTargetDC;
		HGLRC mRenderContext;


		unsigned int mBatchCount;
		unsigned int mPrimCount;

		struct {
			Matrix4 mViewMatrix;

			VertexDeclaration* mVDeclaration;
			GLuint mIndexBuffer;
			GLuint mVertexBuffer;

			GLenum mAlphaFunc;
			float mAlphaRef;
		} mRenderState;

	private:
		PFNGLACTIVETEXTUREARBPROC	glActiveTexture;
		PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTexture;
		PFNGLGENBUFFERSARBPROC glGenBuffers;
		PFNGLBINDBUFFERARBPROC glBindBuffer;
		PFNGLBUFFERDATAARBPROC glBufferData;
		PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;
	};
};
