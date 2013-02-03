#pragma once
#include "LogManager.hpp"

namespace Halia
{
	template< typename T >
	class SLList
	{
	public:
		class Entry
		{
			friend SLList;

		protected:
			inline Entry( T val, Entry* nxt )
				: value( val ), next( nxt )
			{
			};

			T value;
			Entry* next;
		};

		class Iter
		{
			friend SLList;

		protected:
			inline Iter( SLList* owner, Entry* start )
				: mOwner( owner ), mValue( start )
			{
			};

		public:
			inline bool IsValid( ) const
			{
				return mValue != 0;
			};

			inline Iter operator++( )
			{
				Iter retval = *this;
				mValue = mValue->next;
				return retval;
			};

			inline Iter operator++( int )
			{
				mValue = mValue->next;
				return *this;
			};

			inline T& operator*( ) const
			{
				return mValue->value;
			};

		private:
			SLList* mOwner;
			Entry* mValue;
		};

		inline SLList( )
			: mFirst( 0 ), mLast( 0 )
		{
		};

		inline ~SLList( )
		{
#ifdef _DEBUG
			if( mFirst )
				printf( "A SLList was destructed before being cleared\n" );
#endif
		};

		inline Iter Begin( )
		{
			return Iter( this, mFirst );
		};

		inline Iter End( )
		{
			return Iter( this, mEnd );
		};

		void PushBack( T value )
		{
			Entry* e = new Entry( value, 0 );

			if( !mFirst )
				mFirst = e;
			else
				mLast->next = e;
			mLast = e;
		};

		void PushFront( T value )
		{
			Entry* e = new Entry( value, mFirst );

			if( !mLast )
				mLast = e;
			mFirst = e;
		};

		T PopFront( )
		{
			Entry* cur = mFirst;
			T retval = cur->value;
			mFirst = cur->next;
			delete cur;
			return retval;
		};

	private:
		Entry* mFirst;
		Entry* mLast;
	};
};