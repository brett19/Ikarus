#pragma once
#include "SceneNode.hpp"

class RoseSkyBox
{
public:
	RoseSkyBox( Halia::IndexedMeshMirror* mesh )
	{
		mEntity = new Halia::MeshEntity( mesh );
	};

	~RoseSkyBox( )
	{
	};

	virtual void Render( Halia::RenderSystem* rs, Halia::Camera* cam )
	{
		Halia::Matrix4 cammat = cam->GetGTransform( );
		Halia::Matrix4 mat = cammat;
		mat.m[3][0] = 0.0f;
		mat.m[3][1] = 0.0f;
		mat.m[3][2] = 0.0f;
		mat *= Halia::Matrix4::CreateScaling( 2.0f );

		cam->Apply( rs );
		rs->SetViewMatrix( Halia::Matrix4::IDENTITY );
		mEntity->SetTransform( mat );
		mEntity->Render( rs );
	};

private:
	Halia::MeshEntity* mEntity;
};