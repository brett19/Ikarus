#pragma once
#include "Entity.hpp"

namespace Halia
{
	class EntityGroup : public Entity
	{
	public:
		class SubEntity
		{
		public:
			SubEntity( Entity* ent )
				: mEntity( ent )
			{
			};

			SubEntity( Entity* ent, Matrix4 mat )
				: mEntity( ent ), mTransform( mat )
			{
			};

			~SubEntity( )
			{
			};

			Entity* GetEntity( ) const
			{
				return mEntity;
			};

			const Matrix4& GetTransform( ) const
			{
				return mTransform;
			};

		protected:
			Entity* mEntity;
			Matrix4 mTransform;
		};

		EntityGroup( )
		{
		};

		~EntityGroup( )
		{
		};

		void AddChild( Entity* ent )
		{
			mChildren.PushBack( SubEntity( ent ) );
		};

		void AddChild( Entity* ent, const Matrix4& mat )
		{
			mChildren.PushBack( SubEntity( ent, mat ) );
		};

		virtual void Update( const Matrix4& parentmat )
		{
			mTransform = parentmat;

			for( SLList<SubEntity>::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
			{
				(*i).GetEntity( )->Update( (*i).GetTransform( ) * parentmat );
			}
		};

		virtual void UpdateSkel( const Matrix4& parentmat, SkeletonNode* skel )
		{
			mTransform = parentmat;

			for( SLList<SubEntity>::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
			{
				(*i).GetEntity( )->UpdateSkel( (*i).GetTransform( ) * parentmat, skel );
			}
		};

		virtual void UpdateCulling( const Frustum& camf, const Matrix4& mat )
		{
			mCulled = CheckCulling( camf, mat );
			if( mCulled )
				return;

			for( SLList<SubEntity>::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
			{
				(*i).GetEntity( )->UpdateCulling( camf, (*i).GetTransform( ) * mat );
			}
		};

		virtual void QueueRender( RenderQueue* rq )
		{
			if( !mEnabled || mCulled )
				return;

			for( SLList<SubEntity>::Iter i = mChildren.Begin( ); i.IsValid( ); ++i )
			{
				(*i).GetEntity( )->QueueRender( rq );
			}
		};

		virtual void Render( RenderSystem* rs )
		{
			/*
				We should NEVER get here, as everything should be queued first. Kthx
			*/
			__asm { INT 3 }
		};

	protected:
		SLList< SubEntity > mChildren;
	};
};