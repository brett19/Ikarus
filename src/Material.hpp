#pragma once
#include <vector>
#include "TextureStage.hpp"
#include "RenderCached.hpp"

namespace Halia
{
	class Material
	{
	public:
		Material( )
			: mTwoSided( true ), mAlphaBlend( false ), mAlphaTest( false ), mAlphaRef( 128 ),
			mDepthWrite( true ), mDepthBuffer( true ), mSpecularEnabled( false )
		{
		};

		Material( Color diffuse, Color ambient, Color specular, Color emissive, bool alphablend,
			bool alphatest, bool depthwrite, bool depthbuffer, bool specularenabled )
			: mDiffuse( diffuse ), mAmbient( ambient ), mSpecular( specular ), mEmissive( emissive ),
			mTwoSided( true ), mAlphaBlend( alphablend ), mAlphaTest( alphatest ), mAlphaRef( 128 ), 
			mDepthWrite( depthwrite ), mDepthBuffer( depthbuffer ), mSpecularEnabled( specularenabled )
		{
		};

		~Material( )
		{
		};

		void AddTextureStage( const TextureStage& ts )
		{
			mTStages.push_back( ts );
		};

		TextureStage& GetTextureStage( int stageid )
		{
			return mTStages[ stageid ];
		};

		std::vector< TextureStage > mTStages;
		Color mDiffuse;
		Color mAmbient;
		Color mSpecular;
		Color mEmissive;
		bool mTwoSided;
		bool mAlphaBlend;
		bool mAlphaTest;
		unsigned char mAlphaRef;
		bool mDepthWrite;
		bool mDepthBuffer;
		bool mSpecularEnabled;

	private:

	};
};