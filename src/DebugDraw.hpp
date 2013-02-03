#pragma once
#include "RenderSystem.hpp"
#include "BoundingBox.hpp"
#include "Matrix4.hpp"

namespace Halia
{
	struct DebugVert
	{
		Vector3 pos;
		unsigned int color;
	};

	class DebugDraw
	{
	public:
		DebugDraw( );
		~DebugDraw( );

		void DrawBoundingBox( RenderSystem* rs, const BoundingBox& bb, const Matrix4& mat, unsigned int color = 0xFF000000 );

	protected:
		VertexDeclaration* mDecl;
		Material* mMaterial;
	};

	extern DebugDraw* DDInstance;
};