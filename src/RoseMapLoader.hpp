#pragma once
#include "RenderSystem.hpp"
#include "Frustum.hpp"
#include "Matrix4.hpp"
#include "Entity.hpp"

class RoseMapPatchEntity : public Halia::Entity
{
	public:
		RoseMapPatchEntity( Halia::VertexDeclaration* decl, 
			Halia::VertexBufferBase* vb, Halia::IndexBufferBase* ib )
			: mDecl( decl ), mVb( vb ), mIb( ib )
		{
			for( int i = 0; i < 16*16; i++ )
				mPatchCulled[i] = false;
		};

		~RoseMapPatchEntity( )
		{
		};

		virtual void Update( const Halia::Matrix4& parentmat )
		{
			mTransform = parentmat;
		};

		virtual void UpdateCulling( const Halia::Frustum& camf, const Halia::Matrix4& mat )
		{
			mCulled = CheckCulling( camf, mTransform );

			for( int i = 0; i < 16*16; i++ )
			{
				mPatchCulled[i] = mPatchBb[i].CheckOutsideFrustum( camf, mTransform );
			}
		};

		virtual void QueueRender( Halia::RenderQueue* rq )
		{
			if( !mEnabled || mCulled )
				return;

			rq->PushEntity( this, false );
		};

		virtual void Render( Halia::RenderSystem* rs )
		{
			int retval = 0;

			rs->SetWorldMatrix( mTransform );
			rs->SetVertexDeclaration( mDecl );
			rs->SetStreamSource( 0, mVb, 0 );
			rs->SetIndexSource( mIb );

			for( int i = 0; i < 16 * 16; i++ )
			{
				if( mPatchCulled[i] )
					continue;

				retval = rs->SetMaterial( mMats[i] );
				rs->DrawIndexedPrimitive( Halia::PrimitiveType::TriangleStrip, 5*5*i, 0, 5*5, 44 );
			}
		};

		void SetMaterial( int i, Halia::Material* mat )
		{
			mMats[i] = mat;
		};

		void SetPatchBoundingBox( int i, Halia::BoundingBox& box )
		{
			mPatchBb[i] = box;
		};

	private:
		Halia::VertexDeclaration* mDecl;
		Halia::VertexBufferBase* mVb;
		Halia::IndexBufferBase* mIb;
		Halia::Material* mMats[16*16];
		Halia::BoundingBox mPatchBb[ 16*16 ];
		bool mPatchCulled[16*16];
};

class RoseMapLoader : public Halia::Resource
{
public:
	struct RoseMapVert
	{
		Halia::Vector3 pos;
		Halia::Vector2 uv1;
		Halia::Vector2 uv2;
		Halia::Vector2 uv3;
	};

	struct Tile
	{
		int tile1;
		int tile2;
		int rot;
	};

	struct ObjectLightmapInfo
	{
		int ddsid;
		int pixelperobj;
		int objsperwidth;
		int ddspos;
	};

	struct Object
	{
		std::string name;
		short warpid;
		short eventid;
		int objtype;
		int objid;
		int mapxpos;
		int mapypos;
		Halia::Vector3 position;
		Halia::Quarternion rotation;
		Halia::Vector3 scale;

		int lmcount;
		ObjectLightmapInfo* lm;
	};

	struct Patch
	{
		float heights[65][65];
		int tiles[16][16];
		int cnstcount;
		Object* cnsts;
		int decocount;
		Object* decos;
		int lightmapcount;
		Halia::TextureFile** lightmaps;
		int dlightmapcount;
		Halia::TextureFile** dlightmaps;
		Halia::TextureFile* lightmap;
		Halia::VertexBuffer<RoseMapVert>* vbuffer;
		Halia::Vector3 boundmin, boundmax;
	};

	RoseMapLoader( const std::string& basepath, const std::string& mapname, RoseMeshList* cnst, RoseMeshList* deco, int pstartx, int pstarty )
		: mBasePath( basepath ), mMapName( mapname ), mCnstList( cnst ), 
			mDecoList( deco ), mPStartX( pstartx ), mPStartY( pstarty ), 
			mTileTex( 0 ), mTile( 0 )
	{
		for( int ix = 0; ix < 64; ix++ )
			for( int iy = 0; iy < 64; iy++ )
				mPatches[ix][iy] = 0;
	};

	bool Load( )
	{
		bool retval = true;

		//:: Get map bounds
		for( int i = mPStartX; i < 64; i++ )
		{
			if( !Halia::FSMInstance->FileExists( mBasePath + mMapName + "\\" + GetPatchName( i, mPStartY ) + ".IFO" ) ) {
				mPSizeX = i - mPStartX;
				break;
			}
		}
		for( int i = mPStartY; i < 64; i++ )
		{
			if( !Halia::FSMInstance->FileExists( mBasePath + mMapName + "\\" + GetPatchName( mPStartX, i ) + ".IFO" ) ) {
				mPSizeY = i - mPStartY;
				break;
			}
		}

		//:: Load map info		
		retval &= LoadZon( );
		
		//:: Create global declaration
		mDecl = new Halia::VertexDeclaration( );
		mDecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::POSITION, Halia::VDeclType::FLOAT3, 0 ) );
		mDecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::TEXCOORD, Halia::VDeclType::FLOAT2, 0 ) );
		mDecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::TEXCOORD, Halia::VDeclType::FLOAT2, 1 ) );
		mDecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::TEXCOORD, Halia::VDeclType::FLOAT2, 2 ) );

		//:: Create global index buffer
		short predefindices[] = { 
			 5,  0,  6,  1,  7,  2,  8,  3, 9 ,  4, 
			 4, 10, 10,  5, 11,  6, 12,  7, 13,  8,
			14,  9,  9, 15, 15, 10, 16, 11, 17, 12,
			18, 13, 19, 14, 14, 20, 20, 15, 21, 16,
			22, 17, 23, 18, 24, 19
		};

		mIBuffer = new Halia::IndexBuffer<short>( );
		mIBuffer->Resize( 46 );
		memcpy( mIBuffer->GetBuffer( ), predefindices, mIBuffer->GetBufferSize( ) );

		//:: Load map patches
		for( int ix = mPStartX; ix < mPStartX + mPSizeX; ix++ )
		{
			for( int iy = mPStartY; iy < mPStartY + mPSizeY; iy++ )
			{
				retval &= LoadPatch( ix, iy );
			}
		}

		printf( "Completed loading map '%s'\n", mMapName.c_str( ) );

		mLoaded = true;
		return retval;
	};

	std::string GetPatchName( int x, int y )
	{
		char tmp[ 10 ];
		sprintf( tmp, "%d_%d", x, y );
		return std::string( tmp );
	};

	bool LoadZon( )
	{
		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mBasePath + mMapName + "\\" + mMapName + ".ZON" );
		if( !fh )
			return false;

		int blockcount = fh->Read<int>( );
		for( int i = 0; i < blockcount; i++ )
		{
			int blockid = fh->Read<int>( );
			int blockoffset = fh->Read<int>( );
			int lastoffset = fh->Tell( );
			fh->Seek( blockoffset, Halia::SeekOrigin::SET );
			ReadZonChunk( fh, blockid );
			fh->Seek( lastoffset, Halia::SeekOrigin::SET );
		}

		Halia::FSMInstance->CloseFile( fh );
		return true;
	};

	void ReadZonChunk( Halia::FileSystem::FileStream* fh, int blockid )
	{
		if( blockid == 2 ) {
			mTileTexCount = fh->Read<int>( );
			mTileTex = new Halia::TextureFile*[ mTileTexCount ];
			for( int i = 0; i < mTileTexCount; i++ )
			{
				std::string texpath = fh->ReadString( Halia::StringType::PRE7 );
				if( texpath == "end" )
					break;
				mTileTex[i] = new Halia::TextureFile( texpath );
				mTileTex[i]->Load( );
			}
		} else if( blockid == 3 ) {
			mTileCount = fh->Read<int>( );
			mTile = new Tile[ mTileCount ];
			for( int i = 0; i < mTileCount; i++ )
			{
				mTile[i].tile1 = fh->Read<int>( );
				mTile[i].tile2 = fh->Read<int>( );
				mTile[i].tile1 += fh->Read<int>( );
				mTile[i].tile2 += fh->Read<int>( );
				fh->Read<int>( );
				mTile[i].rot = fh->Read<int>( );
				fh->Read<int>( );
			}
		}
	};

	bool LoadPatch( int x, int y )
	{
		bool retval = true;
		mPatches[x][y] = new Patch( );
		retval &= LoadHim( x, y );
		retval &= LoadTil( x, y );
		retval &= LoadIfo( x, y );
		retval &= LoadLitBuilding( x, y );
		retval &= LoadLitObject( x, y );
		retval &= GeneratePatch( x, y );
		return retval;
	};

	bool LoadLitBuilding( int x, int y )
	{
		std::string lmpath = mBasePath + mMapName + "\\" + GetPatchName(x,y) + "\\LIGHTMAP\\";
		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( lmpath + "BUILDINGLIGHTMAPDATA.LIT" );
		if( !fh )
			return false;

		int objcount = fh->Read<int>( );
		for( int i = 0; i < objcount; i++ )
		{
			int partcount = fh->Read<int>( );
			int objid = fh->Read<int>( ) - 1;

			mPatches[x][y]->cnsts[objid].lmcount = partcount;
			mPatches[x][y]->cnsts[objid].lm = new ObjectLightmapInfo[ partcount ];
			for( int j = 0; j < partcount; j++ )
			{
				fh->ReadString( Halia::StringType::PRE7 ); // original name
				int partid = fh->Read<int>( );
				fh->ReadString( Halia::StringType::PRE7 ); // dds name

				mPatches[x][y]->cnsts[objid].lm[partid].ddsid = fh->Read<int>( );
				mPatches[x][y]->cnsts[objid].lm[partid].pixelperobj = fh->Read<int>( );
				mPatches[x][y]->cnsts[objid].lm[partid].objsperwidth = fh->Read<int>( );
				mPatches[x][y]->cnsts[objid].lm[partid].ddspos = fh->Read<int>( );
			}
		}

		mPatches[x][y]->lightmapcount = fh->Read<int>( );
		mPatches[x][y]->lightmaps = new Halia::TextureFile*[ mPatches[x][y]->lightmapcount ];
		for( int i = 0; i < mPatches[x][y]->lightmapcount; i++ )
		{
			std::string path = fh->ReadString( Halia::StringType::PRE7 );
			mPatches[x][y]->lightmaps[i] = new Halia::TextureFile( lmpath + path );
			mPatches[x][y]->lightmaps[i]->Load( );
		}

		Halia::FSMInstance->CloseFile( fh );
		return true;
	}

	bool LoadLitObject( int x, int y )
	{
		std::string lmpath = mBasePath + mMapName + "\\" + GetPatchName(x,y) + "\\LIGHTMAP\\";
		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( lmpath + "OBJECTLIGHTMAPDATA.LIT" );
		if( !fh )
			return false;

		int objcount = fh->Read<int>( );
		for( int i = 0; i < objcount; i++ )
		{
			int partcount = fh->Read<int>( );
			int objid = fh->Read<int>( ) - 1;

			mPatches[x][y]->decos[objid].lmcount = partcount;
			mPatches[x][y]->decos[objid].lm = new ObjectLightmapInfo[ partcount ];
			for( int j = 0; j < partcount; j++ )
			{
				fh->ReadString( Halia::StringType::PRE7 ); // original name
				int partid = fh->Read<int>( );
				fh->ReadString( Halia::StringType::PRE7 ); // dds name

				mPatches[x][y]->decos[objid].lm[partid].ddsid = fh->Read<int>( );
				mPatches[x][y]->decos[objid].lm[partid].pixelperobj = fh->Read<int>( );
				mPatches[x][y]->decos[objid].lm[partid].objsperwidth = fh->Read<int>( );
				mPatches[x][y]->decos[objid].lm[partid].ddspos = fh->Read<int>( );
			}
		}

		mPatches[x][y]->dlightmapcount = fh->Read<int>( );
		mPatches[x][y]->dlightmaps = new Halia::TextureFile*[ mPatches[x][y]->dlightmapcount ];
		for( int i = 0; i < mPatches[x][y]->dlightmapcount; i++ )
		{
			std::string path = fh->ReadString( Halia::StringType::PRE7 );
			mPatches[x][y]->dlightmaps[i] = new Halia::TextureFile( lmpath + path );
			mPatches[x][y]->dlightmaps[i]->Load( );
		}

		Halia::FSMInstance->CloseFile( fh );
		return true;
	}

	bool LoadHim( int x, int y )
	{
		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mBasePath + mMapName + "\\" + GetPatchName(x,y) + ".HIM" );
		if( !fh )
			return false;

		int width = fh->Read<int>( );
		int height = fh->Read<int>( );
		int gridcount = fh->Read<int>( );
		float gridsize = fh->Read<float>( );

		for( int iy = 0; iy <= 64; iy++ )
		{
			for( int ix = 0; ix <= 64; ix++ )
			{
				mPatches[x][y]->heights[ix][iy] = fh->Read<float>( );
			}
		}

		Halia::FSMInstance->CloseFile( fh );
		return true;
	}

	bool LoadTil( int x, int y )
	{
		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mBasePath + mMapName + "\\" + GetPatchName(x,y) + ".TIL" );
		if( !fh )
			return false;

		int width = fh->Read<int>( );
		int height = fh->Read<int>( );

		for( int iy = 0; iy < 16; iy++ )
		{
			for( int ix = 0; ix < 16; ix++ )
			{
				fh->Read<char>( ); // brush number
				fh->Read<char>( ); // tile index
				fh->Read<char>( ); // tile set
				mPatches[x][y]->tiles[ix][iy] = fh->Read<int>( );
			}
		}

		Halia::FSMInstance->CloseFile( fh );
		return true;
	}

	void ReadIfoObject( Halia::FileSystem::FileStream* fh, Object& obj )
	{
		obj.name = fh->ReadString( Halia::StringType::PRE7 );
		obj.warpid = fh->Read<short>( );
		obj.eventid = fh->Read<short>( );
		obj.objtype = fh->Read<int>( );
		obj.objid = fh->Read<int>( );
		obj.mapxpos = fh->Read<int>( );
		obj.mapypos = fh->Read<int>( );
		obj.rotation.x = fh->Read<float>( );
		obj.rotation.y = fh->Read<float>( );
		obj.rotation.z = fh->Read<float>( );
		obj.rotation.w = fh->Read<float>( );
		obj.position.x = fh->Read<float>( ) + 520000.0f;
		obj.position.y = fh->Read<float>( ) + 520000.0f;
		obj.position.z = fh->Read<float>( );
		obj.scale.x = fh->Read<float>( );
		obj.scale.y = fh->Read<float>( );
		obj.scale.z = fh->Read<float>( );
	};

	void ReadIfoChunk( Halia::FileSystem::FileStream* fh, int blockid, int x, int y )
	{
		if( blockid == 1 ) {
			// Deco
			mPatches[x][y]->decocount = fh->Read<int>( );
			mPatches[x][y]->decos = new Object[ mPatches[x][y]->decocount ];
			for( int i = 0; i < mPatches[x][y]->decocount; i++ )
			{
				ReadIfoObject( fh, mPatches[x][y]->decos[i] );
			}
		} else if( blockid == 3 ) {
			// Cnst
			mPatches[x][y]->cnstcount = fh->Read<int>( );
			mPatches[x][y]->cnsts = new Object[ mPatches[x][y]->cnstcount ];
			for( int i = 0; i < mPatches[x][y]->cnstcount; i++ )
			{
				ReadIfoObject( fh, mPatches[x][y]->cnsts[i] );
			}
		} else if( blockid == 9 ) {
			// Water Planes
		}
	};

	bool LoadIfo( int x, int y )
	{
		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mBasePath + mMapName + "\\" + GetPatchName(x,y) + ".IFO" );
		if( !fh )
			return false;

		int blockcount = fh->Read<int>( );
		for( int i = 0; i < blockcount; i++ )
		{
			int blockid = fh->Read<int>( );
			int blockoffset = fh->Read<int>( );
			int lastoffset = fh->Tell( );
			fh->Seek( blockoffset, Halia::SeekOrigin::SET );
			ReadIfoChunk( fh, blockid, x, y );
			fh->Seek( lastoffset, Halia::SeekOrigin::SET );
		}

		Halia::FSMInstance->CloseFile( fh );
		return true;
	};

	bool GeneratePatch( int x, int y )
	{
		printf( "Generating Patch %d,%d\n", x, y );

		std::string patchname = GetPatchName(x,y);
		mPatches[x][y]->lightmap = new Halia::TextureFile( mBasePath + mMapName + "\\" + patchname + "\\" + patchname + "_PLANELIGHTINGMAP.DDS" );
		mPatches[x][y]->lightmap->Load( );

		mPatches[x][y]->vbuffer = new Halia::VertexBuffer<RoseMapVert>( );

		float minheight = mPatches[x][y]->heights[0][0] / 100.0f;
		float maxheight = mPatches[x][y]->heights[0][0] / 100.0f;

		for( int ix = 0; ix < 16; ix++ )
		{
			for( int iy = 0; iy < 16; iy++ )
			{
				for( int jy = 0; jy < 5; jy++ )
				{
					for( int jx = 0; jx < 5; jx++ )
					{
						int dx4 = ix * 4 + jx;
						int dy4 = iy * 4 + jy;
						int dx5 = ix * 5 + jx;
						int dy5 = iy * 5 + jy;

						RoseMapVert vert;

						vert.pos.x = dx4 * 2.5f;
						vert.pos.y = -( dy4 * 2.5f );
						vert.pos.z = mPatches[x][y]->heights[dx4][dy4] / 100.0f;

						if( vert.pos.z > maxheight )
							maxheight = vert.pos.z;
						if( vert.pos.z < minheight )
							minheight = vert.pos.z;

						vert.uv1.x = jx / 4.0f;
						vert.uv1.y = jy / 4.0f;
						vert.uv2.x = jx / 4.0f;
						vert.uv2.y = jy / 4.0f;
						vert.uv3.x = dx4 / 64.0f;
						vert.uv3.y = dy4 / 64.0f;

						int tileid = mPatches[x][y]->tiles[ix][iy];
						if( mTile[tileid].rot == 2 ) {
							vert.uv2.x = 1.0f - vert.uv2.x;
						} else if( mTile[tileid].rot == 3 ) {
							vert.uv2.y = 1.0f - vert.uv2.y;
						} else if( mTile[tileid].rot == 4 ) {
							vert.uv2.x = 1.0f - vert.uv2.x;
							vert.uv2.y = 1.0f - vert.uv2.y;
						}

						mPatches[x][y]->vbuffer->AddVertex( vert );
					}
				}
			}
		}

		mPatches[x][y]->boundmin = Halia::Vector3( 0.0f, -160.0f, minheight );
		mPatches[x][y]->boundmax = Halia::Vector3( 160.0f, 0.0f, maxheight );

		return true;
	};

	void Unload( )
	{
	};

	Halia::SceneNode* CreateNode( )
	{
		char namegen[ 200 ];
		sprintf( namegen, "Map_Root" );
		Halia::SceneNode* root = new Halia::SceneNode( namegen );

		//for( int ix = 33-1; ix <= 33+1; ix++ )
		for( int ix = mPStartX; ix < mPStartX + mPSizeX; ix++ )
		{
			//for( int iy = 33-1; iy <= 33+1; iy++ )
			for( int iy = mPStartY; iy < mPStartY + mPSizeY; iy++ )
			{
				sprintf( namegen, "Patch_%d_%d_Root", ix, iy );
				Halia::SceneNode* patch = new Halia::SceneNode( namegen );
				RoseMapPatchEntity* ent = new RoseMapPatchEntity( mDecl, mPatches[ix][iy]->vbuffer, mIBuffer );
				printf( "WOOO: %08x\n", mPatches[ix][iy]->vbuffer );
				for( int ix2 = 0; ix2 < 16; ix2++ )
				{
					for( int iy2 = 0; iy2 < 16; iy2++ )
					{
						int tileid = mPatches[ix][iy]->tiles[ix2][iy2];
						Halia::TextureBase* tex1 = mTileTex[ mTile[tileid].tile1 ];
						Halia::TextureBase* tex2 = mTileTex[ mTile[tileid].tile2 ];

						Halia::Material* mat = new Halia::Material( );
						mat->mAlphaBlend = true;
						mat->mAlphaTest = false;
						mat->AddTextureStage(
							Halia::TextureStage(
								tex1,
								Halia::BlendMode( Halia::BlendOp::MODULATE, Halia::BlendArg::TEXTURE, Halia::BlendArg::DIFFUSE ),
								Halia::BlendMode( Halia::BlendOp::SELECTARGUMENT1, Halia::BlendArg::TEXTURE )
							)
						);
						mat->AddTextureStage(
							Halia::TextureStage(
								tex2,
								Halia::BlendMode( Halia::BlendOp::BLENDTEXTUREALPHA, Halia::BlendArg::TEXTURE, Halia::BlendArg::CURRENT ),
								Halia::BlendMode( Halia::BlendOp::SELECTARGUMENT1, Halia::BlendArg::TEXTURE )
							)
						);
						mat->AddTextureStage(
							Halia::TextureStage(
								mPatches[ix][iy]->lightmap,
								Halia::BlendMode( Halia::BlendOp::MODULATE2X, Halia::BlendArg::TEXTURE, Halia::BlendArg::CURRENT ),
								Halia::BlendMode( Halia::BlendOp::SELECTARGUMENT1, Halia::BlendArg::DIFFUSE )
							)
						);
						ent->SetMaterial( ix2 * 16 + iy2, mat );
						ent->SetPatchBoundingBox( ix2 * 16 + iy2, 
							Halia::BoundingBox( 
								Halia::Vector3( ix2*10.0f + 00.0f, iy2*10.0f + 00.0f, mPatches[ix][iy]->boundmin.z ),
								Halia::Vector3( ix2*10.0f + 10.0f, -(iy2*10.0f + 10.0f), mPatches[ix][iy]->boundmax.z )
							)
						);
					}
				}
				patch->AddEntity( ent );
				float x = ix * 160.0f;
				float y = ( 65 - iy ) * 160.0f;
				printf( "Patch is @ %f,%f\n", x, y );
				patch->SetTransform( Halia::Matrix4::CreateTranslation( Halia::Vector3( x, y, 0.0f ) ) );
				root->AddChild( patch );

				// BUILDING
				sprintf( namegen, "Patch_%d_%d_Objects", ix, iy );
				Halia::SceneNode* objs = new Halia::SceneNode( namegen );
				for( int i = 0; i < mPatches[ix][iy]->cnstcount; i++ )
				{
					sprintf( namegen, "Patch_%d_%d_Cnst_%d", ix, iy, i );
					Halia::SceneNode* obj = new Halia::SceneNode( namegen );
					RoseObjectEntity* ent = mCnstList->CreateNode( mPatches[ix][iy]->cnsts[i].objid );
					for( int j = 0; j < mPatches[ix][iy]->cnsts[i].lmcount; j++ )
					{
						int ddsid = mPatches[ix][iy]->cnsts[i].lm[j].ddsid;
						Halia::Matrix4 tm = Halia::Matrix4::IDENTITY;
						int opw = mPatches[ix][iy]->cnsts[i].lm[j].objsperwidth;
						float scale = 1.0f / (float)opw;

						int ox = mPatches[ix][iy]->cnsts[i].lm[j].ddspos % mPatches[ix][iy]->cnsts[i].lm[j].objsperwidth;
						int oy = mPatches[ix][iy]->cnsts[i].lm[j].ddspos / mPatches[ix][iy]->cnsts[i].lm[j].objsperwidth;
						float ofx = scale * ox;
						float ofy = scale * oy;
						Halia::Matrix4 t = Halia::Matrix4::IDENTITY;
						t *= Halia::Matrix4::CreateScaling( Halia::Vector3( scale, scale, 1.0f ) );
						t *= Halia::Matrix4::CreateTranslation2d( Halia::Vector3( ofx, ofy, 1.0f ) );
						ent->AddLightmap( j, mPatches[ix][iy]->lightmaps[ddsid], t );
					}
					obj->AddEntity( ent );
					obj->SetTransform(
						Halia::Matrix4::CreateRotation( mPatches[ix][iy]->cnsts[i].rotation ) *
						Halia::Matrix4::CreateScaling( mPatches[ix][iy]->cnsts[i].scale ) *
						Halia::Matrix4::CreateTranslation( mPatches[ix][iy]->cnsts[i].position / 100.0f )
					);
					obj->SetBoundingBox( ent->GetBoundingBox( ) );
					objs->AddChild( obj );
				}

				// OBJECT
				for( int i = 0; i < mPatches[ix][iy]->decocount; i++ )
				{
					sprintf( namegen, "Patch_%d_%d_Deco_%d", ix, iy, i );
					Halia::SceneNode* obj = new Halia::SceneNode( namegen );
					RoseObjectEntity* ent = mDecoList->CreateNode( mPatches[ix][iy]->decos[i].objid );
					for( int j = 0; j < mPatches[ix][iy]->decos[i].lmcount; j++ )
					{
						int ddsid = mPatches[ix][iy]->decos[i].lm[j].ddsid;
						Halia::Matrix4 tm = Halia::Matrix4::IDENTITY;
						int opw = mPatches[ix][iy]->decos[i].lm[j].objsperwidth;
						float scale = 1.0f / (float)opw;

						int ox = mPatches[ix][iy]->decos[i].lm[j].ddspos % mPatches[ix][iy]->decos[i].lm[j].objsperwidth;
						int oy = mPatches[ix][iy]->decos[i].lm[j].ddspos / mPatches[ix][iy]->decos[i].lm[j].objsperwidth;
						float ofx = scale * ox;
						float ofy = scale * oy;
						Halia::Matrix4 t = Halia::Matrix4::IDENTITY;
						t *= Halia::Matrix4::CreateScaling( Halia::Vector3( scale, scale, 1.0f ) );
						t *= Halia::Matrix4::CreateTranslation2d( Halia::Vector3( ofx, ofy, 1.0f ) );
						ent->AddLightmap( j, mPatches[ix][iy]->dlightmaps[ddsid], t );
					}
					obj->AddEntity( ent );
					obj->SetTransform(
						Halia::Matrix4::CreateRotation( mPatches[ix][iy]->decos[i].rotation ) *
						Halia::Matrix4::CreateScaling( mPatches[ix][iy]->decos[i].scale ) *
						Halia::Matrix4::CreateTranslation( mPatches[ix][iy]->decos[i].position / 100.0f )
					);
					obj->SetBoundingBox( ent->GetBoundingBox( ) );
					objs->AddChild( obj );
				}
				objs->GenBoundingBox( );
				root->AddChild( objs );
			}
		}
		//root->CalculateBounding( );
		return root;
	};

private:
	std::string mBasePath;
	std::string mMapName;
	RoseMeshList* mCnstList;
	RoseMeshList* mDecoList;
	int mPStartX, mPStartY;
	int mPSizeX, mPSizeY;
	int mTileTexCount;
	Halia::TextureFile** mTileTex;
	int mTileCount;
	Tile* mTile;
	Patch* mPatches[64][64];
	Halia::VertexDeclaration* mDecl;
	Halia::IndexBuffer<short>* mIBuffer;
	//Halia::RenderSystem* mRs;
};