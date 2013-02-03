#pragma once
#include "VertexBufferBase.hpp"

namespace Halia
{
	template< typename T >
	class VertexBuffer : public VertexBufferBase
	{
	public:
		VertexBuffer( )
		{
		};

		void Resize( int size )
		{
			mVerts.resize( size );
		};
			
		void AddVertex( T vert )
		{
			mVerts.push_back( vert );
		};

		void SetVertex( int i, T vert )
		{
			mVerts[ i ] = vert;
		};

		template< typename U >
		U& GetVertex( int id ) const
		{
			return ((U*)&mVerts[0])[id];
		};

		T& GetVertex( int id ) const
		{
			return mVerts[ id ];
		};

		char* GetBuffer( ) const
		{
			return (char*)&mVerts[ 0 ];
		};

		int GetBufferSize( ) const
		{
			return mVerts.size( ) * sizeof( T );
		};

		int GetLength( ) const
		{
			return mVerts.size( );
		};

		int GetStride( ) const
		{
			return sizeof( T );
		};

		std::vector< T > mVerts;
	};
};