#pragma once
#include "IndexBufferBase.hpp"
#include "VertexDeclaration.hpp"

namespace Halia
{
	class IndexBufferMirror : public IndexBufferBase
	{
	public:
		IndexBufferMirror( )
		{
			mLength = 0;
			mStride = 0;
			mData = 0;
		};

		IndexBufferMirror( int length, int stride )
		{
			Init( length, stride );
		};

		IndexBufferMirror( IndexBufferBase* base )
		{
			Init( base->GetLength( ), base->GetStride( ) );
		};

		void Init( int length, int stride = 2 )
		{
			if( mData )
				delete[] mData;

			mLength = length;
			mStride = stride;
			mData = new char[ mLength * mStride ];
		};

		char* GetBuffer( )
		{
			return mData;
		};

		int GetBufferSize( )
		{
			return mLength * mStride;
		};

		int GetLength( )
		{
			return mLength;
		};

		int GetStride( )
		{
			return mStride;
		};

		char* mData;
		int mLength;
		int mStride;
	};
};