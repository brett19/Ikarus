#pragma once

class GSDebug
{
public:
	GSDebug( )
	{
	};

	~GSDebug( )
	{
	};

	bool Init( )
	{
	};

	bool Enter( )
	{
	};

	bool Leave( )
	{
	};

	bool PreFrameEvent( )
	{
		return true;
	};

	bool FrameEvent( )
	{
		return true;
	};

	bool EventHandler( Halia::Event* evt )
	{
		return true;
	};

protected:

};