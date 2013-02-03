#pragma once
#include "VertexBufferBase.hpp"

namespace Halia
{
	class VertexBufferMirror : public VertexBufferBase
	{
	public:
		VertexBufferMirror( )
		{
			mLength = 0;
			mStride = 0;
			mData = 0;
		};

		VertexBufferMirror( int length, int stride )
		{
			Init( length, stride );
		};

		VertexBufferMirror( VertexBufferBase* base )
		{
			Init( base->GetLength( ), base->GetStride( ) );
		};

		void Init( int length, int stride )
		{
			mLength = length;
			mStride = stride;
			mData = new char[ mLength * mStride ];
		};

		char* GetBuffer( ) const
		{
			return mData;
		};

		int GetBufferSize( ) const
		{
			return mLength * mStride;
		};

		int GetLength( ) const
		{
			return mLength;
		};

		int GetStride( ) const
		{
			return mStride;
		};

		char* mData;
		int mLength;
		int mStride;
	};
};