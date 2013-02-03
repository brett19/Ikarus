#pragma once
#include "SkeletonNode.hpp"

class RoseSkeletonLoader : public Halia::Resource
{
public:
	struct Bone
	{
		int parent;
		std::string name;
		Halia::Vector3 offset;
		Halia::Quarternion rot;
	};

	RoseSkeletonLoader( )
		: mPath( "" ), mBones( 0 ), mAttachBones( 0 )
	{
	};

	RoseSkeletonLoader( const std::string& path )
		: mPath( path ), mBones( 0 ), mAttachBones( 0 )
	{
	};

	~RoseSkeletonLoader( )
	{
		Unload( );
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

		fh->Seek( 7, Halia::SeekOrigin::SET );
		
		mBoneCount = fh->Read<int>( );
		mBones = new Bone[ mBoneCount ];
		for( int i = 0; i < mBoneCount; i++ )
		{
			mBones[i].parent = fh->Read<int>( );
			mBones[i].name = fh->ReadString( Halia::StringType::NULLTERM );
			mBones[i].offset.x = fh->Read<float>( );
			mBones[i].offset.y = fh->Read<float>( );
			mBones[i].offset.z = fh->Read<float>( );
			mBones[i].offset /= 100.0f;
			mBones[i].rot.w = fh->Read<float>( );
			mBones[i].rot.x = fh->Read<float>( );
			mBones[i].rot.y = fh->Read<float>( );
			mBones[i].rot.z = fh->Read<float>( );
		}

		mAttachCount = fh->Read<int>( );
		mAttachBones = new Bone[ mAttachCount ];
		for( int i = 0; i < mAttachCount; i++ )
		{
			mAttachBones[i].name = fh->ReadString( Halia::StringType::NULLTERM );
			mAttachBones[i].parent = fh->Read<int>( );
			mAttachBones[i].offset.x = fh->Read<float>( );
			mAttachBones[i].offset.y = fh->Read<float>( );
			mAttachBones[i].offset.z = fh->Read<float>( );
			mAttachBones[i].offset /= 100.0f;
			mAttachBones[i].rot.w = fh->Read<float>( );
			mAttachBones[i].rot.x = fh->Read<float>( );
			mAttachBones[i].rot.y = fh->Read<float>( );
			mAttachBones[i].rot.z = fh->Read<float>( );
		}

		Halia::FSMInstance->CloseFile( fh );
		mLoaded = true;
		return true;
	};

	void Unload( )
	{
		mBoneCount = 0;
		if( mBones ) {
			delete[] mBones;
			mBones = 0;
		}
		mAttachCount = 0;
		if( mAttachBones ) {
			delete[] mAttachBones;
			mAttachBones = 0;
		}
	};

	Halia::SkeletonNode* CreateNode( )
	{
		Halia::SkeletonNode* sn = new Halia::SkeletonNode( );

		for( int i = 0; i < mBoneCount; i++ )
		{
			Halia::BoneNode* bone = new Halia::BoneNode( mBones[i].name, mBones[i].offset, mBones[i].rot );
			sn->AddBone( bone );
			if( i != 0 ) {
				Halia::BoneNode* parent = sn->GetBone( mBones[i].parent );
				parent->AddChild( bone );
				bone->SetBind( parent );
			} else {
				sn->AddChild( bone );
				bone->SetBind( );
			}
		}

		for( int i = 0; i < mAttachCount; i++ )
		{
			Halia::BoneNode* bone = new Halia::BoneNode( mAttachBones[i].name, mAttachBones[i].offset, mAttachBones[i].rot );
			sn->AddBone( bone );
			if( i != 0 ) {
				Halia::BoneNode* parent = sn->GetBone( mAttachBones[i].parent );
				parent->AddChild( bone );
				bone->SetBind( parent );
			} else {
				sn->AddChild( bone );
				bone->SetBind( );
			}
		}

		return sn;
	};

private:
	std::string mPath;
	int mBoneCount;
	Bone* mBones;
	int mAttachCount;
	Bone* mAttachBones;
};