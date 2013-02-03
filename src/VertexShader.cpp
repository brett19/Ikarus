#include "VertexShader.hpp"
#include "FileSystemManager.hpp"
#include "RenderCached.hpp"
#include "Resource.hpp"
#include "Point.hpp"

namespace Halia
{
	VertexShader::VertexShader( )
		: mPath( "" ), mData( 0 )
	{
	}

	VertexShader::VertexShader( const std::string& path )
		: mPath( path ), mData( 0 )
	{
	};

	VertexShader::~VertexShader( )
	{
	};

	void VertexShader::SetPath( const std::string& path )
	{
		mPath = path;
	};

	bool VertexShader::Load( )
	{
		//:: Check if its already loaded
		if( mData )
			return true;

		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mPath );
		if( !fh ) {
			printf( "Failed to load Vertex Shader '%s'\n", mPath.c_str( ) );
			return false;
		}

		mDataLen = fh->GetSize( );
		mData = new unsigned char[ mDataLen ];
		fh->Read( (char*)mData, mDataLen );

		Halia::FSMInstance->CloseFile( fh );

		return true;
	};

	void VertexShader::Unload( )
	{
		if( mData )
			delete[] mData;
	};
};