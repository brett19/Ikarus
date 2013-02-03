#pragma once
#include "SceneNode.hpp"
#include "BoneNode.hpp"

namespace Halia
{
	class SkeletonNode : public SceneNode
	{
	public:
		virtual void Update( const Matrix4& parentmat )
		{
			mGTransform = mTransform * parentmat;

			for( SLList< SceneNode* >::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
			{
				(*i)->Update( mGTransform );
			}
			for( SLList< Entity* >::Iter i = mEntities.Begin( ); i.IsValid( ); ++i )
			{
				(*i)->UpdateSkel( mGTransform, this );
			}
		};

		void AddBone( BoneNode* bone )
		{
			mBones.push_back( bone );
			bone->mSkelParent = this;
		};

		BoneNode* GetBone( int id )
		{
			return mBones[ id ];
		};

		BoneNode* GetBone( const std::string& name )
		{
			for( std::vector<BoneNode*>::iterator i = mBones.begin( ); i != mBones.end( ); i++ )
			{
				if( (*i)->mName == name )
					return *i;
			}
			return 0;
		};

	private:
		std::vector<BoneNode*> mBones;
	};
};