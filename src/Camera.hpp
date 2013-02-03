#pragma once
#include "Entity.hpp"
#include "Radian.hpp"
#include "Frustum.hpp"

namespace Halia
{
	class Camera : public SceneNode
	{
	public:
		Camera( Matrix4 proj )
		{
			mProjection = proj;
		};

		~Camera( )
		{
		};

		void SetPosition( const Vector3& pos )
		{
			mPosition = pos;
		};

		void SetTargetPos( const Vector3& pos )
		{
			mTargetPos = pos;
		};

		void Update( const Matrix4& parentmat )
		{
			mTransform = Halia::Matrix4::CreateLookAtRH( mPosition, mTargetPos, Halia::Vector3( 0, 0, 1 ) );
			mView = Halia::Matrix4::CreateInverse( parentmat ) * mTransform;
			SceneNode::Update( parentmat );
		};

		void Apply( RenderSystem* rs )
		{
			rs->SetProjectionMatrix( mProjection );
			rs->SetViewMatrix( mView );
		};

		Frustum GetFrustum( ) const
		{
			Matrix4 clip = mGTransform * mProjection;
			return Frustum::CreateFromMatrix( clip );
		};

		const Matrix4& GetViewMatrix( ) const
		{
			return mView;
		};

		const Vector3& GetPosition( ) const
		{
			return mPosition;
		};

	private:
		Matrix4 mProjection;
		Matrix4 mView;
		Vector3 mPosition;
		Vector3 mTargetPos;
	};
};