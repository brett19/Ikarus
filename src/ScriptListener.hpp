#pragma once
#include <lua.hpp>

namespace Halia
{
	class ScriptListener
	{
	public:
		ScriptListener( );
		~ScriptListener( );

		virtual int ScriptEvent( lua_State *L, char* func ) = 0;

	};
};