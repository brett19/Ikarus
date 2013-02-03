#pragma once
#include <winsock2.h>
#include "Socket.hpp"

namespace Halia
{
	class Network
	{
	public:
		static bool Init( )
		{
			WSADATA wsaData;
			return( !WSAStartup( 0x202, &wsaData ) );
		};

		static void Destroy( )
		{
			WSACleanup( );
		};
	};
};