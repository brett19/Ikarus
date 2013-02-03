#pragma once
#include <map>
#include "Halia.hpp"
#include "Resource.hpp"
#include "Mesh.hpp"

namespace RoseMeshFormat
{
	enum RoseMeshFormat
	{
		POSITION = 2,
		NORMAL = 4,
		COLOR = 8,
		BONEW = 16,
		BONEI = 32,
		TANGENT = 64,
		UV1 = 128,
		UV2 = 256,
		UV3 = 512,
		UV4 = 1024
	};
};

class RoseMeshLoader : public Halia::Resource
{
public:
	RoseMeshLoader( )
		: mPath( "" ), mMesh( 0 )
	{
	};

	RoseMeshLoader( const std::string& path )
		: mPath( path ), mMesh( 0 )
	{
	};

	~RoseMeshLoader( )
	{
	};

	void SetPath( const std::string& path )
	{
		mPath = path;
	};

	bool Load( )
	{
		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mPath );
		if( !fh )
			return false;

		mMesh = new Halia::IndexedMeshMirror( );
		fh->Seek( 8, Halia::SeekOrigin::SET );
		int vertexformat = fh->Read<int>( );
		Halia::VertexDeclaration* vdecl = GetVertexDeclaration( vertexformat );
		mMesh->SetVertexDeclaration( vdecl );

		mBoundingBox = fh->Read<Halia::BoundingBox>( );

		short bonecount = fh->Read<short>( );
		short* bones = new short[ bonecount ];
		for( short i = 0; i < bonecount; i++ )
		{
			bones[ i ] = fh->Read<short>( );
		}

		short vertexcount = fh->Read<short>( );
		mMesh->InitVertexBuffer( vertexcount );
		char* vbuffer = mMesh->GetVertexBuffer( )->GetBuffer( );
		int vstride = vdecl->GetStride( 0 );

		if( vertexformat & RoseMeshFormat::POSITION )
		{
			int elemoff = vdecl->GetElementOffset( Halia::VDeclSemantic::POSITION, 0 );
			for( int i = 0; i < vertexcount; i++ )
			{
				*((Halia::Vector3*)&vbuffer[vstride*i+elemoff]) = fh->Read<Halia::Vector3>( );
			}
		}

		if( vertexformat & RoseMeshFormat::NORMAL )
		{
			int elemoff = vdecl->GetElementOffset( Halia::VDeclSemantic::NORMAL, 0 );
			for( int i = 0; i < vertexcount; i++ )
			{
				*((Halia::Vector3*)&vbuffer[vstride*i+elemoff]) = fh->Read<Halia::Vector3>( );
			}
		}

		if( vertexformat & RoseMeshFormat::COLOR )
		{
			int elemoff = vdecl->GetElementOffset( Halia::VDeclSemantic::COLOUR, 0 );
			for( int i = 0; i < vertexcount; i++ )
			{
				*((unsigned int*)&vbuffer[vstride*i+elemoff]) = fh->Read<unsigned int>( );
			}
		}

		if( vertexformat & RoseMeshFormat::BONEW && vertexformat & RoseMeshFormat::BONEI )
		{
			int elemoffw = vdecl->GetElementOffset( Halia::VDeclSemantic::BLENDWEIGHT, 0 );
			int elemoffi = vdecl->GetElementOffset( Halia::VDeclSemantic::BLENDINDEX, 0 );
			for( int i = 0; i < vertexcount; i++ )
			{
				*((Halia::Vector4*)&vbuffer[vstride*i+elemoffw]) = fh->Read<Halia::Vector4>( );
				*((unsigned char*)&vbuffer[vstride*i+elemoffi+0]) = (unsigned char)bones[ fh->Read<unsigned short>( ) ];
				*((unsigned char*)&vbuffer[vstride*i+elemoffi+1]) = (unsigned char)bones[ fh->Read<unsigned short>( ) ];
				*((unsigned char*)&vbuffer[vstride*i+elemoffi+2]) = (unsigned char)bones[ fh->Read<unsigned short>( ) ];
				*((unsigned char*)&vbuffer[vstride*i+elemoffi+3]) = (unsigned char)bones[ fh->Read<unsigned short>( ) ];
			}
		}

		if( vertexformat & RoseMeshFormat::TANGENT )
		{
			int elemoff = vdecl->GetElementOffset( Halia::VDeclSemantic::TANGENT, 0 );
			for( int i = 0; i < vertexcount; i++ )
			{
				*((Halia::Vector3*)&vbuffer[vstride*i+elemoff]) = fh->Read<Halia::Vector3>( );
			}
		}

		if( vertexformat & RoseMeshFormat::UV1 )
		{
			int elemoff = vdecl->GetElementOffset( Halia::VDeclSemantic::TEXCOORD, 0 );
			for( int i = 0; i < vertexcount; i++ )
			{
				*((Halia::Vector2*)&vbuffer[vstride*i+elemoff]) = fh->Read<Halia::Vector2>( );
			}
		}

		if( vertexformat & RoseMeshFormat::UV2 )
		{
			int elemoff = vdecl->GetElementOffset( Halia::VDeclSemantic::TEXCOORD, 1 );
			for( int i = 0; i < vertexcount; i++ )
			{
				*((Halia::Vector2*)&vbuffer[vstride*i+elemoff]) = fh->Read<Halia::Vector2>( );
			}
		}

		if( vertexformat & RoseMeshFormat::UV3 )
		{
			int elemoff = vdecl->GetElementOffset( Halia::VDeclSemantic::TEXCOORD, 2 );
			for( int i = 0; i < vertexcount; i++ )
			{
				*((Halia::Vector2*)&vbuffer[vstride*i+elemoff]) = fh->Read<Halia::Vector2>( );
			}
		}

		if( vertexformat & RoseMeshFormat::UV4 )
		{
			int elemoff = vdecl->GetElementOffset( Halia::VDeclSemantic::TEXCOORD, 3 );
			for( int i = 0; i < vertexcount; i++ )
			{
				*((Halia::Vector2*)&vbuffer[vstride*i+elemoff]) = fh->Read<Halia::Vector2>( );
			}
		}

		//:: Load indices
		short facecount = fh->Read<short>( );
		mMesh->InitIndexBuffer( facecount * 3 );
		mMesh->SetPrimitiveCount( facecount );
		unsigned short* ibuffer = (unsigned short*)mMesh->GetIndexBuffer( )->GetBuffer( );

		for( int i = 0; i < facecount * 3; i++ )
		{
			ibuffer[i] = fh->Read<short>( );
		}

		//:: Load material links
		short matcount = fh->Read<short>( );
		for( int i = 0; i < matcount; i++ )
		{
			fh->Read<short>( ); // numfaces
		}

		//:: Load strip indices
		short stripcount = fh->Read<short>( );
		if( stripcount > 0 )
		{
			mMesh->SetPrimitiveType( Halia::PrimitiveType::TriangleStrip );
			mMesh->InitIndexBuffer( stripcount );
			mMesh->SetPrimitiveCount( stripcount - 2 );
			unsigned short* sibuffer = (unsigned short*)mMesh->GetIndexBuffer( )->GetBuffer( );

			for( int i = 0; i < stripcount; i++ )
			{
				sibuffer[i] = fh->Read<short>( );
			};
		}

		//:: Pool type
		fh->Read<short>( ); // static = 0, dynamic = 1, system = 2

		Halia::FSMInstance->CloseFile( fh );
		mLoaded = true;
		return true;
	};

	void Unload( )
	{
		if( mMesh )
		{
			delete mMesh;
			mMesh = 0;
		}
	};

	Halia::IndexedMeshMirror* GetMesh( )
	{
		return mMesh;
	};

	const Halia::BoundingBox& GetBoundingBox( )
	{
		return mBoundingBox;
	};

	static Halia::VertexDeclaration* GetVertexDeclaration( int vertexformat )
	{
		std::map<int,Halia::VertexDeclaration*>::iterator i = mVDecls.find( vertexformat );
		if( i != mVDecls.end( ) )
			return i->second;

		Halia::VertexDeclaration* vdecl = new Halia::VertexDeclaration( );
		if( vertexformat & RoseMeshFormat::POSITION )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::POSITION, Halia::VDeclType::FLOAT3, 0 ) );
		if( vertexformat & RoseMeshFormat::NORMAL )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::NORMAL, Halia::VDeclType::FLOAT3, 0 ) );
		if( vertexformat & RoseMeshFormat::COLOR )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::COLOUR, Halia::VDeclType::UBYTE4, 0 ) );
		if( vertexformat & RoseMeshFormat::BONEW )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::BLENDWEIGHT, Halia::VDeclType::FLOAT4, 0 ) );
		if( vertexformat & RoseMeshFormat::BONEI )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::BLENDINDEX, Halia::VDeclType::UBYTE4, 0 ) );
		if( vertexformat & RoseMeshFormat::TANGENT )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::TANGENT, Halia::VDeclType::FLOAT3, 0 ) );
		if( vertexformat & RoseMeshFormat::UV1 )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::TEXCOORD, Halia::VDeclType::FLOAT2, 0 ) );
		if( vertexformat & RoseMeshFormat::UV2 )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::TEXCOORD, Halia::VDeclType::FLOAT2, 1 ) );
		if( vertexformat & RoseMeshFormat::UV3 )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::TEXCOORD, Halia::VDeclType::FLOAT2, 2 ) );
		if( vertexformat & RoseMeshFormat::UV4 )
			vdecl->AddElement( Halia::VertexElement( 0, Halia::VDeclSemantic::TEXCOORD, Halia::VDeclType::FLOAT2, 3 ) );

		mVDecls.insert( std::pair<int,Halia::VertexDeclaration*>( vertexformat, vdecl ) );
		return vdecl;
	};

protected:
	std::string mPath;
	Halia::BoundingBox mBoundingBox;
	Halia::IndexedMeshMirror* mMesh;

	static std::map<int,Halia::VertexDeclaration*> mVDecls;
};