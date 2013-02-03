#pragma once
#include "Network.hpp"

namespace Halia
{
	namespace NetworkErr
	{
		enum NetworkErr
		{
			SUCCESS = 0,
			CREATE,
			BIND,
			LISTEN,
			ACCEPT,
			RESOLVE,
			CONNECT,

			NOTCONNECTED,
		};
	};

	class Socket
	{
	public:
		Socket( )
			: mSock( INVALID_SOCKET )
		{
		};

		~Socket( )
		{
#ifdef _DEBUG
			if( IsConnected( ) )
				printf( "Socket destructed before being closed\n" );
#endif
			Disconnect( );
		};

		int Bind( const std::string& ip, int port )
		{
			if( !Create( ) )
				return NetworkErr::CREATE;
			
			mSockInfo.sin_family = AF_INET;
			mSockInfo.sin_addr.s_addr = inet_addr( ip.c_str( ) );
			mSockInfo.sin_port = htons( port );
			
			if( bind( mSock, (SOCKADDR *)&mSockInfo, sizeof( mSockInfo ) ) == SOCKET_ERROR )
				return NetworkErr::BIND;

			return NetworkErr::SUCCESS;
		};

		int Listen( int backlog = SOMAXCONN )
		{
			if( listen( mSock, backlog ) == SOCKET_ERROR )
				return NetworkErr::LISTEN;
			return NetworkErr::SUCCESS;
		};
		
		int Accept( Socket& sock )
		{
			int sisize = sizeof( sock.mSockInfo );
			sock.mSock = accept( mSock, (SOCKADDR *)&sock.mSockInfo, &sisize );
			if( sock.mSock == INVALID_SOCKET )
				return NetworkErr::ACCEPT;
			return NetworkErr::SUCCESS;
		};

		int Connect( const std::string& host, int port )
		{
			if( !Create( ) )
				return NetworkErr::CREATE;

			hostent* hostent;
			if( !(hostent = gethostbyname( host.c_str( ) )))
				return NetworkErr::RESOLVE;

			mSockInfo.sin_family		= AF_INET;
			mSockInfo.sin_addr.s_addr	= *((unsigned long*)hostent->h_addr_list[0]);
			mSockInfo.sin_port		= htons( port );

			if( connect( mSock, (SOCKADDR *)&mSockInfo, sizeof( mSockInfo ) ) == SOCKET_ERROR )
				return NetworkErr::CONNECT;

			return NetworkErr::SUCCESS;
		};

		void Disconnect( )
		{
			shutdown( mSock, SD_BOTH );
			Destroy( );
		};

		int Send( void* buffer, int len )
		{
			if( mSock == INVALID_SOCKET )
				return 0;

			int retval = send( mSock, (char*)buffer, len, 0 );
			if( retval <= 0 ) {
				Disconnect( );
				return 0;
			}
			return retval;
		};

		int Recv( void* buffer, int len )
		{
			if( mSock == INVALID_SOCKET )
				return 0;

			int retval = recv( mSock, (char*)buffer, len, 0 );
			if( retval <= 0 ) {
				Disconnect( );
				return 0;
			}
			return retval;
		};

		bool IsConnected( )
		{
			return mSock != INVALID_SOCKET;
		};

		const SOCKET& GetSocket( )
		{
			return mSock;
		};

	private:
		bool Create( )
		{
			if( mSock != INVALID_SOCKET )
				return true;
			mSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
			if( mSock == INVALID_SOCKET )
				return false;
			return true;
		};

		void Destroy( )
		{
			closesocket( mSock );
			mSock = INVALID_SOCKET;
		};

	protected:
		SOCKET mSock;
		struct sockaddr_in mSockInfo;
	};
};