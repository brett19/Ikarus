#include "WindowsOSFileSystem.hpp"

namespace Halia
{
	WindowsOSFileSystem::FileStream::FileStream( )
	{
		mFh = 0;
	};

	bool WindowsOSFileSystem::FileStream::Open( const std::string& path )
	{
		mFh = fopen( path.c_str( ), "rb+" );
		if( mFh == 0 )
			return false;
		return true;
	};

	void WindowsOSFileSystem::FileStream::Close( )
	{
		if( mFh != 0 )
			fclose( mFh );
	};

	int WindowsOSFileSystem::FileStream::Read( void* buffer, int size )
	{
		return fread( buffer, 1, size, mFh );
	};

	int WindowsOSFileSystem::FileStream::Write( void* buffer, int size )
	{
		return fwrite( buffer, 1, size, mFh );
	};

	void WindowsOSFileSystem::FileStream::Seek( int offset, SeekOrigin::SeekOrigin origin )
	{
		fseek( mFh, offset, origin );
	};

	int WindowsOSFileSystem::FileStream::Tell( ) const
	{
		return ftell( mFh );
	};

	bool WindowsOSFileSystem::FileStream::Eof( ) const
	{
		return feof( mFh ) != 0;
	};

	WindowsOSFileSystem::WindowsOSFileSystem( const std::string& basepath )
	{
		mBasePath = basepath;
	};

	FileSystem::FileStream* WindowsOSFileSystem::OpenFile( const std::string& path )
	{
		FileStream* fh = new FileStream( );
		if( !fh->Open( mBasePath + path ) )
		{
			delete fh;
			return 0;
		}
		return (FileSystem::FileStream*)fh;
	};

	bool WindowsOSFileSystem::FileExists( const std::string& path )
	{
		FileStream* fh = new FileStream( );
		bool exists = fh->Open( mBasePath + path );
		fh->Close( );
		delete fh;
		return exists;
	};
};