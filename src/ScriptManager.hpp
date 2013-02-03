#pragma once
#include <lua.hpp>
#include <string>
#include "ScriptListener.hpp"

namespace Halia
{
	class ScriptManager
	{
	public:
		ScriptManager( );
		~ScriptManager( );

		bool Init( );
		void SetListener( ScriptListener* l );
		bool LoadFile( std::string path );
		bool CallFunction( std::string name );
		bool AddFunction( std::string name );

		static int LUAPassthru( lua_State *L );

	private:
		ScriptListener* mListener;
		lua_State* mState;	
	};
};