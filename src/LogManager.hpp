#pragma once
#include <stdio.h>
#include <stdarg.h>

namespace Halia
{
	namespace LogMsgType
	{
		enum LogMsgType
		{
			ERR = 0,
			WARN = 1,
			MSG = 2,
			DBG = 3,
		};
	};

	class LogManager
	{
	public:
		LogManager( );
		~LogManager( );

		void Open( char* path = "halia.log" );
		void Close( );
		void Write( LogMsgType::LogMsgType msgtype, char* format, ... );

	private:
		FILE* mLogFile;
	};

	extern LogManager* LMInstance;
};
#define HLOGGER Halia::LMInstance
#define HLOGERR(msg,...) Halia::LMInstance->Write( Halia::LogMsgType::ERR, msg, __VA_ARGS__ )
#define HLOGWARN(msg,...) Halia::LMInstance->Write( Halia::LogMsgType::WARN, msg, __VA_ARGS__ )
#define HLOGMSG(msg,...) Halia::LMInstance->Write( Halia::LogMsgType::MSG, msg, __VA_ARGS__ )
#define HLOGDBG(msg,...) Halia::LMInstance->Write( Halia::LogMsgType::DBG, msg, __VA_ARGS__ )