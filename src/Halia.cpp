#include "Halia.hpp"

namespace Halia
{
	void Init( )
	{
		LMInstance = new LogManager( );
		FSMInstance = new FileSystemManager( );
		EInstance = new Engine( );
		Halia::Network::Init( );
	};

	void Destroy( )
	{
		Halia::Network::Destroy( );
		delete EInstance;
		delete FSMInstance;
		delete LMInstance;
	};
};