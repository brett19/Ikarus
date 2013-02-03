#pragma once
#include "SkeletonNode.hpp"
#include "Resource.hpp"

namespace RoseAnimTrackType
{
	enum RoseAnimTrackType
	{
		POSITION = 2,
		ROTATION = 4,
	};
};

class RoseAnimator : public Halia::Resource
{
public:
	struct Channel
	{
		RoseAnimTrackType::RoseAnimTrackType animtype;
		int	boneid;
		char* data;
	};

	RoseAnimator( )
		: mPath( "" ), mChannels( 0 )
	{
	};

	RoseAnimator( const std::string& path )
		: mPath( path ), mChannels( 0 )
	{
	};

	~RoseAnimator( )
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

		fh->Seek( 8, Halia::SeekOrigin::SET );

		mFps = fh->Read<int>( );
		mFrameCount = fh->Read<int>( );
		mChannelCount = fh->Read<int>( );

		mChannels = new Channel[ mChannelCount ];
		for( int i = 0; i < mChannelCount; i++ )
		{
			mChannels[i].animtype = (RoseAnimTrackType::RoseAnimTrackType)fh->Read<int>( );
			mChannels[i].boneid = fh->Read<int>( );
			if( mChannels[i].animtype == RoseAnimTrackType::POSITION )
				mChannels[i].data = (char*)new Halia::Vector3[ mFrameCount ];
			else if( mChannels[i].animtype == RoseAnimTrackType::ROTATION )
				mChannels[i].data = (char*)new Halia::Quarternion[ mFrameCount ];
			else
				mChannels[i].data = 0;
		}

		for( int i = 0; i < mFrameCount; i++ )
		{
			for( int j = 0; j < mChannelCount; j++ )
			{
				if( mChannels[j].animtype == RoseAnimTrackType::POSITION ) {
					((Halia::Vector3*)mChannels[j].data)[i].x = fh->Read<float>( );
					((Halia::Vector3*)mChannels[j].data)[i].y = fh->Read<float>( );
					((Halia::Vector3*)mChannels[j].data)[i].z = fh->Read<float>( );
				} else if( mChannels[j].animtype == RoseAnimTrackType::ROTATION ) {
					((Halia::Quarternion*)mChannels[j].data)[i].w = fh->Read<float>( );
					((Halia::Quarternion*)mChannels[j].data)[i].x = fh->Read<float>( );
					((Halia::Quarternion*)mChannels[j].data)[i].y = fh->Read<float>( );
					((Halia::Quarternion*)mChannels[j].data)[i].z = fh->Read<float>( );
				}
			}
		}

		Halia::FSMInstance->CloseFile( fh );
		mLoaded = true;
		return true;
	};

	void Unload( )
	{
		if( mChannels )
		{
			delete[] mChannels;
			mChannels = 0;
		}
	};

	void ApplyAnimation( Halia::SkeletonNode* skel, float time )
	{
		int frame = (int)( time * (float)mFps );
		frame = frame % mFrameCount;

		for( int i = 0; i < mChannelCount; i++ )
		{
			Halia::BoneNode* bone = skel->GetBone( mChannels[i].boneid );
			if( mChannels[i].animtype == RoseAnimTrackType::POSITION )
				bone->SetOffset( ((Halia::Vector3*)mChannels[i].data)[frame] / 100.0f );
			else if( mChannels[i].animtype == RoseAnimTrackType::ROTATION )
				bone->SetRotation( ((Halia::Quarternion*)mChannels[i].data)[frame] );
		}
	};

	void ApplyAnimation( Halia::Camera* cam, float time )
	{
		int frame = (int)( time * (float)mFps );
		frame = frame % mFrameCount;

		Halia::Vector3 eye = ((Halia::Vector3*)mChannels[0].data)[frame];
		eye = eye / 100.0f + Halia::Vector3( 5200.0f, 5200.0f, 0.0f );
		Halia::Vector3 at = ((Halia::Vector3*)mChannels[1].data)[frame];
		at = at / 100.0f + Halia::Vector3( 5200.0f, 5200.0f, 0.0f );

		cam->SetPosition( eye );
		cam->SetTargetPos( at );
	};

	float GetMaxTime( )
	{
		return (float)( mFrameCount - 1 ) / (float)mFps;
	};

	int GetFrameCount( )
	{
		return mFrameCount;
	};

protected:
	std::string mPath;
	int mFps;
	int mFrameCount;
	int mChannelCount;
	Channel* mChannels;
};