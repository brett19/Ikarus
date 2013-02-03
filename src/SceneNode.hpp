#pragma once
#include "SLList.hpp"
#include "Plane.hpp"
#include "Entity.hpp"
#include "BoundingBox.hpp"
#include "BoundingSphere.hpp"
#include "Renderable.hpp"
#include "DebugDraw.hpp"

namespace Halia
{
	class SceneNode : public Renderable
	{
		friend class Camera;
	public:
		SceneNode( const std::string& name )
			: mName( name )
		{
		};

		SceneNode( )
			: mName( "" )
		{
		};

		void AddChild( SceneNode* node )
		{
			mChildren.PushBack( node );
		};

		void AddEntity( Entity* entity )
		{
			mEntities.PushBack( entity );
		};

		void GenBoundingBox( )
		{
			mBoundingMode = 1;
			mBoundingBox = BoundingBox( );

			for( SLList< SceneNode* >::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
			{
				if( (*i)->GetBoundingType( ) == 1 )
					mBoundingBox.AddTransformedBB( (*i)->GetBoundingBox( ), (*i)->GetTransform( ) );
			}
			for( SLList< Entity* >::Iter i = mEntities.Begin( ); i.IsValid( ); ++i )
			{
				if( (*i)->GetBoundingType( ) == 1 )
					mBoundingBox.AddTransformedBB( (*i)->GetBoundingBox( ), Matrix4::IDENTITY );
			}
		};

		virtual void Update( const Matrix4& parentmat )
		{
			mGTransform = mTransform * parentmat;

			for( SLList< SceneNode* >::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
			{
				(*i)->Update( mGTransform );
			}
			for( SLList< Entity* >::Iter i = mEntities.Begin( ); i.IsValid( ); ++i )
			{
				(*i)->Update( mGTransform );
			}
		};

		virtual void UpdateCulling( const Frustum& camf, const Matrix4& mat )
		{
			mCulled = CheckCulling( camf, mGTransform );
			if( mCulled )
				return;

			for( SLList< SceneNode* >::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
				(*i)->UpdateCulling( camf, mGTransform );
			for( SLList< Entity* >::Iter i = mEntities.Begin( ); i.IsValid( ); ++i )
				(*i)->UpdateCulling( camf, mGTransform );
		};

		virtual void QueueRender( RenderQueue* rq )
		{
			if( !mEnabled || mCulled )
				return;

			for( SLList< SceneNode* >::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
			{
				(*i)->QueueRender( rq );
			}
			for( SLList< Entity* >::Iter i = mEntities.Begin( ); i.IsValid( ); ++i )
			{
				(*i)->QueueRender( rq );
			}
		};

		virtual void Render( RenderSystem* rs )
		{
			/*
			if( mBoundingMode == 1 )
				DDInstance->DrawBoundingBox( rs, mBoundingBox, mGTransform, 0xFF00FF00 );
			//*/
			//DDInstance->DrawBoundingBox( rs, BoundingBox( Vector3( -0.01, -0.01, -0.01 ), Vector3( 0.01, 0.01, 0.01 ) ), mGTransform, 0xFFFF0000 );

			for( SLList< SceneNode* >::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
			{
				(*i)->Render( rs );
			}
			for( SLList< Entity* >::Iter i = mEntities.Begin( ); i.IsValid( ); ++i )
			{
				(*i)->Render( rs );
			}
		};

		const Matrix4& GetGTransform( )
		{
			return mGTransform;
		};

	protected:
		std::string mName;
		Matrix4 mGTransform;
		
		SLList< SceneNode* > mChildren;
		SLList< Entity* > mEntities;
	};
};