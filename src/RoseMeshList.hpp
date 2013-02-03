#pragma once
#include "Halia.hpp"
#include "Mesh.hpp"
#include "EntityGroup.hpp"
#include "Resource.hpp"
#include "BoundingBox.hpp"
#include "Vector3.hpp"
#include "RoseMeshLoader.hpp"

namespace RoseMLFlag
{
	enum RoseMLFlag
	{
		POSITION = 1,
		ROTATION = 2,
		SCALE = 3,
		AXISROTATION = 4,
		BONEINDEX = 5,
		DUMMYINDEX = 6,
		PARENT = 7,
		COLLISION = 0x1D,
		ZMOPATH = 0x1E,
		RANGEMODE = 0x1F,
		LIGHTMAPMODE = 0x20
	};
};

class RoseObjectEntity : public Halia::EntityGroup
{
public:
	RoseObjectEntity( )
	{
		mBoundingMode = 1;
	};

	~RoseObjectEntity( )
	{
	};

	void AddSubObject( Halia::IndexedMeshMirror* mesh, const Halia::Matrix4& mat, const Halia::BoundingBox& bb )
	{
		Halia::MeshEntity* ent = new Halia::MeshEntity( mesh );
		ent->SetBoundingBox( bb );
		AddChild( ent, mat );
		mMesh.push_back( mesh );
		mBoundingBox.AddTransformedBB( bb, mat );
	};

	void AddLightmap( int meshid, Halia::TextureBase* tex, const Halia::Matrix4& transform )
	{
		Halia::Material* mat = mMesh[meshid]->GetMaterial( );
		mat->AddTextureStage(
			Halia::TextureStage(
				tex,
				Halia::BlendMode( Halia::BlendOp::MODULATE2X, Halia::BlendArg::TEXTURE, Halia::BlendArg::CURRENT ),
				Halia::BlendMode( Halia::BlendOp::SELECTARGUMENT1, Halia::BlendArg::CURRENT ),
				transform
			)
		);
	};

protected:
	std::vector< Halia::IndexedMeshMirror* > mMesh;
};

class RoseMeshList : public Halia::Resource
{
public:
	class SubObject
	{
	public:
		SubObject( )
			: mMeshLoader( 0 ), mMaterial( 0 ), mScale( 1, 1, 1 )
		{
		};

		void SetMeshLoader( RoseMeshLoader* val )
		{
			mMeshLoader = val;
		};

		void SetMaterial( Halia::Material* val )
		{
			mMaterial = val;
		};

		void SetPosition( const Halia::Vector3& val )
		{
			mPosition = val;
		};

		void SetRotation( const Halia::Quarternion& val )
		{
			mRotation = val;
		};

		void SetScale( const Halia::Vector3& val )
		{
			mScale = val;
		};

		Halia::IndexedMeshMirror* CreateCopy( )
		{
			mMeshLoader->Load( );
			Halia::IndexedMeshMirror* mesh = new Halia::IndexedMeshMirror( *mMeshLoader->GetMesh( ) );
			((Halia::TextureFile*)mMaterial->GetTextureStage( 0 ).texture)->Load( );
			mesh->SetMaterial( new Halia::Material( *mMaterial ) );
			return mesh;
		};

		Halia::Matrix4 CreateMatrix( )
		{
			return Halia::Matrix4::CreateRotation( mRotation ) * 
				Halia::Matrix4::CreateScaling( mScale ) * 
				Halia::Matrix4::CreateTranslation( mPosition );
		};

		const Halia::BoundingBox& GetBoundingBox( )
		{
			return mMeshLoader->GetBoundingBox( );
		};

	protected:
		RoseMeshLoader* mMeshLoader;
		Halia::Material* mMaterial;
		Halia::Vector3 mPosition;
		Halia::Quarternion mRotation;
		Halia::Vector3 mScale;
	};

	class Object
	{
	public:
		void AddObj( SubObject* obj )
		{
			mSubObjs.push_back( obj );
		};

		SubObject* GetObj( int id )
		{
			return mSubObjs[id];
		};

		int GetObjCount( )
		{
			return mSubObjs.size( );
		};

	protected:
		std::vector< SubObject* > mSubObjs;
	};

	RoseMeshList( )
		: mPath( "" )
	{
	};

	RoseMeshList( const std::string& path )
		: mPath( path )
	{
	};

	void SetPath( const std::string& path )
	{
		mPath = path;
	};

	bool Load( )
	{
		printf( "Loading '%s'\n", mPath.c_str( ) );

		if( IsLoaded( ) )
			return true;

		printf( "Proc Started on '%s'\n", mPath.c_str( ) );

		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mPath );
		if( !fh )
			return false;

		short meshcount = fh->Read<short>( );
		for( int i = 0; i < meshcount; i++ )
		{
			std::string path = fh->ReadString( Halia::StringType::NULLTERM );
			mMesh.push_back( new RoseMeshLoader( path ) );
		}

		short matcount = fh->Read<short>( );
		for( int i = 0; i < matcount; i++ )
		{
			Halia::Material* mat = new Halia::Material( );

			std::string path = fh->ReadString( Halia::StringType::NULLTERM );
			Halia::TextureFile* tex = new Halia::TextureFile( path );
			
			mat->AddTextureStage(
				Halia::TextureStage(
					tex,
					Halia::BlendMode( Halia::BlendOp::SELECTARGUMENT1, Halia::BlendArg::TEXTURE )
				)
			);

			fh->Read<short>( ); // is_skin
			mat->mAlphaBlend = fh->Read<short>( ) != 0;
			mat->mTwoSided = fh->Read<short>( ) != 0;
			mat->mAlphaTest = fh->Read<short>( ) != 0;
			mat->mAlphaRef = (unsigned char)fh->Read<short>( );
			mat->mDepthWrite = fh->Read<short>( ) != 0;
			mat->mDepthBuffer = fh->Read<short>( ) != 0;
			fh->Read<short>( ); // blending_mode
			mat->mSpecularEnabled = fh->Read<short>( ) != 0;
			fh->Read<float>( ); // alpha
			fh->Read<short>( ); // glow_type
			fh->Read<float>( ); // glowcolor_red
			fh->Read<float>( ); // glowcolor_green
			fh->Read<float>( ); // glowcolor_blue

			mMaterial.push_back( mat );
		}
			
		short eftcount = fh->Read<short>( );
		for( int i = 0; i < eftcount; i++ )
		{
			std::string path = fh->ReadString( Halia::StringType::NULLTERM );
			//mEffect.push_back( new RoseEffectLoader( path ) );
		}

		short objcount = fh->Read<short>( );
		for( int i = 0; i < objcount; i++ )
		{
			//:: More fucked shit!
			fh->Read<float>( ); // boundingsphere_radius
			fh->Read<float>( ); // boundingsphere_x
			fh->Read<float>( ); // boundingsphere_y

			Object* obj = new Object( );
			short subobjcount = fh->Read<short>( );
			if( subobjcount )
			{	
				for( int j = 0; j < subobjcount; j++ )
				{
					short meshid = fh->Read<short>( );
					short matid = fh->Read<short>( );

					SubObject* subobj = new SubObject( );
					subobj->SetMeshLoader( mMesh[ meshid ] );
					subobj->SetMaterial( mMaterial[ matid ] );

					char switch_id = 0;
					while( ( switch_id = fh->Read<char>( ) ) != 0 )
					{
						unsigned char switch_len = fh->Read<unsigned char>( );

						if( switch_id == RoseMLFlag::POSITION ) {
							Halia::Vector3 pos;
							pos.x = fh->Read<float>( );
							pos.y = fh->Read<float>( );
							pos.z = fh->Read<float>( );
							pos /= 100.0f;
							subobj->SetPosition( pos );
						} else 	if( switch_id == RoseMLFlag::ROTATION ) {
							Halia::Quarternion rot;
							rot.w = fh->Read<float>( );
							rot.x = fh->Read<float>( );
							rot.y = fh->Read<float>( );
							rot.z = fh->Read<float>( );
							subobj->SetRotation( rot );
						} else if( switch_id == RoseMLFlag::SCALE ) {
							Halia::Vector3 scale;
							scale.x = fh->Read<float>( );
							scale.y = fh->Read<float>( );
							scale.z = fh->Read<float>( );
							subobj->SetScale( scale );
						} else if( switch_id == RoseMLFlag::AXISROTATION ) {
							fh->Read<float>( ); // x
							fh->Read<float>( ); // y
							fh->Read<float>( ); // z
							fh->Read<float>( ); // w
						} else if( switch_id == RoseMLFlag::BONEINDEX ) {
							fh->Read<short>( ); // bone index
						} else if( switch_id == RoseMLFlag::DUMMYINDEX ) {
							fh->Read<short>( ); // dummy index
						} else if( switch_id == RoseMLFlag::PARENT ) {
							fh->Read<short>( ); // parent
						} else if( switch_id == RoseMLFlag::COLLISION ) {
							fh->Read<short>( ); // collision_type
						} else if( switch_id == RoseMLFlag::ZMOPATH ) {
							fh->ReadString( switch_len ); // zmo path
						} else if( switch_id == RoseMLFlag::RANGEMODE ) {
							fh->Read<short>( ); // range_set
						} else if( switch_id == RoseMLFlag::LIGHTMAPMODE ) {
							fh->Read<short>( ); // use_lightmap
						} else {
							fh->Seek( switch_len, Halia::SeekOrigin::CUR );
						}
					}

					obj->AddObj( subobj );
				}

				short pointcount = fh->Read<short>( );
				for( int j = 0; j < pointcount; j++ )
				{
					short effectid = fh->Read<short>( );
					short effecttype = fh->Read<short>( );

					char switch_id = 0;
					while( ( switch_id = fh->Read<char>( ) ) != 0 )
					{
						unsigned char switch_len = fh->Read<unsigned char>( );

						if( 0 ) {
						} else {
							fh->Seek( switch_len, Halia::SeekOrigin::CUR );
						}
					}
				}

				//:: RATHER SURE THESE ARE FUCKEDDD
				fh->Read<Halia::BoundingBox>( ); // bounding_box
			}

			mObject.push_back( obj );	
		}

		Halia::FSMInstance->CloseFile( fh );

		printf( "Done Loading '%s'\n", mPath.c_str( ) );

		mLoaded = true;
		return true;
	};

	void Unload( )
	{
		
	};

	RoseObjectEntity* CreateNode( int objid )
	{
		Object* obj = mObject[ objid ];
		if( obj == 0 )
			return 0;

		RoseObjectEntity* ent = new RoseObjectEntity( );
		for( int i = 0; i < obj->GetObjCount( ); ++i )
		{
			SubObject* subobj = obj->GetObj( i );
			ent->AddSubObject( subobj->CreateCopy( ), subobj->CreateMatrix( ), subobj->GetBoundingBox( ) );
		}

		return ent;
	};

protected:
	std::string mPath;
	
	std::vector< RoseMeshLoader* > mMesh;
	std::vector< Halia::Material* > mMaterial;
	//std::vector< RoseEffectLoader* > mEffect;
	std::vector< Object* > mObject;
};