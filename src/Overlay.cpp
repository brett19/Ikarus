#include "Overlay.hpp"
#include "OverlayManager.hpp"

namespace Halia
{
	Overlay::Overlay( )
		: mParent( 0 ), mPosition( 0, 0 ), mSize( 0, 0 ), mVisible( true ), mFocused( false )
	{
	};

	Overlay::Overlay( int x, int y )
		: mParent( 0 ), mPosition( x, y ), mSize( 0, 0 ), mVisible( true ), mFocused( false )
	{
	};

	Overlay::Overlay( int x, int y, int sx, int sy )
		: mParent( 0 ), mPosition( x, y ), mSize( sx, sy ), mVisible( true ), mFocused( false )
	{
	};

	Overlay::~Overlay( )
	{
	};

	void Overlay::AddChild( Overlay* o )
	{
		o->mParent = this;
		mChildren.push_back( o );
	};

	const Point& Overlay::GetPosition( )
	{
		return mPosition;
	};

	void Overlay::SetPosition( int x, int y )
	{
		mPosition = Point( x, y );
	};

	void Overlay::SetSize( int x, int y )
	{
		mSize = Point( x, y );
	};

	void Overlay::SetVisible( bool visible )
	{
		mVisible = visible;
	};

	const Point& Overlay::GetSize( ) const
	{
		return mSize;
	};

	int Overlay::GetType( ) const
	{
		return OverlayType::BASE;
	};

	void Overlay::CenterChild( Overlay* child ) const
	{
		child->SetPosition(
			( mSize.x / 2 ) - ( child->GetSize( ).x / 2 ),
			( mSize.y / 2 ) - ( child->GetSize( ).y / 2 )
		);
	};

	void Overlay::Update( )
	{
		if( mParent )
			mGPosition = mParent->mGPosition + mPosition;
		else
			mGPosition = mPosition;

		for( std::vector< Overlay* >::iterator i = mChildren.begin( ); i != mChildren.end( ); ++i )
		{
			if( (*i)->mVisible )
				(*i)->Update( );
		}
	};

	void Overlay::Render( OverlayManager* mgr, OverlayRenderer* or )
	{
		for( std::vector< Overlay* >::iterator i = mChildren.begin( ); i != mChildren.end( ); ++i )
		{
			if( (*i)->mVisible )
				(*i)->Render( mgr, or );
		}
	};

	bool Overlay::DoEvent( EventBase* evt )
	{
		return false;
	};

	bool Overlay::ProcEvent( EventBase* evt )
	{
		for( std::vector< Overlay* >::iterator i = mChildren.begin( ); i != mChildren.end( ); ++i )
		{
			if( (*i)->ProcEvent( evt ) )
				return true;
		}
		return DoEvent( evt );
	};

	void Overlay::RaiseEvent( EventBase* evt )
	{
		mParent->ProcEventDown( evt );
	};

	void Overlay::ProcEventDown( EventBase* evt )
	{
		if( !DoEvent( evt ) )
			mParent->ProcEventDown( evt );
	};

	bool Overlay::TestPoint( int x, int y )
	{
		if( x > mGPosition.x && y > mGPosition.y &&
			x < mGPosition.x + mSize.x && y < mGPosition.y + mSize.y )
			return true;
		return false;
	};
};