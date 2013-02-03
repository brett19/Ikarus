#include "RenderQueue.hpp"
#include "Entity.hpp"

namespace Halia
{
	void RenderQueue::SetCameraPos( Vector3 pos )
	{
		mCamPos = pos;
	};

	void RenderQueue::PushEntity( Entity* ent, bool sorted )
	{
		if( !sorted )
		{
			mQueue.push_back( ent );
			return;
		}

		Vector3 thisdelta = mCamPos - ent->GetPosition( );
		float thislen = thisdelta.LengthSq( );

		for( std::vector<Entity*>::iterator i = mSQueue.begin( ); i != mSQueue.end( ); ++i )
		{
			Vector3 delta = mCamPos - (*i)->GetPosition( );
			if( delta.LengthSq( ) < thislen )
			{
				mSQueue.insert( i, ent );
				return;
			}
		}
		mSQueue.push_back( ent );
	}

	void RenderQueue::Render( RenderSystem* rs )
	{
		for( std::vector<Entity*>::iterator i = mQueue.begin( ); i != mQueue.end( ); ++i )
		{
			(*i)->Render( rs );
		}
		for( std::vector<Entity*>::iterator i = mSQueue.begin( ); i != mSQueue.end( ); ++i )
		{
			(*i)->Render( rs );
		}
	};

	void RenderQueue::Clear( )
	{
		mQueue.clear( );
		mSQueue.clear( );
	};
};