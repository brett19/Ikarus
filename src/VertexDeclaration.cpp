#include "VertexDeclaration.hpp"

namespace Halia
{
	void VertexDeclaration::AddElement( VertexElement elem_ )
	{
		mElements.push_back( elem_ );
	};

	const VertexElement::List& VertexDeclaration::GetElementList( )
	{
		return mElements;
	};

	bool VertexDeclaration::IsSupported( int streamidx_, VDeclSemantic::VDeclSemantic semantic_ )
	{
		for( VertexElement::List::iterator i = mElements.begin( ); i != mElements.end( ); i++ )
		{
			if( (*i).streamidx == streamidx_ && (*i).semantic == semantic_ )
				return true;
		}
		return false;
	};

	int VertexDeclaration::GetElementOffset( VDeclSemantic::VDeclSemantic semantic_, int usageidx_ )
	{
		int offset = 0;
		for( VertexElement::List::iterator i = mElements.begin( ); i != mElements.end( ); i++ )
		{
			if( (*i).semantic == semantic_ && (*i).usageidx == usageidx_ )
				return offset;
			offset += VDeclType::LENGTH[ (*i).type ];
		}
		return -1;
	};

	int VertexDeclaration::GetStride( int streamidx_ )
	{
		int stride = 0;
		for( VertexElement::List::iterator i = mElements.begin( ); i != mElements.end( ); i++ )
		{
			if( (*i).streamidx != streamidx_ )
				continue;
			stride += VDeclType::LENGTH[ (*i).type ];
		}
		return stride;
	};
};