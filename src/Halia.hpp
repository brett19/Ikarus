#pragma once
#include "Engine.hpp"

namespace Halia
{
	void Init( );
	void Destroy( );

	extern Engine* EInstance;
};
#define HENGINE Halia::EInstance
#define HRENDERER Halia::EInstance->GetRenderer( )
#define HINPUTER Halia::EInstance->GetInputer( )
#define HSCRIPTER Halia::EInstance->GetScripter( )
#define HOVERLAYER Halia::EInstance->GetOverlayer( )
#define HRESMGR Halia::EInstance->GetResourceManager( )