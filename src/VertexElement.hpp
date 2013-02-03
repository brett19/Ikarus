#pragma once
#include <vector>

namespace Halia
{
	namespace VDeclType
	{
		enum VDeclType
		{
			FLOAT1 = 0,
			FLOAT2 = 1,
			FLOAT3 = 2,
			FLOAT4 = 3,
			COLOUR = 4,
			UBYTE4 = 5,
			SHORT2 = 6,
			SHORT4 = 7,
			UBYTE4N = 8,
			SHORT2N = 9,
			SHORT4N = 10,
			USHORT2N = 11,
			USHORT4N = 12,
			UDEC3 = 13,
			DEC3N = 14,
			FLOAT16_2 = 15,
			FLOAT16_4 = 16,
			UNUSED = 17
		};
		const int LENGTH[] = { 4, 8, 12, 16, 4, 4, 4, 8, 4, 4, 8, 4, 8, 4, 4, 8, 16, 0 };
	};

	namespace VDeclSemantic
	{
		enum VDeclSemantic
		{
			POSITION = 0,
			BLENDWEIGHT = 1,
			BLENDINDEX = 2,
			NORMAL = 3,
			PSIZE = 4,
			TEXCOORD = 5,
			TANGENT = 6,
			BINORMAL = 7,
			TESSFACTOR = 8,
			POSITIONT = 9,
			COLOUR = 10,
			FOG = 11,
			DEPTH = 12,
			SAMPLE = 13,
		};
	};

	struct VertexElement
	{
		VertexElement( int streamidx_, VDeclSemantic::VDeclSemantic semantic_, VDeclType::VDeclType type_, int usageidx_ )
			: streamidx( streamidx_ ), semantic( semantic_ ), type( type_ ), usageidx( usageidx_ )
		{
		};

		int streamidx;
		VDeclSemantic::VDeclSemantic semantic;
		VDeclType::VDeclType type;
		int usageidx;

		typedef std::vector<VertexElement> List;
	};
};