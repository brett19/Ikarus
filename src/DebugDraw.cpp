#include "DebugDraw.hpp"
#include "LogManager.hpp"

namespace Halia
{
	DebugDraw::DebugDraw( )
	{
		mDecl = new VertexDeclaration( );
		mDecl->AddElement( VertexElement( 0, VDeclSemantic::POSITION, VDeclType::FLOAT3, 0 ) );
		mDecl->AddElement( VertexElement( 0, VDeclSemantic::COLOUR, VDeclType::COLOUR, 0 ) );

		mMaterial = new Material( );
		mMaterial->AddTextureStage(
			TextureStage(
				0,
				BlendMode( BlendOp::SELECTARGUMENT1, BlendArg::DIFFUSE )
			)
		);
		mMaterial->mTwoSided = true;
		mMaterial->mAlphaBlend = false;
		mMaterial->mAlphaTest = false;
		mMaterial->mDepthWrite = true;
		mMaterial->mDepthBuffer = true;
		mMaterial->mSpecularEnabled = false;
	};

	DebugDraw::~DebugDraw( )
	{
		delete mMaterial;
	};

	void DebugDraw::DrawBoundingBox( RenderSystem* rs, const BoundingBox& bb, const Matrix4& mat, unsigned int color )
	{
		DebugVert v[] = 
		{
			{ Vector3( bb.min.x, bb.min.y, bb.min.z ), color },
			{ Vector3( bb.min.x, bb.max.y, bb.min.z ), color },
			{ Vector3( bb.max.x, bb.min.y, bb.min.z ), color },
			{ Vector3( bb.min.x, bb.min.y, bb.max.z ), color },
			{ Vector3( bb.max.x, bb.max.y, bb.min.z ), color },
			{ Vector3( bb.max.x, bb.min.y, bb.max.z ), color },
			{ Vector3( bb.min.x, bb.max.y, bb.max.z ), color },
			{ Vector3( bb.max.x, bb.max.y, bb.max.z ), color }
		};

		short i[] = {
			1, 4, 0, 0, 4, 2, // FRONT
			0, 3, 2, 2, 3, 5, // BOTTOM
			1, 6, 4, 4, 6, 7, // TOP
			1, 6, 0, 0, 6, 3, // LEFT
			4, 7, 2, 2, 7, 5, // RIGHT
			6, 7, 3, 3, 7, 5  // BACK
		};

		rs->SetWorldMatrix( mat );
		rs->SetCullMode( CullMode::NONE );
		rs->SetFillMode( FillMode::WIRE );
		rs->SetMaterial( mMaterial );
		rs->SetVertexDeclaration( mDecl );
		rs->DrawIndexedPrimitiveUP( PrimitiveType::TriangleList, 0, 0, 8, 6*2, v, sizeof( DebugVert ), i, 2 );
		rs->SetFillMode( FillMode::SOLID );
	};

	DebugDraw* DDInstance = new DebugDraw( );
};