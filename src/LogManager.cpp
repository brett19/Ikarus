#include "LogManager.hpp"

namespace Halia
{
	LogManager::LogManager( )
		: mLogFile( 0 )
	{
	};

	LogManager::~LogManager( )
	{
		Close( );
	};

	void LogManager::Open( char* path )
	{
		mLogFile = fopen( path, "wb" );
	};

	void LogManager::Close( )
	{
		if( mLogFile )
		{
			fclose( mLogFile );
			mLogFile = 0;
		};
	};

	void LogManager::Write( LogMsgType::LogMsgType msgtype, char* format, ... )
	{
		va_list args;
		va_start( args, format );
		vprintf( format, args );
		if( mLogFile )
			vfprintf( mLogFile, format, args );
		va_end( args );
		printf( "\n" );
		fprintf( mLogFile, "\n" );
	};

	LogManager* LMInstance = 0;
};