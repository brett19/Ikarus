#pragma once
#include <iostream>
#include <string>

namespace Halia
{
	namespace StringType
	{
		enum StringType
		{
			NULLTERM = -1,
			PRE7 = 0,
			PRE8 = 1,
			PRE16 = 2,
			PRE32 = 3
		};
	}		

	namespace SeekOrigin
	{
		enum SeekOrigin
		{
			SET = 0,
			CUR = 1,
			END = 2
		};
	};

	class FileSystem
	{
	public:
		class FileStream
		{
		public:
			virtual bool Open( const std::string& path ) = 0;
			virtual void Close( ) = 0;
			virtual int Read( void* buffer, int size ) = 0;
			virtual int Write( void* buffer, int size ) = 0;
			virtual void Seek( int offset, SeekOrigin::SeekOrigin origin ) = 0;
			virtual int Tell( ) const = 0;
			virtual bool Eof( ) const = 0;

			virtual int GetSize( )
			{
				int curpos = Tell( );
				Seek( 0, SeekOrigin::END );
				int size = Tell( );
				Seek( curpos, SeekOrigin::SET );
				return size;
			};
			
			template<class T>
			T Read( )
			{
				T tmp;
				Read( &tmp, sizeof( T ) );
				return tmp;
			};

			std::string ReadString( int len )
			{
				char* tmp = new char[ len + 1 ];
				Read( tmp, len );
				tmp[ len ] = 0;
				std::string str( tmp );
				delete[] tmp;
				return str;
			};

			std::string ReadString( StringType::StringType lsize = StringType::NULLTERM )
			{
				if( lsize == StringType::NULLTERM ) {
					int tmps = 0;
					while( ++tmps && Read<char>( ) != 0 );
					Seek( -tmps, SeekOrigin::CUR );
					return ReadString( tmps );
				} else if( lsize == StringType::PRE7 ) {
					int tmpk = 0;
					char tmpc = Read<char>( );
					int tmps = tmpc & 0x7F;
					while( tmpc & 0x80 )
					{
						tmpc = Read<char>( );
						tmpk += 7;
						tmps |= ( tmpc & 0x7F ) << tmpk;
					}
					return ReadString( tmps );
				} else {
					int tmps = 0;
					Read( (char*)&tmps, lsize );
					return ReadString( tmps );
				}
			};

		protected:

		};

		virtual bool FileExists( const std::string& path ) = 0;
		virtual FileStream* OpenFile( const std::string& path ) = 0;
		virtual void CloseFile( FileStream* fh )
		{
			fh->Close( );
			delete fh;
		};

	protected:
		
	};
};