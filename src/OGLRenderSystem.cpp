#include "OGLRenderSystem.hpp"

namespace Halia
{
	OGLRenderSystem::OGLRenderSystem( )
	{
	};

	OGLRenderSystem::~OGLRenderSystem( )
	{
	};

	bool OGLRenderSystem::Init( HWND hwnd, int width, int height, bool fullscreen, int msquality )
	{
		mTargetDC = GetWindowDC( hwnd );

		static PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, 
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 
			32, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 32, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0, };

		int pixelFormat = ChoosePixelFormat(mTargetDC, &pfd);
		SetPixelFormat(mTargetDC, pixelFormat, &pfd);
		mRenderContext = wglCreateContext( mTargetDC );

		if( msquality > 0 )
		{
			int iAttributes[] = { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_COLOR_BITS_ARB, 32,
				WGL_ALPHA_BITS_ARB, 8,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_SAMPLE_BUFFERS_ARB, GL_FALSE,
				WGL_SAMPLES_ARB, 0,
				0, 0 };

			iAttributes[15] = GL_TRUE;
			iAttributes[17] = msquality;
			int pixelFormat;
			UINT numFormats;
			float fAttributes[] = { 0, 0 };

			PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
			if( !wglChoosePixelFormatARB )
				return false;

			BOOL valid = wglChoosePixelFormatARB( mTargetDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats );
			if( !valid || numFormats < 1 )
				return false;

			wglDeleteContext(mRenderContext);

			//recreate window.. your mum..

			valid = SetPixelFormat( mTargetDC, pixelFormat, &pfd );
			if( !valid )
				return false;

			mRenderContext = wglCreateContext( mTargetDC );
		}

		wglMakeCurrent( mTargetDC, mRenderContext );

		glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress( "glClientActiveTextureARB" );
		glActiveTexture = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress( "glActiveTextureARB" );
		glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress( "glGenBuffersARB" );
		glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress( "glBindBufferARB" );
		glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress( "glBufferDataARB" );
		glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress( "glDeleteBuffersARB" );


		if( msquality > 0 )
			glEnable( GL_MULTISAMPLE_ARB );
		else
			glDisable( GL_MULTISAMPLE_ARB );

		glClearDepth( 1.0f );

		mRenderState.mAlphaRef = 128;
		mRenderState.mAlphaFunc = GL_ALWAYS;
		mRenderState.mVDeclaration = 0;

		return true;
	};

	bool OGLRenderSystem::Clear( const Color& color )
	{
		glClearColor( color.r, color.g, color.b, color.a );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		return false;
	};

	bool OGLRenderSystem::BeginScene( )
	{
		mBatchCount = 0;
		mPrimCount = 0;

		return true;
	};

	bool OGLRenderSystem::EndScene( )
	{
		return true;
	};

	bool OGLRenderSystem::Present( )
	{
		return SwapBuffers( mTargetDC ) == TRUE;
	};

	int OGLRenderSystem::GetBatchCount( )
	{
		return mBatchCount;
	};

	int OGLRenderSystem::GetPrimCount( )
	{
		return mPrimCount;
	};

	void OGLRenderSystem::SetProjectionMatrix( const Matrix4& mat )
	{
		glMatrixMode( GL_PROJECTION );
		glLoadMatrixf( &mat.m[ 0 ][ 0 ] );
	};

	void OGLRenderSystem::SetViewMatrix( const Matrix4& mat )
	{
		glMatrixMode( GL_MODELVIEW );
		mRenderState.mViewMatrix = mat;
		glLoadMatrixf( &mRenderState.mViewMatrix.m[ 0 ][ 0 ] );
	};

	void OGLRenderSystem::SetWorldMatrix( const Matrix4& mat )
	{
		glMatrixMode( GL_MODELVIEW );
		glLoadMatrixf( &mRenderState.mViewMatrix.m[ 0 ][ 0 ] );
		glMultMatrixf( &mat.m[0][0] );
	};

	void OGLRenderSystem::SetFillMode( FillMode::FillMode fm )
	{
		if( fm == FillMode::POINT )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
		}
		else if( fm == FillMode::WIRE )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		else if( fm == FillMode::SOLID )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
	};

	void OGLRenderSystem::SetCullMode( CullMode::CullMode cm )
	{
		if( cm == CullMode::NONE )
		{
			glDisable( GL_CULL_FACE );
		}
		else if( cm == CullMode::CCW )
		{
			glEnable( GL_CULL_FACE );
			glFrontFace( GL_CW );
		}
		else if( cm == CullMode::CW )
		{
			glEnable( GL_CULL_FACE );
			glFrontFace( GL_CCW );
		}
	};

	void OGLRenderSystem::SetAlphaBlendEnabled( bool enabled )
	{
		if( enabled ){
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}else{
			glDisable( GL_BLEND );
		}
	};

	void OGLRenderSystem::SetAlphaTestEnabled( bool enabled )
	{
		if( enabled )
			glEnable( GL_ALPHA_TEST );
		else
			glDisable( GL_ALPHA_TEST );
	};

	void OGLRenderSystem::SetAlphaFunction( CompareMode::CompareMode func )
	{
		mRenderState.mAlphaFunc = func + (GL_NEVER - 1);
		glAlphaFunc( mRenderState.mAlphaFunc, mRenderState.mAlphaRef );
	};

	void OGLRenderSystem::SetAlphaReference( unsigned int ref )
	{
		mRenderState.mAlphaRef = float(ref) / 255.0f;
		glAlphaFunc( mRenderState.mAlphaFunc, mRenderState.mAlphaRef );
	};

	void OGLRenderSystem::SetDepthWriteEnabled( bool enabled )
	{
		glDepthMask( enabled );
	};

	void OGLRenderSystem::SetDepthBufferEnabled( bool enabled )
	{
		if( enabled )
			glEnable( GL_DEPTH_TEST );
		else
			glDisable( GL_DEPTH_TEST );
	};

	void OGLRenderSystem::SetSpecularEnabled( bool enabled )
	{
		//:: TODO
	};

	void OGLRenderSystem::SetLightingEnabled( bool enabled )
	{
		if( enabled )
			glEnable( GL_LIGHTING );
		else
			glDisable( GL_LIGHTING );
	};


	void OGLRenderSystem::BlendArgToGL( BlendArg::BlendArg arg, BOOL isAlpha, GLenum* source, GLenum* operand ) {
		*operand = ( isAlpha )?GL_SRC_ALPHA:GL_SRC_COLOR;
		switch( arg ) {
			case BlendArg::DIFFUSE:
				*source = GL_PRIMARY_COLOR_EXT;
					break;
			case BlendArg::CURRENT:
				*source = GL_PREVIOUS_EXT;
				break;
			case BlendArg::TEXTURE:
				*source = GL_TEXTURE;
				break;
			case BlendArg::CONSTANT:
			case BlendArg::TFACTOR:
				*source = GL_CONSTANT_EXT;
				break;
			case BlendArg::SPECULAR:
				*source = GL_TEXTURE;
				break;
		};
	}

	void OGLRenderSystem::ApplyBlend( BlendOp::BlendOp op, BlendArg::BlendArg arg1, BlendArg::BlendArg arg2, BOOL isAlpha )
	{
		GLenum src1, opr1;
		GLenum src2, opr2;
		GLenum comb_target;
		GLenum src0_target, src1_target, src2_target;
		GLenum opr0_target, opr1_target, opr2_target;
		GLenum scal_target;
		GLenum opr=0, invopr, src3_target, opr3_target;

		BlendArgToGL( arg1, isAlpha, &src1, &opr1 );
		BlendArgToGL( arg2, isAlpha, &src2, &opr2 );

		if (isAlpha) {
			comb_target = GL_COMBINE_ALPHA_EXT;
			src0_target = GL_SOURCE0_ALPHA_EXT;
			src1_target = GL_SOURCE1_ALPHA_EXT;
			src2_target = GL_SOURCE2_ALPHA_EXT;
			opr0_target = GL_OPERAND0_ALPHA_EXT;
			opr1_target = GL_OPERAND1_ALPHA_EXT;
			opr2_target = GL_OPERAND2_ALPHA_EXT;
			scal_target = GL_ALPHA_SCALE;
		} else {
			comb_target = GL_COMBINE_RGB_EXT;
			src0_target = GL_SOURCE0_RGB_EXT;
			src1_target = GL_SOURCE1_RGB_EXT;
			src2_target = GL_SOURCE2_RGB_EXT;
			opr0_target = GL_OPERAND0_RGB_EXT;
			opr1_target = GL_OPERAND1_RGB_EXT;
			opr2_target = GL_OPERAND2_RGB_EXT;
			scal_target = GL_RGB_SCALE_EXT;
		}

		if (isAlpha) {
			 opr = GL_SRC_ALPHA;
			invopr = GL_ONE_MINUS_SRC_ALPHA;
			src3_target = GL_SOURCE3_ALPHA_NV;
			opr3_target = GL_OPERAND3_ALPHA_NV;
			 } else {
				opr = GL_SRC_COLOR;
				invopr = GL_ONE_MINUS_SRC_COLOR;
				src3_target = GL_SOURCE3_RGB_NV;
				opr3_target = GL_OPERAND3_RGB_NV;
				  }

		switch( op ) {
			case BlendOp::SELECTARGUMENT1:
				/*glTexEnvi(GL_TEXTURE_ENV, comb_target, GL_REPLACE);
				glTexEnvi(GL_TEXTURE_ENV, src0_target, src1);
				glTexEnvi(GL_TEXTURE_ENV, opr0_target, opr1);
				glTexEnvi(GL_TEXTURE_ENV, scal_target, 1);*/

				glTexEnvi(GL_TEXTURE_ENV, comb_target, GL_ADD);
				glTexEnvi(GL_TEXTURE_ENV, src0_target, src1);
				glTexEnvi(GL_TEXTURE_ENV, opr0_target, opr1);
				glTexEnvi(GL_TEXTURE_ENV, src1_target, GL_ZERO);
				glTexEnvi(GL_TEXTURE_ENV, opr1_target, invopr);
				glTexEnvi(GL_TEXTURE_ENV, src2_target, GL_ZERO);
				glTexEnvi(GL_TEXTURE_ENV, opr2_target, opr);
				glTexEnvi(GL_TEXTURE_ENV, src3_target, GL_ZERO);
				glTexEnvi(GL_TEXTURE_ENV, opr3_target, opr);
				break;

			case BlendOp::MODULATE:
				/*glTexEnvi(GL_TEXTURE_ENV, comb_target, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, src0_target, src1);
				glTexEnvi(GL_TEXTURE_ENV, opr0_target, opr1);
				glTexEnvi(GL_TEXTURE_ENV, src1_target, src2);
				glTexEnvi(GL_TEXTURE_ENV, opr1_target, opr2);
				glTexEnvi(GL_TEXTURE_ENV, scal_target, 1);*/
				glTexEnvi(GL_TEXTURE_ENV, comb_target, GL_ADD);
				glTexEnvi(GL_TEXTURE_ENV, src0_target, src1);
				glTexEnvi(GL_TEXTURE_ENV, opr0_target, opr1);
				glTexEnvi(GL_TEXTURE_ENV, src1_target, src2);
				glTexEnvi(GL_TEXTURE_ENV, opr1_target, opr2);
				glTexEnvi(GL_TEXTURE_ENV, src2_target, GL_ZERO);
				glTexEnvi(GL_TEXTURE_ENV, opr2_target, opr);
				glTexEnvi(GL_TEXTURE_ENV, src3_target, GL_ZERO);
				glTexEnvi(GL_TEXTURE_ENV, opr3_target, opr);
				glTexEnvi(GL_TEXTURE_ENV, scal_target, 1);
				break;

			case BlendOp::MODULATE2X:
				/*glTexEnvi(GL_TEXTURE_ENV, comb_target, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, src0_target, src1);
				glTexEnvi(GL_TEXTURE_ENV, opr0_target, opr1);
				glTexEnvi(GL_TEXTURE_ENV, src1_target, src2);
				glTexEnvi(GL_TEXTURE_ENV, opr1_target, opr2);
				glTexEnvi(GL_TEXTURE_ENV, scal_target, 2);*/
				glTexEnvi(GL_TEXTURE_ENV, comb_target, GL_ADD);
				glTexEnvi(GL_TEXTURE_ENV, src0_target, src1);
				glTexEnvi(GL_TEXTURE_ENV, opr0_target, opr1);
				glTexEnvi(GL_TEXTURE_ENV, src1_target, src2);
				glTexEnvi(GL_TEXTURE_ENV, opr1_target, opr2);
				glTexEnvi(GL_TEXTURE_ENV, src2_target, GL_ZERO);
				glTexEnvi(GL_TEXTURE_ENV, opr2_target, opr);
				glTexEnvi(GL_TEXTURE_ENV, src3_target, GL_ZERO);
				glTexEnvi(GL_TEXTURE_ENV, opr3_target, opr);
				glTexEnvi(GL_TEXTURE_ENV, scal_target, 2);

				break;

			case BlendOp::BLENDTEXTUREALPHA:
				/*glTexEnvi(GL_TEXTURE_ENV, comb_target, GL_INTERPOLATE_EXT);
				glTexEnvi(GL_TEXTURE_ENV, src0_target, src1);
				glTexEnvi(GL_TEXTURE_ENV, opr0_target, opr1);
				glTexEnvi(GL_TEXTURE_ENV, src1_target, src2);
				glTexEnvi(GL_TEXTURE_ENV, opr1_target, opr2);
				glTexEnvi(GL_TEXTURE_ENV, src2_target, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, opr2_target, GL_SRC_ALPHA);
				glTexEnvi(GL_TEXTURE_ENV, scal_target, 1);*/

				glTexEnvi(GL_TEXTURE_ENV, comb_target, GL_ADD);
				glTexEnvi(GL_TEXTURE_ENV, src0_target, src1);
				glTexEnvi(GL_TEXTURE_ENV, opr0_target, opr1);
				glTexEnvi(GL_TEXTURE_ENV, src1_target, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, opr1_target, invopr);
				glTexEnvi(GL_TEXTURE_ENV, src2_target, src2);
				glTexEnvi(GL_TEXTURE_ENV, opr2_target, opr2);
				glTexEnvi(GL_TEXTURE_ENV, src3_target, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, opr3_target, GL_ONE_MINUS_SRC_ALPHA);
				glTexEnvi(GL_TEXTURE_ENV, scal_target, 1);
				break;
		};
	}

	int OGLRenderSystem::SetMaterial( Material* mat )
	{
		if( mat == 0 )
			return 1;

		int j = 0;
		for( std::vector< TextureStage >::iterator i = mat->mTStages.begin( ); i != mat->mTStages.end( ); i++, j++ )
		{
			glActiveTexture( GL_TEXTURE0_ARB + j );

			GLuint cTex = GetTextureCache( (*i).texture );
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, cTex );

			if( !(*i).uses_transform ) {
				glMatrixMode( GL_TEXTURE );
				glLoadIdentity( );
			} else {
				glMatrixMode( GL_TEXTURE );
				glLoadMatrixf( &(*i).transform.m[ 0 ][ 0 ] );
			}

			ApplyBlend( (*i).colormode.op, (*i).colormode.arg1, (*i).colormode.arg2, FALSE );
			ApplyBlend( (*i).alphamode.op, (*i).alphamode.arg1, (*i).alphamode.arg2, TRUE );
		}

		SetAlphaBlendEnabled( mat->mAlphaBlend );
		SetAlphaTestEnabled( mat->mAlphaTest );
		SetDepthWriteEnabled( mat->mDepthWrite );
		SetDepthBufferEnabled( mat->mDepthBuffer );
		SetSpecularEnabled( mat->mSpecularEnabled );

		return 0;
	};

	int OGLRenderSystem::SetVertexDeclaration( VertexDeclaration* vdecl )
	{
		mRenderState.mVDeclaration = vdecl;
		
		return 0;
	};

	int OGLRenderSystem::SetVertexShader( VertexShader* vshader )
	{
		return 0;
	};

	int OGLRenderSystem::SetStreamSource( int streamidx, VertexBufferBase* buffer, int start )
	{
		mRenderState.mVertexBuffer = GetVertexBufferCache( buffer );

		return 0;
	};

	int OGLRenderSystem::SetIndexSource( IndexBufferBase* buffer )
	{
		mRenderState.mIndexBuffer = GetIndexBufferCache( buffer );

		return 0;
	};

	bool OGLRenderSystem::DrawPrimitive( PrimitiveType::PrimitiveType type, int start, int count )
	{
		glBindBuffer( GL_ARRAY_BUFFER, mRenderState.mVertexBuffer );

		if( !ApplyVertexDeclaration( mRenderState.mVDeclaration->GetStride( 0 ) ) )
			return false;

		glDrawArrays( type, 0, GetVertexCount( type, count ) );

		ResetClientState( );

		++mBatchCount;
		mPrimCount += count;

		return true;
	};

	bool OGLRenderSystem::DrawPrimitiveUP( PrimitiveType::PrimitiveType type, int count, void* data, int stride )
	{
		if( !ApplyVertexDeclaration( stride, (char*)data ) )
			return false;

		glDrawArrays( type, 0, GetVertexCount( type, count ) );

		ResetClientState( );

		++mBatchCount;
		mPrimCount += count;

		return true;
	};

	bool OGLRenderSystem::DrawIndexedPrimitive( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count )
	{
		glBindBuffer( GL_ARRAY_BUFFER, mRenderState.mVertexBuffer );

		int stride = mRenderState.mVDeclaration->GetStride( 0 );
		if( !ApplyVertexDeclaration( stride, (char*)(vstart * stride) ) )
			return false;

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mRenderState.mIndexBuffer );
		glDrawElements( type, GetVertexCount( type, count ), GL_UNSIGNED_SHORT, 0 );

		ResetClientState( );

		++mBatchCount;
		mPrimCount += count;

		return true;
	};

	bool OGLRenderSystem::DrawIndexedPrimitiveUP( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count, void* vdata, int vstride, void* idata, int istride )
	{
		//++mBatchCount;
		//mPrimCount += count;
		return true;
	};

	void OGLRenderSystem::ResetClientState( )
	{
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );

		glClientActiveTexture( GL_TEXTURE0_ARB );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glClientActiveTexture( GL_TEXTURE1_ARB );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glClientActiveTexture( GL_TEXTURE2_ARB );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );

		glActiveTexture( GL_TEXTURE0_ARB );
		glDisable( GL_TEXTURE_2D );
		glActiveTexture( GL_TEXTURE1_ARB );
		glDisable( GL_TEXTURE_2D );
		glActiveTexture( GL_TEXTURE2_ARB );
		glDisable( GL_TEXTURE_2D );
	};

	int OGLRenderSystem::GetVertexCount( PrimitiveType::PrimitiveType type, int count )
	{
		switch( type ){
			case PrimitiveType::LineList: return count * 2;
			case PrimitiveType::LineStrip: return count + 1;
			case PrimitiveType::TriangleList: return count * 3;
			case PrimitiveType::TriangleStrip: return count + 2;
			case PrimitiveType::TriangleFan: return count + 2;
		}

		return count;
	};

	bool OGLRenderSystem::ApplyVertexDeclaration( int stride, char* data )
	{
		static const int glVDeclType[] = { GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_FLOAT, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_SHORT, GL_UNSIGNED_BYTE, GL_SHORT, GL_SHORT, GL_UNSIGNED_SHORT, GL_UNSIGNED_SHORT, 0, 0, 0, 0 };
		static const int glVDeclCount[] = {  1, 2,  3,  4, 4, 4, 2, 4, 4, 2, 4, 2, 4, 0, 0, 0, 0, 0 };
		if( !mRenderState.mVDeclaration )
			return false;

		int curPos = 0;
		for( VertexElement::List::iterator i = mRenderState.mVDeclaration->mElements.begin( ); i != mRenderState.mVDeclaration->mElements.end( ); i++ )
		{
			switch( (*i).semantic )
			{
			case VDeclSemantic::POSITION:
				glEnableClientState( GL_VERTEX_ARRAY );
				glVertexPointer( glVDeclCount[ (*i).type ], glVDeclType[ (*i).type ], stride, data + curPos );
				break;
			case VDeclSemantic::TEXCOORD:
				glClientActiveTexture( GL_TEXTURE0_ARB + (*i).usageidx );
				glTexCoordPointer( glVDeclCount[ (*i).type ], glVDeclType[ (*i).type ], stride, data + curPos );
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				break;
			case VDeclSemantic::COLOUR:
				glEnableClientState( GL_COLOR_ARRAY );
				glColorPointer( glVDeclCount[ (*i).type ], glVDeclType[ (*i).type ], stride, data + curPos );
				break;
			}

			curPos += VDeclType::LENGTH[ (*i).type ];
		}

		return true;
	};

	GLuint OGLRenderSystem::GetTextureCache( TextureBase* tex )
	{
		GLuint& cache = (GLuint&)tex->mCache;
		if( tex->IsDirty( ) )
		{
			if( cache ) {
				glDeleteTextures( 1, &cache );
				cache = 0;
			}

			int texformat = tex->GetFormat( );
			if( texformat == 0 ) {
				glGenTextures( 1, &cache );
				if( !cache )
				{
					printf( "Failed to generate texture :: cache = 0\n" );
					return 0;
				}

				glBindTexture( GL_TEXTURE_2D, cache );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

				DWORD dwFlags = *(DWORD*)(tex->mData + 0x50);
				if( dwFlags & 0x40 )
				{
					DWORD bpp = *(DWORD*)(tex->mData + 0x58);
					int hasAlpha = dwFlags & 0x1;
					DWORD aMask = *(DWORD*)(tex->mData + 0x68);

					int type;
					int format;
					int data;

					if( bpp == 16 ) {
						if( aMask == 0x80 ) {
							//a1r5g5b5
							format = GL_RGB5_A1;
							type = GL_BGRA;
							data = GL_UNSIGNED_SHORT_1_5_5_5_REV;
						} else {
							//a4r4g4b4
							if( hasAlpha ) {
								format = GL_RGBA4;
								type = GL_BGRA;
								data = GL_UNSIGNED_SHORT_4_4_4_4_REV;
							} else {
								format = GL_RGB4;
								type = GL_BGR;
								data = GL_UNSIGNED_SHORT_4_4_4_4_REV;
							}
						}
					} else if( bpp == 32 ) {
						if( hasAlpha ) {
							format = GL_RGBA;
							type = GL_BGRA;
							data = GL_UNSIGNED_INT_8_8_8_8_REV;
						} else {
							format = GL_RGB;
							type = GL_BGR;
							data = GL_UNSIGNED_INT_8_8_8_8_REV;
						}
					}

					DWORD width = tex->GetWidth( );
					DWORD height = tex->GetHeight( );

					glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, type, data, (GLvoid*)(tex->mData + 0x80) );
				} else {
					FIMEMORY* imgmem = FreeImage_OpenMemory( tex->mData, tex->mDataLen );
					FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory( imgmem );
					FIBITMAP* img = FreeImage_LoadFromMemory( fif, imgmem, 0 );

					FIBITMAP* temp = img;
					img = FreeImage_ConvertTo32Bits( img );
					FreeImage_FlipVertical( img );

					glTexImage2D( GL_TEXTURE_2D, 0, 4, FreeImage_GetWidth( img ), FreeImage_GetHeight( img ), 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)FreeImage_GetBits( img ) );

					FreeImage_Unload( temp );
					FreeImage_Unload( img );
					FreeImage_CloseMemory( imgmem );
				}
			} else if( texformat == TextureFormat::A8 ) {
				glGenTextures( 1, &cache );
				glBindTexture( GL_TEXTURE_2D, cache );
				glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA8, tex->GetWidth( ), tex->GetHeight( ), 0, GL_ALPHA, GL_UNSIGNED_BYTE, (GLvoid*)tex->mData );
			} else {
				printf( "Failed to create Texture Cache :: Invalid format: %d\n", texformat );
				return 0;
			}

			tex->SetDirty( false );
		}
		return cache;
	};

	GLuint OGLRenderSystem::GetVertexBufferCache( VertexBufferBase* vb )
	{
		GLuint& cache = (GLuint&)vb->mCache;
		if( vb->IsDirty( ) )
		{
			if( cache ) {
				glDeleteBuffers( 1, &cache );
				cache = 0;
			}

			glGenBuffers( 1, &cache );
			if( !cache )
			{
				printf( "Failed to generate vertex buffer :: cache = 0\n" );
				return 0;
			}

			glBindBuffer( GL_ARRAY_BUFFER, cache );
			glBufferData( GL_ARRAY_BUFFER, vb->GetBufferSize( ), vb->GetBuffer( ), GL_STATIC_DRAW );

			vb->SetDirty( false );
		}
		return cache;
	};

	GLuint OGLRenderSystem::GetIndexBufferCache( IndexBufferBase* ib )
	{
		GLuint& cache = (GLuint&)ib->mCache;
		if( ib->IsDirty( ) )
		{
			if( cache ) {
				glDeleteBuffers( 1, &cache );
				cache = 0;
			}

			glGenBuffers( 1, &cache );
			if( !cache )
			{
				printf( "Failed to generate index buffer :: cache = 0\n" );
				return 0;
			}

			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, cache );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, ib->GetBufferSize( ), ib->GetBuffer( ), GL_STATIC_DRAW );

			ib->SetDirty( false );
		}
		return cache;
	};
};