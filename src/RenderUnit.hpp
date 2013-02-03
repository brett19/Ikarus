#pragma once
#include "SkeletonNode.hpp"
#include "Entity.hpp"

namespace Halia
{
	class RenderUnit
	{
	public:
		RenderUnit( )
		{
		};

		~RenderUnit( )
		{
		};

		void Render( RenderSystem* rs )
		{
			
		};

	protected:
		MeshBase* mMesh;
		Material* mMaterial;
	};
};