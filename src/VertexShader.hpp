#pragma once
#include <string>
#include "RenderCached.hpp"
#include "Resource.hpp"

namespace Halia
{
	class VertexShader : public RenderCached, public Resource
	{
	public:
		VertexShader( );
		VertexShader( const std::string& path );
		~VertexShader( );

		void SetPath( const std::string& path );
		bool Load( );
		void Unload( );

		std::string mPath;
		int mDataLen;
		unsigned char* mData;
	};
};