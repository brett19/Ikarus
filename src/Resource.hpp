#pragma once
#include "SmartPtr.hpp"

namespace Halia
{
	class Resource : public SmartPtr
	{
	public:
		Resource( );
		~Resource( );

		bool IsLoaded( ) const;
		virtual bool Load( ) = 0;
		virtual void Unload( ) = 0;

	protected:
		bool volatile mLoaded;
	};
};