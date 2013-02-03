#include "DX9RenderSystem.hpp"

namespace Halia
{
	DX9RenderSystem::DX9RenderSystem( )
	{
	};

	DX9RenderSystem::~DX9RenderSystem( )
	{
	};

	bool DX9RenderSystem::Init( HWND hwnd, int width, int height, bool fullscreen, int msquality )
	{
		mD3d = Direct3DCreate9( D3D_SDK_VERSION );

		D3DPRESENT_PARAMETERS pp = { 0 };
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.EnableAutoDepthStencil = TRUE;
		pp.AutoDepthStencilFormat = D3DFMT_D24X8;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		if( msquality >= 0 )
		{
			pp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
			pp.MultiSampleQuality = msquality;
		}

		if( fullscreen )
		{
			pp.Windowed = false;
			pp.BackBufferCount = 1;
			pp.BackBufferWidth = width;
			pp.BackBufferHeight = height;
			pp.BackBufferFormat = D3DFMT_X8R8G8B8;
		}
		else
		{
			D3DDISPLAYMODE currentmode;
			if( FAILED(mD3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &currentmode )) )
				return false;

			// Set up windowed formatting
			pp.Windowed = true;
			pp.BackBufferCount = 1;
			pp.BackBufferFormat = currentmode.Format;
		}

		UINT adapterid = D3DADAPTER_DEFAULT;	
		D3DDEVTYPE rendertype = D3DDEVTYPE_HAL;

		int adaptercount = (int)mD3d->GetAdapterCount( );
		for( int i = 0; i < adaptercount; i++ )
		{
			D3DADAPTER_IDENTIFIER9 info;
			mD3d->GetAdapterIdentifier( i, 0, &info );
			
			if( strstr( info.Description, "PerfHUD" ) )
			{
				adapterid = i;
				rendertype = D3DDEVTYPE_REF;
				printf( "PerfHUD found on adapter %d (%s)\n", i, info.Description );
			}
		}

		if( FAILED(mD3d->CreateDevice( adapterid, rendertype, hwnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &mDevice )) )
			return false;

		for( int i = 0; i < 8; i++ )
			mDevice->SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );

		return true;
	};

	bool DX9RenderSystem::Clear( const Color& color )
	{
		if( FAILED(mDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
			color.ToDword( ), 1.0f, 0 )) )
		{
			printf( "Clear failed\n" );
			return false;
		}

		return true;
	};

	bool DX9RenderSystem::BeginScene( )
	{
		if( FAILED(mDevice->BeginScene( )) )
		{
			printf( "BeginScene failed\n" );
			return false;
		}

		ResetStates( );
		mBatchCount = 0;
		mPrimCount = 0;
		mCacheCount = 0;

		return true;
	};

	bool DX9RenderSystem::EndScene( )
	{
		if( FAILED(mDevice->EndScene( )) )
		{
			printf( "EndScene failed\n" );
			return false;
		}

		return true;
	};

	bool DX9RenderSystem::Present( )
	{
		HRESULT pres = mDevice->Present( NULL, NULL, NULL, NULL );
		if( pres == D3DERR_DEVICELOST ) {
			return false;
		} else if( FAILED(pres) ) {
			return false;
		}

		return true;
	};

	int DX9RenderSystem::GetBatchCount( )
	{
		return mBatchCount;
	};

	int DX9RenderSystem::GetPrimCount( )
	{
		return mPrimCount;
	};

	void DX9RenderSystem::SetProjectionMatrix( const Matrix4& mat )
	{
		mDevice->SetTransform( D3DTS_PROJECTION, GetDXMatrix( mat ) );
		mMatProj = mat;
	};

	void DX9RenderSystem::SetViewMatrix( const Matrix4& mat )
	{
		mDevice->SetTransform( D3DTS_VIEW, GetDXMatrix( mat ) );
		mMatView = mat;
	};

	void DX9RenderSystem::SetWorldMatrix( const Matrix4& mat )
	{
		mDevice->SetTransform( D3DTS_WORLD, GetDXMatrix( mat ) );
		mMatWorld = mat;
	};

	void DX9RenderSystem::SetTextureMatrix( int stageid, const Matrix4& mat )
	{
		mDevice->SetTransform( (D3DTRANSFORMSTATETYPE)( D3DTS_TEXTURE0 + stageid ), GetDXMatrix( mat ) );
	};

	void DX9RenderSystem::SetFillMode( FillMode::FillMode fm )
	{
		if( fm == FillMode::POINT )
			SetRenderState( D3DRS_FILLMODE, D3DFILL_POINT );
		else if( fm == FillMode::WIRE )
			SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		else
			SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	};

	void DX9RenderSystem::SetCullMode( CullMode::CullMode cm )
	{
		if( cm == CullMode::NONE )
			SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		else if( cm == CullMode::CW )
			SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
		else if( cm == CullMode::CCW )
			SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	};

	void DX9RenderSystem::SetAlphaBlendEnabled( bool enabled )
	{
		SetRenderState( D3DRS_ALPHABLENDENABLE, enabled );
		SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	};

	void DX9RenderSystem::SetAlphaTestEnabled( bool enabled )
	{
		SetRenderState( D3DRS_ALPHATESTENABLE, enabled );
		SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		SetRenderState( D3DRS_ALPHAREF, 128 );
	};

	void DX9RenderSystem::SetAlphaFunction( CompareMode::CompareMode func )
	{
		SetRenderState( D3DRS_ALPHAFUNC, func );
	};

	void DX9RenderSystem::SetAlphaReference( unsigned int ref )
	{
		SetRenderState( D3DRS_ALPHAREF, ref );
	};

	void DX9RenderSystem::SetDepthWriteEnabled( bool enabled )
	{
		SetRenderState( D3DRS_ZWRITEENABLE, enabled );
	};

	void DX9RenderSystem::SetDepthBufferEnabled( bool enabled )
	{
		SetRenderState( D3DRS_ZENABLE, enabled );
	};

	void DX9RenderSystem::SetSpecularEnabled( bool enabled )
	{
		SetRenderState( D3DRS_SPECULARENABLE, enabled );
	};

	void DX9RenderSystem::SetLightingEnabled( bool enabled )
	{
		SetRenderState( D3DRS_LIGHTING, enabled );
	};

	void DX9RenderSystem::SetTextureStageState( int stageid, TextureState::TextureState state, int value )
	{
		if( mTextureStateCached[ stageid ][ state ] && mTextureStates[ stageid ][ state ] == value )
			return;
		mTextureStateCached[ stageid ][ state ] = true;
		mTextureStates[ stageid ][ state ] = value;
		mDevice->SetTextureStageState( stageid, (D3DTEXTURESTAGESTATETYPE)state, value );
	};

	int DX9RenderSystem::SetTexture( int stageid, TextureBase* tex )
	{
		if( !tex )
			return 1;

		if( !tex->IsDirty( ) && mTexture[ stageid ] == tex )
			return 0;

		LPDIRECT3DTEXTURE9 dxbuf = GetTextureCache( tex );
		
		if( dxbuf ) {
			if( !FAILED(mDevice->SetTexture( stageid, dxbuf )) )
			{
				mTexture[ stageid ] = tex;
				return 0;
			}
		} else if( mCacheCount >= DX9MAXFRAMECACHE ) {
			return 2;
		}
		printf( "Failed to set texture\n" );
		return 1;
	};

	int DX9RenderSystem::SetMaterial( Material* mat )
	{
		if( mat == 0 )
			return 1;

		int j = 0;
		for( std::vector< TextureStage >::iterator i = mat->mTStages.begin( ); i != mat->mTStages.end( ); i++, j++ )
		{
			if( SetTexture( j, (*i).texture ) == 2 )
				return 2;

			if( !(*i).uses_transform ) {
				SetTextureStageState( j, TextureState::TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
			} else {
				SetTextureStageState( j, TextureState::TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
				SetTextureMatrix( j, (*i).transform );
			}

			SetTextureStageState( j, TextureState::COLOROP, (*i).colormode.op );
			if( (*i).colormode.op != BlendOp::DISABLE ) {
				SetTextureStageState( j, TextureState::COLORARG1, (*i).colormode.arg1 );
				SetTextureStageState( j, TextureState::COLORARG2, (*i).colormode.arg2 );
			}
			SetTextureStageState( j, TextureState::ALPHAOP, (*i).alphamode.op );
			if( (*i).alphamode.op != BlendOp::DISABLE ) {
				SetTextureStageState( j, TextureState::ALPHAARG1, (*i).alphamode.arg1 );
				SetTextureStageState( j, TextureState::ALPHAARG2, (*i).alphamode.arg2 );
			}
		}
		SetTextureStageState( j, TextureState::COLOROP, D3DTOP_DISABLE );
		SetTextureStageState( j, TextureState::ALPHAOP, D3DTOP_DISABLE );

		SetAlphaBlendEnabled( mat->mAlphaBlend );
		SetAlphaTestEnabled( mat->mAlphaTest );
		SetDepthWriteEnabled( mat->mDepthWrite );
		SetDepthBufferEnabled( mat->mDepthBuffer );
		SetSpecularEnabled( mat->mSpecularEnabled );

		return 0;
	};

	int DX9RenderSystem::SetVertexDeclaration( VertexDeclaration* vdecl )
	{
		if( !vdecl->IsDirty( ) && mDecl == vdecl )
			return 0;

		LPDIRECT3DVERTEXDECLARATION9 decl = GetVertexDeclCache( vdecl );

		if( decl ) {
			if( !FAILED(mDevice->SetVertexDeclaration( decl )) )
			{
				mDecl = vdecl;
				return 0;
			}
		} else if( mCacheCount >= DX9MAXFRAMECACHE ) {
			return 2;
		}
		printf( "Failed to set Vertex Declaration\n" );
		return 1;
	};

	int DX9RenderSystem::SetVertexShader( VertexShader* vshader )
	{
		LPDIRECT3DVERTEXSHADER9 shader = GetVertexShaderCache( vshader );
		
		if( shader ) {
			if( !FAILED(mDevice->SetVertexShader( shader )) )
			{
				Matrix4 mat = mMatWorld * mMatView * mMatProj;
				mDevice->SetVertexShaderConstantF( 0, (float*)GetDXMatrix(mat), 4 );
				return 0;
			}
		} else if( mCacheCount >= DX9MAXFRAMECACHE ) {
			return 2;
		}
		printf( "Failed to set Vertex Shader\n" );
		return 1;
	};

	int DX9RenderSystem::SetStreamSource( int streamidx, VertexBufferBase* buffer, int start )
	{
		if( !buffer->IsDirty( ) && mVBuffer[ streamidx ] == buffer )
			return 0;

		LPDIRECT3DVERTEXBUFFER9 dxbuf = GetVertexBufferCache( buffer );

		if( dxbuf ) {
			if( !FAILED(mDevice->SetStreamSource( streamidx, dxbuf, 0, buffer->GetStride( ) )) )
			{
				mVBuffer[ streamidx ] = buffer;
				return 0;
			}
		} else if( mCacheCount >= DX9MAXFRAMECACHE ) {
			return 2;
		}
		printf( "Failed to set stream source\n" );
		return 1;
	};

	int DX9RenderSystem::SetIndexSource( IndexBufferBase* buffer )
	{
		if( !buffer->IsDirty( ) && mIBuffer == buffer )
			return 0;

		LPDIRECT3DINDEXBUFFER9 dxbuf = GetIndexBufferCache( buffer );

		if( dxbuf ) {
			if( !FAILED(mDevice->SetIndices( dxbuf )) )
			{
				mIBuffer = buffer;
				return 0;
			}
		} else if( mCacheCount >= DX9MAXFRAMECACHE ) {
			return 2;
		}
		printf( "Failed to set index source\n" );
		return 1;
	};

	bool DX9RenderSystem::DrawPrimitive( PrimitiveType::PrimitiveType type, int start, int count )
	{
		if( FAILED(mDevice->DrawPrimitive( (D3DPRIMITIVETYPE)type, start, count )) ) 
		{
			printf( "DrawPrimitive failed\n" );
			return false;
		}

		mBatchCount++;
		mPrimCount += count;

		return true;
	};

	bool DX9RenderSystem::DrawPrimitiveUP( PrimitiveType::PrimitiveType type, int count, void* data, int stride )
	{
		if( FAILED(mDevice->DrawPrimitiveUP( (D3DPRIMITIVETYPE)type, count, data, stride )) )
		{
			printf( "DrawPrimitiveUP failed\n" );
			return false;
		}

		mBatchCount++;
		mPrimCount += count;

		return true;
	};

	bool DX9RenderSystem::DrawIndexedPrimitive( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count )
	{
		if( FAILED(mDevice->DrawIndexedPrimitive( (D3DPRIMITIVETYPE)type, vstart, istart, vcount, 0, count )) )
		{
			printf( "DrawIndexedPrimitive failed\n" );
			return false;
		}

		mBatchCount++;
		mPrimCount += count;

		return true;
	};

	bool DX9RenderSystem::DrawIndexedPrimitiveUP( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count, void* vdata, int vstride, void* idata, int istride )
	{
		D3DFORMAT idf;
		if( istride == 2 )
			idf = D3DFMT_INDEX16;
		else if( istride == 4 )
			idf = D3DFMT_INDEX32;
		else
			return false;

		if( FAILED(mDevice->DrawIndexedPrimitiveUP( (D3DPRIMITIVETYPE)type, vstart, vcount, count, idata, idf, vdata, vstride )) )
		{
			printf( "DrawIndexedPrimitiveUP failed\n" );
			return false;
		}

		mBatchCount++;
		mPrimCount += count;

		return true;
	};

	void DX9RenderSystem::SetRenderState( D3DRENDERSTATETYPE state, unsigned int value )
	{
		if( mRenderStateCached[ state ] && mRenderStates[ state ] == value )
			return;
		mRenderStateCached[ state ] = true;
		mRenderStates[ state ] = value;
		mDevice->SetRenderState( state, value );
	};

	LPDIRECT3DVERTEXBUFFER9 DX9RenderSystem::GetVertexBufferCache( VertexBufferBase* vb )
	{
		LPDIRECT3DVERTEXBUFFER9& cache = (LPDIRECT3DVERTEXBUFFER9&)vb->mCache;
		if( vb->IsDirty( ) )
		{
			if( mCacheCount >= DX9MAXFRAMECACHE )
				return 0;
			mCacheCount += 1;

			if( cache ) {
				cache->Release( );
				cache = 0;
			}

			if( FAILED(mDevice->CreateVertexBuffer( vb->GetBufferSize( ), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &cache, 0 )) )
			{
				printf( "Failed to create Vertex Buffer Cache\n" );
				return 0;
			}

			if( cache )
			{
				void* buf;
				if( FAILED(cache->Lock( 0, 0, &buf, 0 )) )
					return 0;
				memcpy( buf, vb->GetBuffer( ), vb->GetBufferSize( ) );
				if( FAILED(cache->Unlock( )) )
					return 0;
				vb->SetDirty( false );
			}
		}
		return cache;
	};

	LPDIRECT3DINDEXBUFFER9 DX9RenderSystem::GetIndexBufferCache( IndexBufferBase* ib )
	{
		LPDIRECT3DINDEXBUFFER9& cache = (LPDIRECT3DINDEXBUFFER9&)ib->mCache;
		if( ib->IsDirty( ) )
		{
			if( mCacheCount >= DX9MAXFRAMECACHE )
				return 0;
			mCacheCount += 1;

			if( cache ) {
				cache->Release( );
				cache = 0;
			}
	
			if( FAILED(mDevice->CreateIndexBuffer( ib->GetBufferSize( ), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &cache, 0 )) )
			{
				printf( "Failed to create Index Buffer Cache\n" );
				return 0;
			}

			if( cache )
			{
				void* buf;
				if( FAILED(cache->Lock( 0, 0, &buf, 0 )) )
					return 0;
				memcpy( buf, ib->GetBuffer( ), ib->GetBufferSize( ) );
				if( FAILED(cache->Unlock( )) )
					return 0;
				ib->SetDirty( false );
			}
		}
		return cache;
	};

	LPDIRECT3DTEXTURE9 DX9RenderSystem::GetTextureCache( TextureBase* tex )
	{
		LPDIRECT3DTEXTURE9& cache = (LPDIRECT3DTEXTURE9&)tex->mCache;
		if( tex->IsDirty( ) )
		{
			if( mCacheCount >= DX9MAXFRAMECACHE )
				return 0;
			mCacheCount += 1;

			if( cache ) {
				cache->Release( );
				cache = 0;
			}

			int texformat = tex->GetFormat( );
			if( texformat == 0 ) {
				if( FAILED(D3DXCreateTextureFromFileInMemory( mDevice, tex->mData, tex->mDataLen, &cache )) )
				{
					printf( "Failed to create Texture Cache :: Invalid file data\n" );
					return 0;
				}
			} else if( texformat > 0 ) {
				if( FAILED(D3DXCreateTexture( mDevice, tex->GetWidth( ), tex->GetHeight( ), 1, D3DUSAGE_DYNAMIC, (D3DFORMAT)texformat, D3DPOOL_DEFAULT, &cache )) )
				{
					printf( "Failed to create Texture Cache :: Couldn't create surface\n" );
					return 0;
				}

				LPDIRECT3DSURFACE9 surf;
				cache->GetSurfaceLevel( 0, &surf );
				D3DLOCKED_RECT rect;
				surf->LockRect( &rect, 0, 0 );
				char* data = (char*)rect.pBits;
				memcpy( data, tex->mData, tex->mDataLen );
				surf->UnlockRect( );

				D3DXSaveTextureToFile( "c:\\texdebug.png", D3DXIFF_PNG, cache, 0 );
			} else {
				printf( "Failed to create Texture Cache :: Invalid format\n" );
				return 0;
			}

			tex->SetDirty( false );
		}
		return cache;
	};

	LPDIRECT3DVERTEXDECLARATION9 DX9RenderSystem::GetVertexDeclCache( VertexDeclaration* decl )
	{
		LPDIRECT3DVERTEXDECLARATION9& cache = (LPDIRECT3DVERTEXDECLARATION9&)decl->mCache;

		if( decl->IsDirty( ) )
		{
			if( mCacheCount >= DX9MAXFRAMECACHE )
				return 0;
			mCacheCount += 1;

			if( cache ) {
				cache->Release( );
				cache = 0;
			}

			// Generate Declaration
			std::vector< D3DVERTEXELEMENT9 > elems;
			int offset = 0;
			for( std::vector< VertexElement >::iterator i = decl->mElements.begin( ); i != decl->mElements.end( ); i++ )
			{
				D3DVERTEXELEMENT9 elem = { (*i).streamidx, offset, (*i).type, D3DDECLMETHOD_DEFAULT, (*i).semantic, (*i).usageidx };
				elems.push_back( elem );
				offset += VDeclType::LENGTH[ (*i).type ];
			}
			D3DVERTEXELEMENT9 lastelem = D3DDECL_END();
			elems.push_back( lastelem );

			if( FAILED(mDevice->CreateVertexDeclaration( &elems[0], &cache )) )
			{
				printf( "Failed to create Vertex Declaration Cache\n" );
				return 0;
			}

			decl->SetDirty( false );
		}

		return cache;
	};

	LPDIRECT3DVERTEXSHADER9 DX9RenderSystem::GetVertexShaderCache( VertexShader* shader )
	{
		LPDIRECT3DVERTEXSHADER9& cache = (LPDIRECT3DVERTEXSHADER9&)shader->mCache;

		if( shader->IsDirty( ) )
		{
			if( mCacheCount >= DX9MAXFRAMECACHE )
				return 0;
			mCacheCount += 1;

			if( cache ) {
				cache->Release( );
				cache = 0;
			}

			if( FAILED(mDevice->CreateVertexShader( (DWORD*)shader->mData, &cache )) )
			{
				printf( "Failed to create Vertex Shader Cache\n" );
				return 0;
			}

			shader->SetDirty( false );
		}

		return cache;
	};

	D3DXMATRIX* DX9RenderSystem::GetDXMatrix( const Matrix4& mat )
	{
		return (D3DXMATRIX*)&mat;
	};

	void DX9RenderSystem::ResetStates( )
	{
		for( int i = 0; i < 256; i++ )
			mRenderStateCached[i] = false;
		for( int i = 8; i < 8; i++ )
			for( int j = 0; j < 36; j++ )
				mTextureStateCached[i][j] = false;
		mDecl = 0;
		for( int i = 0; i < 8; i++ )
			mVBuffer[i] = 0;
		mIBuffer = 0;
		for( int i = 0; i < 8; i++ )
			mTexture[i] = 0;
	};
};