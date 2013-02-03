#pragma once
#include "SceneNode.hpp"

namespace Halia
{
	class SceneTree : public SceneNode
	{
	public:
		SceneTree( )
			: SceneNode( "_ROOT" )
		{
		};

		~SceneTree( )
		{
		};

		void Update( )
		{
			return SceneNode::Update( Matrix4::IDENTITY );
		};

		void Render( RenderSystem* rs, Halia::Camera* campos )
		{
			Vector3 cpos = campos->GetPosition( );
			mRQueue.SetCameraPos( cpos );
			SceneNode::QueueRender( &mRQueue );
			mRQueue.Render( rs );
			mRQueue.Clear( );
			return;
		};

	protected:
		RenderQueue mRQueue;
	};
};