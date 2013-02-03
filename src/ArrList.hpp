#pragma once
#include <windows.h>
#include "LogManager.hpp"

namespace Halia
{
	template< typename T >
	class ArrList
	{
	public:
		class Iter
		{
			friend ArrList;

		protected:
			inline Iter( ArrList* owner, int spos )
				: mOwner( owner ), mPos( spos )
			{
			};

		public:
			inline bool IsValid( ) const
			{
				return mPos >= 0 && mPos < mOwner->mSize;
			};

			inline Iter operator++( )
			{
				Iter retval = *this;
				mPos++;
				return retval;
			};

			inline Iter operator++( int ) {
				mPos++;
				return *this;
			};

			inline Iter operator--( )
			{
				Iter retval = *this;
				mPos--;
				return retval;
			};

			inline Iter operator--( int ) {
				mPos--;
				return *this;
			};

			inline T& operator*( ) const
			{
				return mOwner->mList[ mPos ];
			};

		private:
			ArrList* mOwner;
			int mPos;
		};

		inline ArrList( )
			: mList( 0 ), mRealSize( 0 ), mSize( 0 )
		{
			Reserve( );
		};

		inline ~ArrList( )
		{
#ifdef _DEBUG
			if( mSize != 0 )
				printf( "A ArrList was destructed before being cleared\n" );
#endif
		};

		inline Iter Begin( )
		{
			return Iter( this, 0 );
		};

		inline Iter End( )
		{
			return Iter( this, mSize - 1 );
		};

		void Reserve( )
		{
			int size = mRealSize * 2;
			if( size == 0 )
				size = 100;

			T* newlist = new T[ size ];
			if( mSize > 0 )
				memcpy( newlist, mList, mRealSize * sizeof( T ) );
			delete[] mList;

			mList = newlist;
			mRealSize = size;
		};

		void Reserve( int size )
		{
			if( mRealSize > size )
				return;

			T* newlist = new T[ size ];
			if( mSize > 0 )
				memcpy( newlist, mList, mRealSize * sizeof( T ) );
			delete[] mList;

			mList = newlist;
			mRealSize = size;
		};

		inline void Clear( )
		{
			mSize = 0;
		};

		inline void PushBack( T value )
		{
			if( mSize >= mRealSize )
				Reserve( );

			mList[ mSize++ ] = value;
		};

		inline T PopBack( )
		{
			return mList[ --mSize ];
		};

		inline int Size( )
		{
			return mSize;
		};

	private:
		T* mList;
		int mSize;
		int mRealSize;
	};
};