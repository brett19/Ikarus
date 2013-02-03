#pragma once
#include <map>
#include "FileSystem.hpp"
#include "WindowsOSFileSystem.hpp"

namespace Halia
{
	class FileSystemManager : public FileSystem
	{
	public:
		FileSystemManager( );
		~FileSystemManager( );

		void AddFs( const std::string& name, FileSystem* fs );
		FileSystem* FindFs( const std::string& name );
		virtual FileSystem::FileStream* OpenFile( const std::string& path );
		virtual bool FileExists( const std::string& path );

	protected:
		typedef std::map<std::string, FileSystem*> FileSystemList;
		FileSystemList mFsList;
	};

	extern FileSystemManager* FSMInstance;
};
#define HFSM Halia::FSMInstance