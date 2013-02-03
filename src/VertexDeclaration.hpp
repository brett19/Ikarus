#pragma once
#include <vector>
#include "SmartPtr.hpp"
#include "RenderCached.hpp"
#include "VertexElement.hpp"

namespace Halia
{
	class VertexDeclaration : public RenderCached, public SmartPtr
	{
	public:
		void AddElement( VertexElement elem_ );
		const VertexElement::List& GetElementList( );
		bool IsSupported( int streamidx_, VDeclSemantic::VDeclSemantic semantic_ );
		int GetElementOffset( VDeclSemantic::VDeclSemantic semantic_, int usageidx_ );
		int GetStride( int streamidx_ );

		VertexElement::List mElements;
	};
};