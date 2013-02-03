#pragma once
#include <string>
#include <freeimage.h>
#include "FileSystemManager.hpp"
#include "RenderCached.hpp"
#include "Resource.hpp"
#include "Point.hpp"

namespace Halia
{
	namespace TextureFormat
	{
		enum TextureFormat
		{
			R8G8B8 = 20,
			A8R8G8B8 = 21,
			X8R8G8B8 = 22,
			R5G6B5 = 23,
			X1R5G5B5 = 24,
			A1R5G5B5 = 25,
			A4R4G4B4 = 26,
			R3G3B2 = 27,
			A8 = 28,
			A8R3G3B2 = 29,
			X4R4G4B4 = 30,
			A2B10G10R10 = 31,
			A8B8G8R8 = 32,
			X8B8G8R8 = 33,
			G16R16 = 34,
			A2R10G10B10 = 35,
			A16B16G16R16 = 36
		};

		// STARTS AT 20
		const int LENGTH[] = { 
			/*  TO 20 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			/*  20 ON */ 3, 4, 4, 2, 2, 2, 2, 1, 1, 2, 2, 4, 4, 4, 4, 4, 8
		};
	};

	class TextureBase : public RenderCached
	{
	public:
		TextureBase( )
			: mData( 0 ), mDataLen( 0 ), mSize( 0, 0 )
		{
		};

		~TextureBase( )
		{
		};

		const int& GetWidth( ) const
		{
			return mSize.x;
		};

		const int& GetHeight( ) const
		{
			return mSize.y;
		};

		virtual int GetFormat( ) = 0;

		int mDataLen;
		unsigned char* mData;
		Point mSize;
	};

	class TextureMemory : public TextureBase
	{
	public:
		TextureMemory( )
			: mFormat( -1 )
		{
		};

		~TextureMemory( )
		{
		};

		bool Init( int width, int height, TextureFormat::TextureFormat format )
		{
			mSize = Point( width, height );
			mFormat = format;
			mDataLen = width * height * TextureFormat::LENGTH[ (int)format ];
			mData = new unsigned char[ mDataLen ];

			return true;
		};

		char* GetBuffer( )
		{
			return (char*)mData;
		};

		int GetFormat( )
		{
			return mFormat;
		};

	protected:
		int mFormat;
	};

	class TextureFile : public TextureBase, public Resource
	{
	public:
		TextureFile( )
			: mPath( "" )
		{
		};

		TextureFile( const std::string& path )
			: mPath( path )
		{
		};

		~TextureFile( )
		{
		};

		void SetPath( const std::string& path )
		{
			mPath = path;
		};

		bool Load( )
		{
			//:: Check if its already loaded
			if( mData )
				return true;

			Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mPath );
			if( !fh ) {
				printf( "Failed to load Texture '%s'\n", mPath.c_str( ) );
				return false;
			}

			mDataLen = fh->GetSize( );
			mData = new unsigned char[ mDataLen ];
			fh->Read( (char*)mData, mDataLen );

			Halia::FSMInstance->CloseFile( fh );

			FIMEMORY* imgmem = FreeImage_OpenMemory( mData, mDataLen );
			FREE_IMAGE_FORMAT imgf = FreeImage_GetFileTypeFromMemory( imgmem, 0 );
			FIBITMAP* img = FreeImage_LoadFromMemory( imgf, imgmem, 0 );
			mSize.x = FreeImage_GetWidth( img );
			mSize.y = FreeImage_GetHeight( img );
			FreeImage_Unload( img );
			FreeImage_CloseMemory( imgmem );

			return true;
		};

		void Unload( )
		{
			if( mData )
				delete[] mData;
		};

		int GetFormat( )
		{
			return 0;	
		};

	protected:
		std::string mPath;
	};
};