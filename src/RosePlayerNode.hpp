#pragma once
#include "SceneNode.hpp"
#include "RoseMeshList.hpp"

class RosePlayerNode : public Halia::SceneNode
{
public:
	RosePlayerNode( )
	{
	};

	~RosePlayerNode( )
	{
	};

	bool Init( int gender, int face, int hair, int cap, int body, int arm, int foot, int faceitem, int back, int wpn, int subwpn )
	{
		mGender = gender;

		if( mGender == 1 )
			mSkel = IKDATA->skelfemale->CreateNode( );
		else
			mSkel = IKDATA->skelmale->CreateNode( );
		AddChild( mSkel );

		mBody = IKDATA->charml[ mGender ][ RoseItemType::BODY ]->CreateNode( body );
		mSkel->AddEntity( mBody );

		mFoot = IKDATA->charml[ mGender ][ RoseItemType::FOOT ]->CreateNode( foot );
		mSkel->AddEntity( mFoot );

		mArm = IKDATA->charml[ mGender ][ RoseItemType::ARM ]->CreateNode( arm );
		mSkel->AddEntity( mArm );

		mHair = IKDATA->charml[ mGender ][ RoseItemType::HAIR ]->CreateNode( hair );
		mSkel->GetBone( "b1_head" )->AddEntity( mHair );

		mFace = IKDATA->charml[ mGender ][ RoseItemType::FACE ]->CreateNode( face );
		mSkel->GetBone( "b1_head" )->AddEntity( mFace );

		return true;
	};

	Halia::SkeletonNode* GetSkel( )
	{
		return mSkel;
	};

private:
	int mGender;
	Halia::SkeletonNode* mSkel;
	RoseObjectEntity* mBody;
	RoseObjectEntity* mFoot;
	RoseObjectEntity* mArm;
	RoseObjectEntity* mHair;
	RoseObjectEntity* mFace;
	RoseObjectEntity* mWing;
};