#pragma once
#include <vector>
#include "Events.hpp"
#include "RoseData.hpp"

/*
namespace RoseEventType
{
	enum RoseEventType
	{
		ROSEBASE = Halia::EventType::CUSTOM,

		WINDOWSTART,
		LOGINOKAYCLICK,
		LOGINCANCELCLICK,
		SVRCHNOKAYCLICK,
		SVRCHNCANCELCLICK,
		DIALOGOKAYCLICK,
		DIALOGCANCELCLICK,
		WINDOWEND,

		NETSTART,
		NET_CONNECT,
		NET_DISCONNECT,
		NET_CONNECTED,
		NET_DISCONNECTED,
		NET_ERROR,
		NETEND,

		PAKSTART,
		/* CLIENT->SERVER PACKETS /
		PAKCL_INIT,
		PAKCL_LOGIN,
		PAKCL_SELECTSERVER,
		PAKCL_SELECTCHANNEL,
		PAKCC_INIT,
		PAKCC_GETCHARACTERS,
		PAKCC_CREATECHARACTER,
		PAKCC_DELETECHARACTER,
		PAKCC_SELECTCHARACTER,
		PAKCW_INIT,
		/* SERVER->CLIENT PACKETS /
		PAKSL_INIT,
		PAKSL_LOGIN,
		PAKSL_CHANNELLIST,
		PAKSL_CHARINFO,
		PAKSC_INIT,
		PAKSC_CHARACTERLIST,
		PAKSC_CREATECHARACTER,
		PAKSC_DELETECHARACTER,
		PAKSC_WORLDINFO,
		PAKSW_INIT,
		/* DONE PACKETS /
		PAKEND,
	};
};


namespace RoseSockType
{
	enum RoseSockType
	{
		LOGIN,
		CHAR,
		WORLD
	};
};

/*
NET_CONNECT, NET_DISCONNECT, NET_CONNECTED, NET_DISCONNECTED, NET_ERROR
/
class NetworkEvent : public Halia::Event
{
public:
	NetworkEvent( int type, int sockid_ )
		: EventBase( type ), sockid( sockid_ )
	{
	};

	int sockid;
};

class PacketEvent : public Halia::EventBase
{
public:
	PacketEvent( int type )
		: EventBase( type )
	{
	};
};

#define PEVENT(x,y) class x : public PacketEvent { public: x( ) : PacketEvent( y ) { };
#define PEVENTMULTI(x) class x : public PacketEvent { public: x( int type ) : PacketEvent( type ) { };
#define PEVENTEND( ) };

#include "PakClient.hpp"
#include "PakServer.hpp"
*/