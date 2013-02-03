#include "RenderWindow.hpp"

namespace Halia
{
	RenderWindow::RenderWindow( )
		: mWindow( 0 )
	{
	};

	RenderWindow::~RenderWindow( )
	{
	};

	bool RenderWindow::Init( std::string title, int width, int height, unsigned int style )
	{
		WNDCLASS wndclass;
		wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
		wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
		wndclass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
		wndclass.hInstance = 0;
		wndclass.lpfnWndProc = WndProc;
		wndclass.lpszClassName = "HaliaWindow";
		wndclass.lpszMenuName = 0;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.style = 0;

		if( !RegisterClass( &wndclass ) )
			return false;

		mWindow = CreateWindow( "HaliaWindow", title.c_str( ), 
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, 0, (void*)this );
		SetWindowText( mWindow, title.c_str( ) );

		///*
		RECT tmp;
		GetClientRect( mWindow, &tmp );
		tmp.right = width + ( width - tmp.right );
		tmp.bottom = height + ( height - tmp.bottom );
		SetWindowPos( mWindow, 0, 0, 0, tmp.right, tmp.bottom, SWP_NOMOVE );
		//*/

		ShowWindow( mWindow, 1 );

		return true;
	};

	bool RenderWindow::MessageHandler( UINT iMessage, WPARAM wParam, LPARAM lParam )
	{
		switch( iMessage )
		{
			case WM_CLOSE:
				PostQuitMessage( 0 );
				return true;
			default:
				break;
		}

		return false;
	};

	bool RenderWindow::Poll( )
	{
		MSG msg;
		PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
		if( msg.message == WM_QUIT )
		{
			return false;
		}
		else
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		return true;
	};

	HWND& RenderWindow::GetHwnd( )
	{
		return mWindow;
	};

	LRESULT CALLBACK RenderWindow::WndProc( HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam )
	{
		bool bProcessed = false;

		if( iMessage == WM_NCCREATE )
		{
			SetWindowLong( hWindow, GWL_USERDATA, (int)((LPCREATESTRUCT(lParam))->lpCreateParams) );
		}

		RenderWindow* mywindow = (RenderWindow*)GetWindowLong( hWindow, GWL_USERDATA );
		if( mywindow != 0 )
			bProcessed = mywindow->MessageHandler( iMessage, wParam, lParam );

		if( !bProcessed )
		{
			return DefWindowProc( hWindow, iMessage, wParam, lParam );
		}

		return 0;
	};
};