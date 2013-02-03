#pragma once
#include <vector>
#include "Vector3.hpp"

namespace Halia
{
	class Entity;
	class RenderSystem;

	class RenderQueue
	{
	public:
		void SetCameraPos( Vector3 pos );
		void PushEntity( Entity* ent, bool sorted );
		void Render( RenderSystem* rs );
		void Clear( );

	protected:
		std::vector< Entity* > mQueue;
		std::vector< Entity* > mSQueue;
		Vector3 mCamPos;
	};
};