#pragma once
#include "Socket.hpp"
#include "RoseCrypt.hpp"
#include "RosePacket.hpp"

class RoseSocket : public Halia::Socket
{
public:
	RoseSocket( )
		: mCryptKey( 0 )
	{
	};

	~RoseSocket( )
	{
	};

	int Connect( std::string host, int port )
	{
		mCrypt.Default( );
		return Socket::Connect( host, port );
	};

	int Connect( std::string host, int port, unsigned int seed )
	{
		mCrypt.Init( seed );
		return Socket::Connect( host, port );
	};

	int SendPacket( RosePacket* pak )
	{
		printf( "RoseSocket::SendPacket( %04x, %d );\n", pak->GetCommand( ), pak->GetSize( ) );
		int paksize = mCrypt.ESCP( (unsigned char*)pak, mCryptKey );
		return Send( pak, paksize );
	};

	int RecvPacket( RosePacket* pak )
	{
		if( Recv( pak, 6 ) != 6 ) {
			printf( "RECV ERR: Not enough data 1\n" );
			return 1;
		}
		int paksize = mCrypt.DRSH( (unsigned char*)pak );

		if( Recv( (char*)pak+6, paksize-6 ) != paksize-6 ) {
			printf( "RECV ERR: Not enough data 2\n" );
			return 2;
		}
		if( !mCrypt.DRSB( (unsigned char*)pak ) ) {
			printf( "RECV ERR: Decrypt failed\n" );
			return 3;
		}
		printf( "RoseSocket::RecvPacket( %04x, %d );\n", pak->GetCommand( ), pak->GetSize( ) );
		return 0;
	};

	void SetCryptKey( unsigned int key )
	{
		mCryptKey = key;
	};

protected:
	CPC mCrypt;
	int mCryptKey;
};