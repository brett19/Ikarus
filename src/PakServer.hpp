PEVENT( PakSLInitEvent, RoseEventType::PAKSL_INIT )
	unsigned int cryptkey;
PEVENTEND( )

PEVENT( PakSLLoginEvent, RoseEventType::PAKSL_LOGIN )
	struct ServerEntry
	{
		char type;
		int id;
		std::string name;
	};

	int retcode;
	int right;
	int paytype;
	std::vector<ServerEntry> servers;
PEVENTEND( )

PEVENT( PakSLChnListEvent, RoseEventType::PAKSL_CHANNELLIST )
	struct ChannelEntry
	{
		int id;
		int lowage;
		int highage;
		int userpercent;
		std::string name;
	};

	int serverid;
	std::vector<ChannelEntry> channels;
PEVENTEND( )

PEVENT( PakSLCharInfoEvent, RoseEventType::PAKSL_CHARINFO )
	int retcode;
	unsigned int id1;
	unsigned int id2;
	std::string host;
	int port;
PEVENTEND( )

PEVENT( PakSCInitEvent, RoseEventType::PAKSC_INIT )
	unsigned int cryptkey;
PEVENTEND( )

PEVENT( PakSCCharListEvent, RoseEventType::PAKSC_CHARACTERLIST )
	struct CharEntry
	{
		int race;
		int level;
		int job;
		int time;
		bool isplatinum;
		PartItem items[ 10 ];
		std::string name;
	};
	
	std::vector< CharEntry > characters;
PEVENTEND( )

PEVENT( PakSCCreateCharEvent, RoseEventType::PAKSC_CREATECHARACTER )
	int retcode;
	int isplatinum;
PEVENTEND( )

PEVENT( PakSCDelCharEvent, RoseEventType::PAKSC_DELETECHARACTER )
	int time;
	std::string name;
PEVENTEND( )

PEVENT( PakSCWorldInfoEvent, RoseEventType::PAKSC_WORLDINFO )
	//??
PEVENTEND( )