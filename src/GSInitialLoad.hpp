#pragma once

class GSInitialLoad
{
public:
	GSInitialLoad( )
	{
	};

	~GSInitialLoad( )
	{
	};

	bool Init( )
	{
		/*
		mImageTex = new Halia::TextureFile( "3DDATA\\CONTROL\\RES\\EXUI1.DDS" );
		mImageTex->Load( );
		*/

		return true;
	};

	bool IsReady( )
	{
		return true;
	};

	bool Enter( )
	{
		/*
		Halia::OverlayTexture* ot = new Halia::OverlayTexture( mImageTex, 0, 0, mImageTex->GetWidth( ), mImageTex->GetHeight( ) );
		mImage = new Halia::OverlayImage( ot, 0, 0, mImageTex->GetWidth( ), mImageTex->GetHeight( ) );
		HOVERLAYER->CenterChild( mImage );
		HOVERLAYER->AddChild( mImage );
		*/

		return true;
	};

	bool Leave( )
	{
		//mImage->SetVisible( false );

		return true;
	};

	bool PreFrameEvent( )
	{
		return true;
	};

	bool FrameEvent( )
	{
		return true;
	};

	bool EventHandler( Halia::Event* evt )
	{
		return true;
	};

protected:
	//Halia::TextureFile* mImageTex;
	//Halia::OverlayImage* mImage;
};