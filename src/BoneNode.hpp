#pragma once
#include "d3dx9.h"
#include "SceneNode.hpp"

namespace Halia
{
	class BoneNode : public SceneNode
	{
		friend class SkeletonNode;
	public:
		BoneNode( const std::string& name, const Vector3& bindpos, const Quarternion& bindrot )
			: SceneNode( name ), mCurPos( bindpos ), mCurRot( bindrot )
		{
		};

		BoneNode( const Vector3& bindpos, const Quarternion& bindrot )
			: mCurPos( bindpos ), mCurRot( bindrot )
		{
		};

		~BoneNode( )
		{
		};

		void SetBind( BoneNode* parent = 0 )
		{
			mBTransform = Halia::Matrix4::CreateRotation( mCurRot ) * Halia::Matrix4::CreateTranslation( mCurPos );
			if( parent == 0 )
				mGBTransform = mBTransform;
			else
				mGBTransform = mBTransform * parent->mGBTransform;
		};

		void SetOffset( const Vector3& pos )
		{
			mCurPos = pos;
		};

		void SetRotation( const Quarternion& rot )
		{
			mCurRot = rot;
		};

		void Update( const Matrix4& parentmat )
		{
			mTransform = Halia::Matrix4::CreateRotation( mCurRot ) * Halia::Matrix4::CreateTranslation( mCurPos );
			return SceneNode::Update( parentmat );
		};

		Matrix4 GetBoneTransform( )
		{
			return Matrix4::CreateInverse( mGBTransform ) * ( mGTransform * Matrix4::CreateInverse( mSkelParent->GetGTransform( ) ) );
		};

	protected:
		SceneNode* mSkelParent;
		Matrix4 mBTransform;
		Matrix4 mGBTransform;
		Vector3 mCurPos;
		Quarternion mCurRot;
	};
};