#pragma once
#include <lua.hpp>
#include "ScriptManager.hpp"

namespace Halia
{
	ScriptManager::ScriptManager( )
	{
	};

	ScriptManager::~ScriptManager( )
	{
	};

	bool ScriptManager::Init( )
	{
		mState = lua_open( );
		if( mState == 0 )
			return false;
		luaL_openlibs( mState );
		return true;
	};

	void ScriptManager::SetListener( ScriptListener* l )
	{
		mListener = l;
	};

	bool ScriptManager::LoadFile( std::string path )
	{
		if( luaL_loadfile( mState, path.c_str( ) ) != 0 )
		{
			printf( "Could not load LUA file '%s' => '%s'\n", path.c_str( ), lua_tostring(mState, -1) );
			return false;
		}
		if( lua_pcall( mState, 0, 0, 0 ) != 0 )
		{
			printf( "Could not init LUA file '%s' => '%s'\n", path.c_str( ), lua_tostring(mState, -1) );
			return false;
		}
		return true;
	};

	bool ScriptManager::CallFunction( std::string name )
	{
		lua_getglobal( mState, name.c_str( ) );
		if( lua_pcall( mState, 0, 0, 0 ) != 0 )
		{
			printf( "Could not call function '%s' => '%s'\n", name.c_str( ), lua_tostring(mState, -1) );
			return false;
		}
		return true;
	};

	bool ScriptManager::AddFunction( std::string name )
	{
		lua_pushlightuserdata( mState, (void*)this );
		lua_pushstring( mState, name.c_str( ) );
		lua_pushcclosure( mState, (lua_CFunction)ScriptManager::LUAPassthru, 2 );
		lua_setglobal( mState, name.c_str( ) );
		return true;
	};

	int ScriptManager::LUAPassthru( lua_State *L )
	{
		ScriptManager* me = (ScriptManager*)lua_touserdata( L, lua_upvalueindex(1) );
		char* fname = (char*)lua_tostring( L, lua_upvalueindex(2) );
		return me->mListener->ScriptEvent( L, fname );
	};
};