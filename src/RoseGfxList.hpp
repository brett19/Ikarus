#pragma once
#include <string>
#include <vector>
#include <map>
#include "OverlayTexture.hpp"
#include "Texture.hpp"

class RoseGfxList : public Halia::Resource
{
public:
	struct Element
	{
		std::string name;
		Halia::OverlayTexture* ot;
	};

	RoseGfxList( )
		: mPath( "" )
	{
	};

	RoseGfxList( const std::string& path )
		: mPath( path )
	{
	};

	void SetPath( const std::string& path )
	{
		mPath = path;
	};

	bool Load( )
	{
		Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mPath );
		if( !fh )
			return false;

		short ddscount = fh->Read<short>( );
		for( int i = 0; i < ddscount; i++ )
		{
			std::string path = fh->ReadString( Halia::StringType::PRE16 );
			unsigned int colorkey = fh->Read<unsigned int>( );

			Halia::TextureFile* tex = new Halia::TextureFile( "3DDATA\\CONTROL\\RES\\" + path );
			tex->Load( );
			mDdsList.push_back( tex );
		}

		short totalelements = fh->Read<short>( );
		for( int i = 0; i < ddscount; i++ )
		{
			short ddselemcount = fh->Read<short>( );
			for( int j = 0; j < ddselemcount; j++ )
			{
				short ownerid = fh->Read<short>( );
				int x1 = fh->Read<int>( );
				int y1 = fh->Read<int>( );
				int x2 = fh->Read<int>( );
				int y2 = fh->Read<int>( );
				unsigned int color = fh->Read<unsigned int>( );
				std::string name = fh->ReadString( 0x20 );
				Halia::OverlayTexture* ot = new Halia::OverlayTexture( mDdsList[i], x1, y1, x2-x1, y2-y1 );

				Element e = { name, ot };
				mElems.push_back( e );
			}
		}

		Halia::FSMInstance->CloseFile( fh );

		mLoaded = true;
		return true;
	};

	void Unload( )
	{
		
	};

	Halia::OverlayTexture* GetOTexture( std::string name )
	{
		for( std::vector<Element>::iterator i = mElems.begin( ); i != mElems.end( ); i++ )
		{
			if( (*i).name == name )
				return (*i).ot;
		}
		return 0;
	};

protected:
	std::string mPath;
	std::vector<Halia::TextureBase*> mDdsList;
	std::vector<Element> mElems;
};