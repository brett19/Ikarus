#pragma once
#include "RenderCached.hpp"

namespace Halia
{
	class IndexBufferBase : public RenderCached
	{
	public:
		virtual char* GetBuffer( ) = 0;
		virtual int GetBufferSize( ) = 0;
		virtual int GetLength( ) = 0;
		virtual int GetStride( ) = 0;
	};
};