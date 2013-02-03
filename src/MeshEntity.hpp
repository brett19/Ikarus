#pragma once
#include "SkeletonNode.hpp"
#include "Entity.hpp"
#include "DebugDraw.hpp"

namespace Halia
{
	class MeshEntity : public Entity
	{
	public:
		MeshEntity( MeshBase* mesh )
		{
			mBaseState = mesh;
			mCurState = 0;
		};

		~MeshEntity( )
		{
		};

		virtual void Update( const Matrix4& parentmat )
		{
			mTransform = parentmat;
		};

		virtual void UpdateSkel( const Matrix4& parentmat, SkeletonNode* skel = 0 )
		{
			mTransform = parentmat;

			if( mCurState == 0 || mCurState->GetBufferSize( ) < mBaseState->GetVertexBuffer( )->GetBufferSize( ) )
			{
				if( mCurState )
				{
					delete mCurState;
					mCurState = 0;
				}

				mCurState = new VertexBufferMirror( mBaseState->GetVertexBuffer( ) );

				if( mCurState )
				{
					memcpy( mCurState->GetBuffer( ), mBaseState->GetVertexBuffer( )->GetBuffer( ), mBaseState->GetVertexBuffer( )->GetBufferSize( ) );
				}
			}

 			if( mCurState )
			{
				if( mBaseState->GetVertexDeclaration( )->IsSupported( 0, VDeclSemantic::BLENDWEIGHT ) &&
					mBaseState->GetVertexDeclaration( )->IsSupported( 0, VDeclSemantic::BLENDINDEX ) )
				{
					char* vbufferin = mBaseState->GetVertexBuffer( )->GetBuffer( );
					char* vbuffer = mCurState->GetBuffer( );
					int stride = mBaseState->GetVertexDeclaration( )->GetStride( 0 );
					int posoffset = mBaseState->GetVertexDeclaration( )->GetElementOffset( VDeclSemantic::POSITION, 0 );
					int bwoffset = mBaseState->GetVertexDeclaration( )->GetElementOffset( VDeclSemantic::BLENDWEIGHT, 0 );
					int bioffset = mBaseState->GetVertexDeclaration( )->GetElementOffset( VDeclSemantic::BLENDINDEX, 0 );

					for( int i = 0; i < mBaseState->GetVertexBuffer( )->GetLength( ); i++ )
					{
						char* vertbuf = vbuffer + ( i * stride );
						char* vertbufin = vbufferin + ( i * stride );
						Vector3* pos = (Vector3*)( vertbuf + posoffset );
						Vector3* posin = (Vector3*)( vertbufin + posoffset );
						float* weights = (float*)( vertbufin + bwoffset );
						unsigned char* indices = (unsigned char*)( vertbufin + bioffset );
						Matrix4 mats[4];
						mats[0] = skel->GetBone( indices[0] )->GetBoneTransform( );
						mats[1] = skel->GetBone( indices[1] )->GetBoneTransform( );
						mats[2] = skel->GetBone( indices[2] )->GetBoneTransform( );
						mats[3] = skel->GetBone( indices[3] )->GetBoneTransform( );
						
						Vector3 tmppos = Vector3( 0, 0, 0 );
						tmppos += mats[ 0 ].TransformCoord( *posin ) * weights[ 0 ];
						tmppos += mats[ 1 ].TransformCoord( *posin ) * weights[ 1 ];
						tmppos += mats[ 2 ].TransformCoord( *posin ) * weights[ 2 ];
						tmppos += mats[ 3 ].TransformCoord( *posin ) * weights[ 3 ];
						*pos = tmppos;
					}
				}

				mCurState->SetDirty( );
			}
		};

		virtual void UpdateCulling( const Frustum& camf, const Matrix4& mat )
		{
			//mCulled = CheckCulling( camf, mat );
		};

		virtual void QueueRender( RenderQueue* rq )
		{
			if( !mBaseState || !mEnabled || mCulled )
				return;
			rq->PushEntity( this, true );
		};

		virtual void Render( RenderSystem* rs )
		{
			rs->SetWorldMatrix( mTransform );
			rs->SetVertexDeclaration( mBaseState->GetVertexDeclaration( ) );
			rs->SetMaterial( mBaseState->GetMaterial( ) );
			if( mCurState ) {
				rs->SetStreamSource( 0, mCurState, 0 );
			} else {
				rs->SetStreamSource( 0, mBaseState->GetVertexBuffer( ), 0 );
			}
			if( mBaseState->GetIndexBuffer( ) == 0 ) {
				rs->DrawPrimitive( mBaseState->GetPrimitiveType( ), 0, mBaseState->GetPrimitiveCount( ) );
			} else {
				rs->SetIndexSource( mBaseState->GetIndexBuffer( ) );
				rs->DrawIndexedPrimitive( mBaseState->GetPrimitiveType( ), 0, 0, mBaseState->GetVertexBuffer( )->GetLength( ), mBaseState->GetPrimitiveCount( ) );
			}
		};

	private:
		VertexBufferBase* mCurState;
		MeshBase* mBaseState;
	};
};