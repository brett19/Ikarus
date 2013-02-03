PEVENT( PakCLInitEvent, RoseEventType::PAKCL_INIT )
PEVENTEND( )

PEVENT( PakCLLoginEvent, RoseEventType::PAKCL_LOGIN )
	std::string username;
	std::string password;
PEVENTEND( )

PEVENT( PakCLSrvSelEvent, RoseEventType::PAKCL_SELECTSERVER )
	int serverid;
PEVENTEND( )

PEVENT( PakCLChnSelEvent, RoseEventType::PAKCL_SELECTCHANNEL )
	int serverid;
	int channelid;
PEVENTEND( )

PEVENT( PakCCInitEvent, RoseEventType::PAKCC_INIT )
	unsigned int id;
	std::string password;
PEVENTEND( )

PEVENT( PakCCGetCharsEvent, RoseEventType::PAKCC_GETCHARACTERS )
PEVENTEND( )

PEVENT( PakCCCreateCharEvent, RoseEventType::PAKCC_CREATECHARACTER )
	int race;
	int stone;
	int hair;
	int face;
	int weapon;
	int zone;
	std::string name;
PEVENTEND( )

PEVENT( PakCCDeleteCharEvent, RoseEventType::PAKCC_DELETECHARACTER )
	int id;
	bool del;
	std::string name;
PEVENTEND( )

PEVENT( PakCCSelCharEvent, RoseEventType::PAKCC_SELECTCHARACTER )
	int id;
	int runmode;
	int ridemode;
	std::string name;
PEVENTEND( )

PEVENT( PakCWInitEvent, RoseEventType::PAKCW_INIT )
	unsigned int id;
	std::string password;
PEVENTEND( )
