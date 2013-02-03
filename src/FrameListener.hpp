#pragma once

namespace Halia
{
	class FrameListener
	{
	public:
		FrameListener( );
		~FrameListener( );

		virtual bool FrameEvent( ) = 0;
		virtual bool PreFrameEvent( ) = 0;

	};
};