#pragma once

namespace Halia
{
	class SmartPtr
	{
	public:
		inline SmartPtr( )
		{
			mRefCount = 1;
		};

		inline ~SmartPtr( )
		{
		};

		inline int AddRef( )
		{
			return mRefCount++;
		};

		inline int DelRef( )
		{
			int rc = --mRefCount;
			if( rc == 0 )
				delete this;
			return rc;
		};

	private:
		int volatile mRefCount;
	};
};