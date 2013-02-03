#include "FileSystemManager.hpp"

namespace Halia
{
	FileSystemManager::FileSystemManager( )
	{
	};

	FileSystemManager::~FileSystemManager( )
	{
		while( mFsList.begin( ) != mFsList.end( ) )
		{
			delete mFsList.begin( )->second;
			mFsList.erase( mFsList.begin( ) );
		};
	};

	void FileSystemManager::AddFs( const std::string& name, FileSystem* fs )
	{
		mFsList.insert( std::pair<std::string, FileSystem*>( name, fs ) );
	};

	FileSystem* FileSystemManager::FindFs( const std::string& name )
	{
		FileSystemList::iterator i = mFsList.find( name );
		if( i != mFsList.end( ) )
			return (*i).second;
		return 0;
	};

	FileSystem::FileStream* FileSystemManager::OpenFile( const std::string& path )
	{
		for( FileSystemList::iterator i = mFsList.begin( ); i != mFsList.end( ); i++ )
		{
			FileStream* fh = (*i).second->OpenFile( path );
			if( fh != 0 )
				return fh;
		}
		return 0;
	};

	bool FileSystemManager::FileExists( const std::string& path )
	{
		for( FileSystemList::iterator i = mFsList.begin( ); i != mFsList.end( ); i++ )
		{
			if( (*i).second->FileExists( path ) )
				return true;
		}
		return false;
	};

	FileSystemManager* FSMInstance = 0;
};