#pragma once
#include "RenderCached.hpp"

namespace Halia
{
	class VertexBufferBase : public RenderCached
	{
	public:
		virtual char* GetBuffer( ) const = 0;
		virtual int GetBufferSize( ) const = 0;
		virtual int GetLength( ) const = 0;
		virtual int GetStride( ) const = 0;
	};
};