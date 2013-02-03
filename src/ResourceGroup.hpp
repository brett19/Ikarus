#pragma once
#include <vector>
#include "Resource.hpp"

namespace Halia
{
	class ResourceGroup : public Resource
	{
	public:
		ResourceGroup( );
		~ResourceGroup( );

		void AddResource( Resource* res );
		bool DelResource( Resource*& res );
		virtual bool Load( );
		virtual void Unload( );

	protected:
		std::vector< Resource* > mResources;
	};
};