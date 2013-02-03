#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"

namespace Halia
{
	class MeshBase
	{
	public:
		virtual PrimitiveType::PrimitiveType GetPrimitiveType( ) const = 0;
		virtual int GetPrimitiveCount( ) const = 0;
		virtual VertexDeclaration* GetVertexDeclaration( ) const = 0;
		virtual VertexBufferBase* GetVertexBuffer( ) const = 0;
		virtual IndexBufferBase* GetIndexBuffer( ) const = 0;
		virtual Material* GetMaterial( ) const = 0;
	};

	class MeshMirror : public MeshBase
	{
	public:
		MeshMirror( )
		{
			mType = PrimitiveType::TriangleList;
			mDecl = 0;
			mVBuffer = new VertexBufferMirror( );
			mIBuffer = 0;
			mMaterial = 0;
		};

		MeshMirror( int vlength, int vstride )
		{
			mType = PrimitiveType::TriangleList;
			mDecl = 0;
			mVBuffer = new VertexBufferMirror( vlength, vstride );
			mIBuffer = 0;
			mMaterial = 0;
		};

		~MeshMirror( )
		{
		};

		void SetPrimitiveType( PrimitiveType::PrimitiveType type )
		{
			mType = type;
		};

		PrimitiveType::PrimitiveType GetPrimitiveType( ) const
		{
			return mType;
		};

		void SetPrimitiveCount( int primcount )
		{
			mPrimCount = primcount;
		};

		int GetPrimitiveCount( ) const
		{
			return mPrimCount;
		};
	
		void SetVertexDeclaration( VertexDeclaration* vdecl )
		{
			mDecl = vdecl;
		};

		VertexDeclaration* GetVertexDeclaration( ) const
		{
			return mDecl;
		};

		void InitVertexBuffer( int length )
		{
			mVBuffer->Init( length, mDecl->GetStride( 0 ) );
		};

		void InitVertexBuffer( int length, int stride )
		{
			mVBuffer->Init( length, stride );
		};

		template< typename T >
		void SetVertexValue( int vrt, VDeclSemantic::VDeclSemantic semantic, T value )
		{
			char* vertexval = mVBuffer->GetBuffer( ) + ( mVBuffer->GetStride( ) * vrt ) + mDecl->GetElementOffset( semantic, 0 );
			*(T*)vertexval = value;
		};

		VertexBufferBase* GetVertexBuffer( ) const
		{
			return (VertexBufferBase*&)mVBuffer;
		};

		VertexBufferMirror* GetTypedVertexBuffer( ) const
		{
			return mVBuffer;
		};

		IndexBufferBase* GetIndexBuffer( ) const
		{
			return mIBuffer;
		};

		Material* GetMaterial( ) const
		{
			return mMaterial;
		};

		void SetMaterial( Material* mat )
		{
			mMaterial = mat;
		};

	protected:
		PrimitiveType::PrimitiveType mType;
		int mPrimCount;
		VertexDeclaration* mDecl;
		VertexBufferMirror* mVBuffer;
		IndexBufferBase* mIBuffer;
		Material* mMaterial;
	};

	class IndexedMeshMirror : public MeshBase
	{
	public:
		IndexedMeshMirror( )
		{
			mType = PrimitiveType::TriangleList;
			mDecl = 0;
			mVBuffer = new VertexBufferMirror( );
			mIBuffer = new IndexBufferMirror( );
			mMaterial = 0;
		};

		IndexedMeshMirror( int vlength, int vstride, int ilength, int istride )
		{
			mType = PrimitiveType::TriangleList;
			mDecl = 0;
			mVBuffer = new VertexBufferMirror( vlength, vstride );
			mIBuffer = new IndexBufferMirror( ilength, istride );
			mMaterial = 0;
		};

		~IndexedMeshMirror( )
		{
		};

		void SetPrimitiveType( PrimitiveType::PrimitiveType type )
		{
			mType = type;
		};

		PrimitiveType::PrimitiveType GetPrimitiveType( ) const
		{
			return mType;
		};

		void SetPrimitiveCount( int primcount )
		{
			mPrimCount = primcount;
		};

		int GetPrimitiveCount( ) const
		{
			return mPrimCount;
		};

		void SetVertexDeclaration( VertexDeclaration* vdecl )
		{
			mDecl = vdecl;
		};

		VertexDeclaration* GetVertexDeclaration( ) const
		{
			return mDecl;
		};

		void InitVertexBuffer( int length )
		{
			mVBuffer->Init( length, mDecl->GetStride( 0 ) );
		};

		void InitVertexBuffer( int length, int stride )
		{
			mVBuffer->Init( length, stride );
		};

		VertexBufferBase* GetVertexBuffer( ) const
		{
			return (VertexBufferBase*&)mVBuffer;
		};

		VertexBufferMirror* GetTypedVertexBuffer( ) const
		{
			return mVBuffer;
		};

		void InitIndexBuffer( int length, int stride = 2 )
		{
			mIBuffer->Init( length, stride );
		};

		IndexBufferBase* GetIndexBuffer( ) const
		{
			return (IndexBufferBase*&)mIBuffer;
		};

		IndexBufferMirror* GetTypedIndexBuffer( ) const
		{
			return mIBuffer;
		};

		Material* GetMaterial( ) const
		{
			return mMaterial;
		};

		void SetMaterial( Material* mat )
		{
			mMaterial = mat;
		};

	protected:
		PrimitiveType::PrimitiveType mType;
		int mPrimCount;
		VertexDeclaration* mDecl;
		VertexBufferMirror* mVBuffer;
		IndexBufferMirror* mIBuffer;
		Material* mMaterial;
	};

	template< class M >
	class Mesh : public MeshBase
	{
	public:
		Mesh( )
		{
			mType = PrimitiveType::TriangleList;
			mDecl = 0;
			mVBuffer = new VertexBuffer< M >( );
			mIBuffer = 0;
			mMaterial = 0;
		};

		void SetPrimitiveType( PrimitiveType::PrimitiveType type )
		{
			mType = type;
		};

		PrimitiveType::PrimitiveType GetPrimitiveType( ) const
		{
			return mType;
		};

		void SetPrimitiveCount( int primcount )
		{
			mPrimCount = primcount;
		};

		int GetPrimitiveCount( ) const
		{
			return mPrimCount;
		};

		void SetVertexDeclaration( VertexDeclaration* vdecl )
		{
			mDecl = vdecl;
		};

		VertexDeclaration* GetVertexDeclaration( ) const
		{
			return mDecl;
		};

		VertexBufferBase* GetVertexBuffer( ) const
		{
			return (VertexBufferBase*&)mVBuffer;
		};

		VertexBuffer< M >* GetTypedVertexBuffer( ) const
		{
			return mVBuffer;
		};

		IndexBufferBase* GetIndexBuffer( ) const
		{
			return mIBuffer;
		};

		Material* GetMaterial( ) const
		{
			return mMaterial;
		};

		void SetMaterial( Material* mat )
		{
			mMaterial = mat;
		};

		typedef VertexBuffer< M >* VbType;

	private:
		PrimitiveType::PrimitiveType mType;
		int mPrimCount;
		VertexDeclaration* mDecl;
		VertexBuffer< M >* mVBuffer;
		IndexBufferBase* mIBuffer;
		Material* mMaterial;
	};

	template< class M, class I >
	class IndexedMesh : public MeshBase
	{
	public:
		IndexedMesh( )
		{
			mType = PrimitiveType::TriangleList;
			mDecl = 0;
			mVBuffer = new VertexBuffer< M >( );
			mIBuffer = new IndexBuffer< I >( );
			mMaterial = 0;
		};

		void SetPrimitiveType( PrimitiveType::PrimitiveType type )
		{
			mType = type;
		};

		PrimitiveType::PrimitiveType GetPrimitiveType( ) const
		{
			return mType;
		};

		void SetPrimitiveCount( int primcount )
		{
			mPrimCount = primcount;
		};

		int GetPrimitiveCount( ) const
		{
			return mPrimCount;
		};

		void SetVertexDeclaration( VertexDeclaration* vdecl )
		{
			mDecl = vdecl;
		};

		VertexDeclaration* GetVertexDeclaration( ) const
		{
			return mDecl;
		};

		VertexBufferBase* GetVertexBuffer( ) const
		{
			return (VertexBufferBase*&)mVBuffer;
		};

		VertexBuffer< M >* GetTypedVertexBuffer( ) const
		{
			return mVBuffer;
		};

		IndexBufferBase* GetIndexBuffer( ) const
		{
			return (IndexBufferBase*&)mIBuffer;
		};

		IndexBuffer< I >* GetTypedIndexBuffer( ) const
		{
			return mVBuffer;
		};

		Material* GetMaterial( ) const
		{
			return mMaterial;
		};

		void SetMaterial( Material* mat )
		{
			mMaterial = mat;
		};

		typedef VertexBuffer< M >* VbType;
		typedef IndexBuffer< I >* IbType;

	private:
		PrimitiveType::PrimitiveType mType;
		int mPrimCount;
		VertexDeclaration* mDecl;
		VertexBuffer< M >* mVBuffer;
		IndexBuffer< I >* mIBuffer;
		Material* mMaterial;
	};
};