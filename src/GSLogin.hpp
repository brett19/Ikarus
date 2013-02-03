#pragma once
#include "RoseGfxList.hpp"
#include "RoseMeshList.hpp"
#include "RoseMapLoader.hpp"
#include "RoseLoginWindow.hpp"
#include "RoseSrvChnWindow.hpp"
#include "RoseDialog.hpp"
#include "RoseAnimator.hpp"
#include "SceneTree.hpp"
#include "Camera.hpp"
#include "IkarusData.hpp"
#include "RosePlayerNode.hpp"
#include "RoseSkyBox.hpp"

#pragma once

class GSLogin
{
public:
	GSLogin( )
	{
	};

	~GSLogin( )
	{
	};

	bool Init( )
	{
	};

	bool Enter( )
	{
	};

	bool Leave( )
	{
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

};

/*
class GSLogin
{
public:
	GSLogin( )
	{
	};

	~GSLogin( )
	{
	};

	bool Init( )
	{
		mResGroup = new Halia::ResourceGroup( );

		///*

		IKDATA->skelmale = new RoseSkeletonLoader( "3DDATA\\AVATAR\\MALE.ZMD" );
		mResGroup->AddResource( IKDATA->skelmale );
		IKDATA->skelfemale = new RoseSkeletonLoader( "3DDATA\\AVATAR\\FEMALE.ZMD" );
		mResGroup->AddResource( IKDATA->skelfemale );

		for( int i = 0; i < 2; i++ )
		{
			for( int j = 0; j < 16; j++ )
			{
				IKDATA->charml[i][j] = 0;
			}
		}

		IKDATA->charml[ 0 ][ RoseItemType::MASK ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_FACEITEM.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::CAP ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_MCAP.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::BODY ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_MBODY.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::ARM ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_MARMS.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::FOOT ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_MFOOT.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::BACK ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_BACK.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::WEAPON ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_WEAPON.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::SUBWPN ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_SUBWPN.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::PAT ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_PAT.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::FACE ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_MFACE.ZSC" );
		IKDATA->charml[ 0 ][ RoseItemType::HAIR ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_MHAIR.ZSC" );

		IKDATA->charml[ 1 ][ RoseItemType::MASK ] = IKDATA->charml[ 0 ][ RoseItemType::MASK ];
		IKDATA->charml[ 1 ][ RoseItemType::CAP ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_WCAP.ZSC" );
		IKDATA->charml[ 1 ][ RoseItemType::BODY ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_WBODY.ZSC" );
		IKDATA->charml[ 1 ][ RoseItemType::ARM ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_WARMS.ZSC" );
		IKDATA->charml[ 1 ][ RoseItemType::FOOT ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_WFOOT.ZSC" );
		IKDATA->charml[ 1 ][ RoseItemType::BACK ] = IKDATA->charml[ 0 ][ RoseItemType::BACK ];
		IKDATA->charml[ 1 ][ RoseItemType::WEAPON ] = IKDATA->charml[ 0 ][ RoseItemType::WEAPON ];
		IKDATA->charml[ 1 ][ RoseItemType::SUBWPN ] = IKDATA->charml[ 0 ][ RoseItemType::SUBWPN ];
		IKDATA->charml[ 1 ][ RoseItemType::PAT ] = IKDATA->charml[ 0 ][ RoseItemType::PAT ];
		IKDATA->charml[ 1 ][ RoseItemType::FACE ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_WFACE.ZSC" );
		IKDATA->charml[ 1 ][ RoseItemType::HAIR ] = new RoseMeshList( "3DDATA\\AVATAR\\LIST_WHAIR.ZSC" );

		for( int i = 0; i < 2; i++ )
		{
			for( int j = 0; j < 16; j++ )
			{
				if( IKDATA->charml[i][j] != 0 )
					mResGroup->AddResource( IKDATA->charml[i][j] );
			}
		}

		///

		RoseMeshList* cnst = new RoseMeshList( "3DDATA\\JUNON\\LIST_CNST_JPT.ZSC" );
		RoseMeshList* deco = new RoseMeshList( "3DDATA\\JUNON\\LIST_DECO_JPT.ZSC" );
		mMap = new RoseMapLoader( "3DDATA\\MAPS\\JUNON\\", "TITLE_JPT", cnst, deco, 32, 31 );
		mCamAnim1 = new RoseAnimator( "3DDATA\\TITLE\\CAMERA01_INTRO01.ZMO" );
		mCamAnim2 = new RoseAnimator( "3DDATA\\TITLE\\CAMERA01_INSELECT01.ZMO" );

		mResGroup->AddResource( cnst );
		mResGroup->AddResource( deco );
		mResGroup->AddResource( mMap );
		mResGroup->AddResource( mCamAnim1 );
		mResGroup->AddResource( mCamAnim2 );

		mGfxList = new RoseGfxList( "3DDATA\\CONTROL\\RES\\EXUI.TSI" );
		mResGroup->AddResource( mGfxList );
		
		mAvatarAnim1 = new RoseAnimator( "3DDATA\\MOTION\\AVATAR\\EMPTY_STOP1_M1.ZMO" );
		mResGroup->AddResource( mAvatarAnim1 );
	
		mSkyMesh = new RoseMeshLoader( "3DDATA\\JUNON\\SKY\\DAY01.ZMS" );
		mSkyTex = new Halia::TextureFile( "3DDATA\\JUNON\\SKY\\DAY01.DDS" );
		mResGroup->AddResource( mSkyMesh );
		mResGroup->AddResource( mSkyTex );

		HRESMGR->QueueResource( mResGroup );
		mHoldTime = GetTickCount( );

		return true;
	};

	bool IsReady( )
	{
		//if( GetTickCount( ) - mHoldTime < 750 )
		//	return false;
		return mResGroup->IsLoaded( );
	};

	bool Enter( )
	{
		mLoginWnd = new RoseLoginWindow( 100, 100, mGfxList );
		mLoginWnd->SetVisible( true );
		HOVERLAYER->CenterChild( mLoginWnd );
		mLoginWnd->SetPosition(
			HOVERLAYER->GetSize( ).x - mLoginWnd->GetSize( ).x - 100,
			mLoginWnd->GetPosition( ).y - 100
		);
		mLoginWnd->SetUsername( "Brett193" );
		mLoginWnd->SetPassword( "d78c480a9f1f12704911bff43f6888ab" );
		HOVERLAYER->AddChild( mLoginWnd );

		mSrvChnWnd = new RoseSrvChnWindow( 100, 100, mGfxList );
		mSrvChnWnd->SetVisible( false );
		HOVERLAYER->CenterChild( mSrvChnWnd );
		HOVERLAYER->AddChild( mSrvChnWnd );

		mDialog = new RoseDialog( 100, 100, mGfxList );
		mDialog->SetVisible( false );
		HOVERLAYER->CenterChild( mDialog );
		HOVERLAYER->AddChild( mDialog );

		mScene = new Halia::SceneTree( );
		mScene->AddChild( mMap->CreateNode( ) );

		mCamera = new Halia::Camera( Halia::Matrix4::CreatePerspectiveFovRH( 40.75f*(Halia::Math::PI/180.0f), 1024.0f / 768.0f, 0.1f, 320.0f ) );
		mScene->AddChild( mCamera );

		Halia::Material* mat = new Halia::Material( );
		mat->AddTextureStage(
			Halia::TextureStage(
				mSkyTex,
				Halia::BlendMode( Halia::BlendOp::SELECTARGUMENT1, Halia::BlendArg::TEXTURE )
			)
		);
		Halia::IndexedMeshMirror* mesh = new Halia::IndexedMeshMirror( *mSkyMesh->GetMesh( ) );
		mesh->SetMaterial( mat );
		mSky = new RoseSkyBox( mesh );

		mCamStart = GetTickCount( );
		mCamAnim = mCamAnim1;

		return true;
	};

	bool Leave( )
	{

		return true;
	};

	bool PreFrameEvent( )
	{
		/*
		mCamAnim = mCamAnim2;
		mCamAnim->ApplyAnimation( mCamera, mCamAnim->GetMaxTime( ) );
		//*/
		/*
		mCamAnim = mCamAnim;
		mCamAnim->ApplyAnimation( mCamera, (float)( GetTickCount( ) - mCamStart ) / 1000.0f );
		///
		///*
		if( mCamAnim == mCamAnim2 ) {
			if( (float)( GetTickCount( ) - mCamStart ) / 1000.0f >= mCamAnim->GetMaxTime( ) )
				mCamAnim->ApplyAnimation( mCamera, mCamAnim->GetMaxTime( ) );
			else
				mCamAnim->ApplyAnimation( mCamera, (float)( GetTickCount( ) - mCamStart ) / 1000.0f );
		} else {
			mCamAnim->ApplyAnimation( mCamera, (float)( GetTickCount( ) - mCamStart ) / 1000.0f );
		}
		///

		for( std::vector<RosePlayerNode*>::iterator i = mAvatars.begin( ); i != mAvatars.end( ); ++i )
		{
			mAvatarAnim1->ApplyAnimation( (*i)->GetSkel( ), (float)( GetTickCount( ) - mCamStart ) / 1000.0f );
		}

		mScene->Update( );
		
		Halia::Frustum camf = mCamera->GetFrustum( );
		mScene->UpdateCulling( camf, Halia::Matrix4::IDENTITY );

		return true;
	};

	bool FrameEvent( )
	{
		HRENDERER->SetCullMode( Halia::CullMode::NONE );
		HRENDERER->SetFillMode( Halia::FillMode::SOLID );

		mSky->Render( HRENDERER, mCamera );
		mCamera->Apply( HRENDERER );
		mScene->Render( HRENDERER, mCamera );

		return true;
	};

	bool EventHandler( Halia::EventBase* evt )
	{
		if( evt->GetType( ) == RoseEventType::NET_CONNECTED )
		{
			NetworkEvent* revt = (NetworkEvent*)evt;
			if( revt->sockid == RoseSockType::LOGIN ) {
				PakCLInitEvent* nevt = new PakCLInitEvent( );
				IKNET->PushEvent( nevt );
			}
			if( revt->sockid == RoseSockType::CHAR ) {
				PakCCInitEvent* nevt = new PakCCInitEvent( );
				nevt->id = IKDATA->charid;
				nevt->password = mLoginWnd->GetPassword( );
				IKNET->PushEvent( nevt );
			}
			/*
			if( revt->sockid == RoseSockType::WORLD ) {
				PakCWInitEvent* nevt = new PakCWInitEvent( );
				IKNET->PushEvent( nevt );
			}
			///
		}
		if( evt->GetType( ) == RoseEventType::PAKSL_INIT )
		{
			PakCLLoginEvent* nevt = new PakCLLoginEvent( );
			nevt->username = mLoginWnd->GetUsername( );
			nevt->password = mLoginWnd->GetPassword( );
			IKNET->PushEvent( nevt );
		}
		if( evt->GetType( ) == RoseEventType::PAKSL_LOGIN )
		{
			PakSLLoginEvent* revt = (PakSLLoginEvent*)evt;
			if( revt->retcode == 0 && revt->servers.size( ) > 0 )
			{
				for( int i = 0; i < (int)revt->servers.size( ); ++i )
				{
					printf( "Server %d - %s\n", revt->servers[i].id, revt->servers[i].name.c_str( ) );
				}
				IKDATA->worldnum = revt->servers[0].id;

				PakCLSrvSelEvent* nevt = new PakCLSrvSelEvent( );
				nevt->serverid = IKDATA->worldnum;
				IKNET->PushEvent( nevt );
			} else {
				// Bad login
			}
		}
		if( evt->GetType( ) == RoseEventType::PAKSL_CHANNELLIST )
		{
			PakSLChnListEvent* revt = (PakSLChnListEvent*)evt;
			if( revt->channels.size( ) > 0 )
			{
				for( int i = 0; i < (int)revt->channels.size( ); ++i )
				{
					printf( "Channel %d - %s\n", revt->channels[i].id, revt->channels[i].name.c_str( ) );
				}
				IKDATA->channum = revt->channels[0].id;

				PakCLChnSelEvent* nevt = new PakCLChnSelEvent( );
				nevt->serverid = IKDATA->worldnum;
				nevt->channelid = IKDATA->channum;
				IKNET->PushEvent( nevt );
			}
		}
		if( evt->GetType( ) == RoseEventType::PAKSL_CHARINFO )
		{
			PakSLCharInfoEvent* revt = (PakSLCharInfoEvent*)evt;
			printf( "Char Info: [%d] %08x %08x (%s:%d)\n", revt->retcode, revt->id1, revt->id2, revt->host.c_str( ), revt->port );
		
			IKDATA->charid = revt->id1;

			IKNET->SetCharInfo( revt->host, revt->port, revt->id2 );
			NetworkEvent* nevt = new NetworkEvent( RoseEventType::NET_CONNECT, RoseSockType::CHAR );
			IKNET->PushEvent( nevt );
		}
		if( evt->GetType( ) == RoseEventType::PAKSC_INIT )
		{
			PakSCInitEvent* revt = (PakSCInitEvent*)evt;
			
			PakCCGetCharsEvent* nevt = new PakCCGetCharsEvent( );
			IKNET->PushEvent( nevt );
		}
		if( evt->GetType( ) == RoseEventType::PAKSC_CHARACTERLIST )
		{
			PakSCCharListEvent* revt = (PakSCCharListEvent*)evt;

			float pos[5][3] = {
				{ 5205.00f, 5205.00f, 1.0f },
				{ 5202.70f, 5206.53f, 1.0f },
				{ 5200.00f, 5207.07f, 1.0f },
				{ 5197.30f, 5206.53f, 1.0f },
				{ 5195.00f, 5205.00f, 1.0f }
			};

			int k = 0;
			for( std::vector<PakSCCharListEvent::CharEntry>::iterator i = revt->characters.begin( ); i != revt->characters.end( ); ++i )
			{	
				RosePlayerNode* avt = new RosePlayerNode( );
				avt->Init(
					(*i).race,
					(*i).items[0].itemno,
					(*i).items[1].itemno,
					(*i).items[2].itemno,
					(*i).items[3].itemno,
					(*i).items[4].itemno,
					(*i).items[5].itemno,
					(*i).items[6].itemno,
					(*i).items[7].itemno,
					(*i).items[8].itemno,
					(*i).items[9].itemno
				);
				avt->SetTransform(
					Halia::Matrix4::CreateRotation( 0, 0, Halia::Math::PI ) *
					Halia::Matrix4::CreateScaling( 1.5f ) *
					Halia::Matrix4::CreateTranslation( Halia::Vector3( pos[k][0], pos[k][1], pos[k][2] ) ) 
				);
				mAvatars.push_back( avt );
				mScene->AddChild( avt );

				k++;
			}

			mCamStart = GetTickCount( );
			mCamAnim = mCamAnim2;
		}

		if( evt->GetType( ) == RoseEventType::LOGINOKAYCLICK )
		{
			NetworkEvent* nevt = new NetworkEvent( RoseEventType::NET_CONNECT, RoseSockType::LOGIN );
			IKNET->PushEvent( nevt );
			mLoginWnd->SetVisible( false );
		}
		if( evt->GetType( ) == RoseEventType::LOGINCANCELCLICK )
		{
			HENGINE->Quit( );
		}

		return false;
	};

protected:
	unsigned int mHoldTime;

	RoseGfxList* mGfxList;
	Halia::ResourceGroup* mResGroup;
	RoseLoginWindow* mLoginWnd;
	RoseSrvChnWindow* mSrvChnWnd;
	RoseDialog* mDialog;
	RoseMapLoader* mMap;
	RoseMeshLoader* mSkyMesh;
	Halia::TextureFile* mSkyTex;
	RoseSkyBox* mSky;
	unsigned int mCamStart;
	RoseAnimator* mCamAnim;
	RoseAnimator* mCamAnim1;
	RoseAnimator* mCamAnim2;
	Halia::Camera* mCamera;
	Halia::SceneTree* mScene;
	std::vector<RosePlayerNode*> mAvatars;
	RoseAnimator* mAvatarAnim1;
};
*/