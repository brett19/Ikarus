#pragma once
#include "Renderable.hpp"
#include "Frustum.hpp"

namespace Halia
{
	class SkeletonNode;

	class Entity : public Renderable
	{
		friend class SceneNode;
	public:
		Entity( )
			: mSorted( false )
		{
		};

		~Entity( )
		{
		};

		virtual Vector3 GetPosition( )
		{
			return mTransform.TransformCoord( Vector3( 0, 0, 0 ) );
		};

		virtual void Update( const Matrix4& parentmat ) = 0;

		virtual void UpdateSkel( const Matrix4& parentmat, SkeletonNode* skel )
		{
			Update( parentmat );
		};

		virtual void Render( RenderSystem* rs ) = 0;

		virtual void QueueRender( RenderQueue* rq ) = 0;

	protected:
		bool mSorted;
	};
};