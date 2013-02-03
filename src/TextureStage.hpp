#pragma once
#include "Texture.hpp"
#include "Matrix4.hpp"

namespace Halia
{
	namespace BlendOp
	{
		enum BlendOp
		{
			DISABLE = 1,
			SELECTARGUMENT1 = 2,
			SELECTARGUMENT2 = 3,
			MODULATE = 4,
			MODULATE2X = 5,
			MODULATE4X = 6,
			ADD = 7,
			ADDSIGNED = 8,
			ADDSIGNED2X = 9,
			SUBTRACT = 10,
			ADDSMOOTH = 11,
			BLENDDIFFUSEALPHA = 12,
			BLENDTEXTUREALPHA = 13,
			BLENDFACTORALPHA = 14,
		    BLENDTEXTUREALPHAPM = 15,
		    BLENDCURRENTALPHA = 16,
		    PREMODULATE = 17,
		    MODULATEALPHA_ADDCOLOR = 18,
		    MODULATECOLOR_ADDALPHA = 19,
		    MODULATEINVALPHA_ADDCOLOR = 20,
		    MODULATEINVCOLOR_ADDALPHA = 21,
			BUMPENVMAP = 22,
			BUMPENVMAPLUMINANCE = 23,
		    DOTPRODUCT3 = 24,
		    MULTIPLYADD = 25,
			LERP = 26
		};
	};
	
	namespace BlendArg
	{
		enum BlendArg
		{
			DIFFUSE = 0,
			CURRENT = 1,
			TEXTURE = 2,
			TFACTOR = 4,
			SPECULAR = 5,
			CONSTANT = 6,
		};
	};

	struct BlendMode
	{
		BlendMode( BlendOp::BlendOp op_, BlendArg::BlendArg arg1_, BlendArg::BlendArg arg2_ )
			: op( op_ ), arg1( arg1_ ), arg2( arg2_ )
		{
		};

		BlendMode( BlendOp::BlendOp op_, BlendArg::BlendArg arg_ )
			: op( op_ ), arg1( arg_ ), arg2( arg_ )
		{
		};

		BlendMode( BlendOp::BlendOp op_ )
			: op( op_ )
		{
		};

		BlendMode( )
		{
		};

		BlendOp::BlendOp op;
		BlendArg::BlendArg arg1;
		BlendArg::BlendArg arg2;
	};

	class TextureStage
	{
	public:
		TextureStage( TextureBase* tex, BlendMode cm, BlendMode am, const Matrix4& t )
			: texture( tex ), colormode( cm ), alphamode( am ), transform( t ), uses_transform( true )
		{
		};

		TextureStage( TextureBase* tex, BlendMode cm, BlendMode am )
			: texture( tex ), colormode( cm ), alphamode( am ), uses_transform( false )
		{
		};

		TextureStage( TextureBase* tex, BlendMode m, const Matrix4& t )
			: texture( tex ), colormode( m ), alphamode( m ), transform( t ), uses_transform( true )
		{
		};

		TextureStage( TextureBase* tex, BlendMode m )
			: texture( tex ), colormode( m ), alphamode( m ), uses_transform( false )
		{
		};

		~TextureStage( )
		{
		};

		void SetTexture( TextureBase* t )
		{
			texture = t;
		};

		TextureBase* texture;
		Matrix4 transform;
		bool uses_transform;
		BlendMode colormode;
		BlendMode alphamode;
	};
};