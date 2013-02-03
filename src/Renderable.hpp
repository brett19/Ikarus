#pragma once
#include "RenderSystem.hpp"
#include "SLList.hpp"
#include "Plane.hpp"
#include "Entity.hpp"
#include "BoundingBox.hpp"
#include "BoundingSphere.hpp"
#include "RenderQueue.hpp"

namespace Halia
{
	class Renderable
	{
	public:
		Renderable( )
			: mTransform( Matrix4::IDENTITY ), mEnabled( true ), mCulled( false ), mBoundingMode( 0 )
		{
		};

		~Renderable( )
		{
		};

		void SetEnabled( bool enabled = true )
		{
			mEnabled = enabled;
		};

		void SetTransform( const Matrix4& transform )
		{
			mTransform = transform;
		};

		void SetBoundingBox( const BoundingBox& box )
		{
			mBoundingBox = box;
			mBoundingMode = 1;
		};

		void SetBoundingSphere( const BoundingSphere& sphere )
		{
			mBoundingSphere = sphere;
			mBoundingMode = 2;
		};

		virtual const Matrix4& GetTransform( ) const
		{
			return mTransform;
		};

		int GetBoundingType( )
		{
			return mBoundingMode;
		};

		const BoundingBox& GetBoundingBox( )
		{
			return mBoundingBox;
		};

		const BoundingSphere& GetBoundingSphere( )
		{
			return mBoundingSphere;
		};

		bool CheckCulling( const Frustum& camf, const Matrix4& mat  ) const
		{
			if( mBoundingMode == 1 )
				return mBoundingBox.CheckOutsideFrustum( camf, mat );
			else if( mBoundingMode == 2 )
				return mBoundingSphere.CheckOutsideFrustum( camf, mat );
			else
				return false;
		};

		virtual void Update( const Matrix4& parentmat ) = 0;

		virtual void UpdateCulling( const Frustum& camf, const Matrix4& mat ) = 0;

		virtual void QueueRender( RenderQueue* rq ) = 0;

		virtual void Render( RenderSystem* rs ) = 0;

	protected:
		Matrix4 mTransform;
		bool mEnabled;
		bool mCulled;
		int mBoundingMode;
		BoundingBox mBoundingBox;
		BoundingSphere mBoundingSphere;
	};
};