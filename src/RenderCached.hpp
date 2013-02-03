#pragma once

namespace Halia
{
	class RenderCached
	{
	public:
		RenderCached( );
		~RenderCached( );
		bool IsDirty( ) const;
		void SetDirty( bool dirty = true );

		void* mCache;
		void* mOther;

	private:
		bool mDirty;
	};
};