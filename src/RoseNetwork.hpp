/*
  THIS FILE NEEDS TO HAVE ITS EVENT LOCKED UP TO PREVENT CROSS-THREAD FUCK UPS
*/
#pragma once
#include <string>
#include "EventListener.hpp"
#include "RoseSocket.hpp"
#include "ArrList.hpp"
#include "CriticalSection.hpp"
#include "Thread.hpp"
#include "Event.hpp"
#include "RoseEvents.hpp"
#include "RosePacket.hpp"

class RoseNetwork : public Halia::Thread
{
public:
	RoseNetwork( )
		: Thread( true )
	{
	};

	~RoseNetwork( )
	{
	};

	bool Init( )
	{
		return true;
	};

	void SetLoginInfo( const std::string& host, int port )
	{
		mLoginHost = host;
		mLoginPort = port;
	};

	void SetCharInfo( const std::string& host, int port, unsigned int seed )
	{
		mCharHost = host;
		mCharPort = port;
		mCharSeed = seed;
	};

	void SetWorldInfo( const std::string& host, int port, unsigned int seed )
	{
		mWorldHost = host;
		mWorldPort = port;
		mWorldSeed = seed;
	};

	void Destroy( )
	{
	};

	void Poll( )
	{
		mQueueOutLock.Enter( );
		for( Halia::ArrList<Halia::EventBase*>::Iter i = mQueueOut.Begin( ); i.IsValid( ); ++i )
			mListener->EventHandler( *i );
		mQueueOut.Clear( );
		mQueueOutLock.Leave( );
	};

	void SetListener( Halia::EventListener* ls )
	{
		mListener = ls;
	};

	void PushEvent( Halia::EventBase* evt )
	{
		mEvent.Set( );
		mQueueInLock.Enter(	);
		mQueueIn.PushBack( evt );
		mQueueInLock.Leave( );
	};

	void RaiseEvent( Halia::EventBase* evt )
	{
		mQueueOutLock.Enter( );
		mQueueOut.PushBack( evt );
		mQueueOutLock.Leave( );
	};

	void ProcEvent( Halia::EventBase* evt )
	{
		switch( evt->GetType( ) )
		{
			case RoseEventType::NET_CONNECT:
			{
				NetworkEvent* revt = (NetworkEvent*)evt;
				int retval = -1;//Halia::NetworkErr::GENERAL;
				if( revt->sockid == RoseSockType::LOGIN ) {
					retval = mLoginSock.Connect( mLoginHost, mLoginPort );
				} else if( revt->sockid == RoseSockType::CHAR ) {
					retval = mCharSock.Connect( mCharHost, mCharPort, mCharSeed );
				} else if( revt->sockid == RoseSockType::WORLD ) {
					retval = mWorldSock.Connect( mWorldHost, mWorldPort, mWorldSeed );
				}

				if( retval == Halia::NetworkErr::SUCCESS ) {
					NetworkEvent* nevt = new NetworkEvent( RoseEventType::NET_CONNECTED, revt->sockid );
					RaiseEvent( nevt );
				} else {
					NetworkEvent* nevt = new NetworkEvent( RoseEventType::NET_ERROR, revt->sockid );
					RaiseEvent( nevt );
				}

				break;
			}

			/* START CLIENT->SERVER PACKETS */

			case RoseEventType::PAKCL_INIT:
			{
				PakCLInitEvent* revt = (PakCLInitEvent*)evt;
				RosePacket pak( 0x703 );
				mLoginSock.SendPacket( &pak );
				break;
			}
			case RoseEventType::PAKCL_LOGIN:
			{
				PakCLLoginEvent* revt = (PakCLLoginEvent*)evt;
				RosePacket pak( 0x708 );
				pak.AddString( revt->password, false );
				pak.AddString( revt->username );
				mLoginSock.SendPacket( &pak );
				break;
			}
			case RoseEventType::PAKCL_SELECTSERVER:
			{
				PakCLSrvSelEvent* revt = (PakCLSrvSelEvent*)evt;
				RosePacket pak( 0x704 );
				pak.Add<int>( revt->serverid );
				mLoginSock.SendPacket( &pak );
				break;
			}
			case RoseEventType::PAKCL_SELECTCHANNEL:
			{
				PakCLChnSelEvent* revt = (PakCLChnSelEvent*)evt;
				RosePacket pak( 0x70a );
				pak.Add<int>( revt->serverid );
				pak.Add<char>( revt->channelid );
				mLoginSock.SendPacket( &pak );
				break;
			}
			case RoseEventType::PAKCC_INIT:
			{
				PakCCInitEvent* revt = (PakCCInitEvent*)evt;
				RosePacket pak( 0x70b );
				pak.Add<unsigned int>( revt->id );
				pak.AddString( revt->password, false );
				mCharSock.SendPacket( &pak );
				break;
			}
			case RoseEventType::PAKCC_GETCHARACTERS:
			{
				PakCCGetCharsEvent* revt = (PakCCGetCharsEvent*)evt;
				RosePacket pak( 0x712 );
				mCharSock.SendPacket( &pak );
				break;
			}
		}
	};

	void ProcPacket( RosePacket& pak, int sockid )
	{
		if( sockid == RoseSockType::LOGIN ) {
			switch( pak.GetCommand( ) )
			{
				case 0x7ff:
				{
					PakSLInitEvent* evt = new PakSLInitEvent( );
					evt->cryptkey = pak.Get<unsigned int>( 1 );
					RaiseEvent( evt );
					mLoginSock.SetCryptKey( evt->cryptkey );
					break;
				}
				case 0x708:
				{
					PakSLLoginEvent* evt = new PakSLLoginEvent( );
					evt->retcode = pak.Get<char>( );
					evt->right = pak.Get<short>( );
					evt->paytype = pak.Get<short>( );
					if( evt->retcode == 0 )
					{
						while( !pak.Eof( ) )
						{
							PakSLLoginEvent::ServerEntry srv;
							srv.type = pak.Get<char>( );
							srv.name = pak.GetString( );
							srv.id = pak.Get<int>( );
							evt->servers.push_back( srv );
						}
					}
					RaiseEvent( evt );
					break;
				}
				case 0x704:
				{
					PakSLChnListEvent* evt = new PakSLChnListEvent( );
					evt->serverid = pak.Get<int>( );
					char chncount = pak.Get<char>( );
					for( int i = 0; i < chncount; ++i )
					{
						PakSLChnListEvent::ChannelEntry chn;
						chn.id = pak.Get<unsigned char>( );
						chn.lowage = pak.Get<unsigned char>( );
						chn.highage = pak.Get<unsigned char>( );
						chn.userpercent = pak.Get<short>( );
						chn.name = pak.GetString( );
						evt->channels.push_back( chn );
					}
					RaiseEvent( evt );
					break;
				}
				case 0x70a:
				{
					PakSLCharInfoEvent* evt = new PakSLCharInfoEvent( );
					evt->retcode = pak.Get<char>( );
					evt->id1 = pak.Get<unsigned int>( );
					evt->id2 = pak.Get<unsigned int>( );
					evt->host = pak.GetString( );
					evt->port = pak.Get<unsigned short>( );
					RaiseEvent( evt );
					break;
				}
			}
		} else if( sockid == RoseSockType::CHAR ) {
			switch( pak.GetCommand( ) )
			{
				case 0x70c:
				{
					PakSCInitEvent* evt = new PakSCInitEvent( );
					evt->cryptkey = pak.Get<unsigned int>( 1 );
					RaiseEvent( evt );
					mCharSock.SetCryptKey( evt->cryptkey );
					break;
				}
				case 0x712:
				{
					PakSCCharListEvent* evt = new PakSCCharListEvent( );
					int charcount = pak.Get<unsigned char>( );
					for( int i = 0; i < charcount; i++ )
					{
						PakSCCharListEvent::CharEntry chr;
						chr.name = pak.GetString( );
						chr.race = pak.Get<unsigned char>( );
						chr.level = pak.Get<short>( );
						chr.job = pak.Get<short>( );
						chr.time = pak.Get<unsigned int>( );
						chr.isplatinum = pak.Get<char>( ) == 1;
						for( int j = 0; j < 10; j++ )
						{
							chr.items[j].value = pak.Get<unsigned int>( );
						}
						printf( "Char name: %s\n", chr.name.c_str( ) );
						evt->characters.push_back( chr );
					}
					RaiseEvent( evt );
					break;
				}
			}
		} else if( sockid == RoseSockType::WORLD ) {
			switch( pak.GetCommand( ) )
			{
				case 0x70c:
				{
					break;
				}
			}
		}
	};

	void ProcSocket( int sockid )
	{
		RosePacket pak;
		int retval = -1;

		if( sockid == RoseSockType::LOGIN )
			retval = mLoginSock.RecvPacket( &pak );
		else if( sockid == RoseSockType::CHAR )
			retval = mCharSock.RecvPacket( &pak );
		else if( sockid == RoseSockType::WORLD )
			retval = mWorldSock.RecvPacket( &pak );

		if( retval < 0 )
			return;

		if( retval > 0 ) {
			NetworkEvent* evt = new NetworkEvent( RoseEventType::NET_DISCONNECTED, sockid );
			RaiseEvent( evt );
			return;
		}

		ProcPacket( pak, sockid );
	};

	int Execute( )
	{
		while( IsRunning( ) )
		{
			mEvent.WaitFor( );

			mQueueInLock.Enter( );
			for( Halia::ArrList<Halia::EventBase*>::Iter i = mQueueIn.Begin( ); i.IsValid( ); ++i )
				ProcEvent( *i );
			mQueueIn.Clear( );
			mQueueInLock.Leave( );

			fd_set fds;
			FD_ZERO( &fds );
			if( mLoginSock.IsConnected( ) )
				FD_SET( mLoginSock.GetSocket( ), &fds );
			if( mCharSock.IsConnected( ) )
				FD_SET( mCharSock.GetSocket( ), &fds );
			if( mWorldSock.IsConnected( ) )
				FD_SET( mWorldSock.GetSocket( ), &fds );
			if( fds.fd_count == 0 ) {
				// No sockets connected, lock down the loop
				mEvent.Reset( );
				continue;
			}
			timeval tm = { 0, 0 };
			int sv = select( 0, &fds, 0, 0, &tm );
			if( FD_ISSET( mLoginSock.GetSocket( ), &fds ) )
				ProcSocket( RoseSockType::LOGIN );
			if( FD_ISSET( mCharSock.GetSocket( ), &fds ) )
				ProcSocket( RoseSockType::CHAR );
			if( FD_ISSET( mWorldSock.GetSocket( ), &fds ) )
				ProcSocket( RoseSockType::WORLD );
		}

		return 0;
	};

protected:
	Halia::EventListener* mListener;
	Halia::Event mEvent;
	Halia::ArrList< Halia::EventBase* > mQueueIn;
	Halia::CriticalSection mQueueInLock;
	Halia::ArrList< Halia::EventBase* > mQueueOut;
	Halia::CriticalSection mQueueOutLock;
	RoseSocket mLoginSock;
	RoseSocket mCharSock;
	RoseSocket mWorldSock;
	std::string mLoginHost;
	int mLoginPort;
	std::string mCharHost;
	int mCharPort;
	unsigned int mCharSeed;
	std::string mWorldHost;
	int mWorldPort;
	unsigned int mWorldSeed;
};

extern RoseNetwork* RNInstance;
#define IKNET RNInstance