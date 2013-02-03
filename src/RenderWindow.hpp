#pragma once
#include <windows.h>
#include <string>

namespace Halia
{
	class RenderWindow
	{
	public:
		RenderWindow( );
		~RenderWindow( );

		bool Init( std::string title, int width, int height, unsigned int style = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_VISIBLE );
		bool MessageHandler( UINT iMessage, WPARAM wParam, LPARAM lParam );
		bool Poll( );
		HWND& GetHwnd( );

		static LRESULT CALLBACK WndProc( HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam );

	private:
		HWND mWindow;
	};
};