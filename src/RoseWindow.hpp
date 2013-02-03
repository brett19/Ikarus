#pragma once
#include "Overlay.hpp"

namespace RoseOverlayType
{
	enum RoseOverlayType
	{
		ROSEBASE = Halia::OverlayType::CUSTOM,
		LOGINWND,
		SRVCHNWND,
		DIALOGWND,
	};
};

class RoseWindow : public Halia::Overlay
{
public:
	RoseWindow( )
	{
	};

	RoseWindow( int x, int y )
		: Overlay( x, y )
	{
	};

	RoseWindow( int x, int y, int sx, int sy )
		: Overlay( x, y, sx, sy )
	{
	};

	~RoseWindow( )
	{
	};
};