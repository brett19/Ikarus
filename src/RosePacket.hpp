#pragma once

class RosePacket
{
public:
	RosePacket( )
		: mCommand( 0 ), mSize( 6 ), mChecksum( 0 ), mCursor( 6 )
	{
	};

	RosePacket( unsigned short cmd )
		: mCommand( cmd ), mSize( 6 ), mChecksum( 0 ), mCursor( 6 )
	{
	};

	RosePacket( unsigned short cmd, unsigned short size )
		: mCommand( cmd ), mSize( size ), mChecksum( 0 ), mCursor( 6 )
	{
	};

	~RosePacket( )
	{
	};

	unsigned short GetSize( ) const
	{
		return mSize;
	};

	void SetCommand( unsigned short cmd )
	{
		mCommand = cmd;
	};

	unsigned short GetCommand( ) const
	{
		return mCommand;
	};

	template< typename T >
	void Add( T val )
	{
		*((T*)&mBuffer[ mSize ]) = val;
		mSize += sizeof( T );
	};

	void AddString( std::string val, bool zt = true )
	{
		for( std::string::iterator i = val.begin( ); i != val.end( ); ++i )
			Add( (char)*i );
		if( zt )
			Add( (char)0 );
	};

	template< typename T >
	T Get( int pos ) const
	{
		return *((T*)&mData[ pos ]);
	};

	std::string GetString( int pos, int len = -1 ) const
	{
		if( len < 0 ) {
			return &mData[ pos ];
		} else {
			std::string tmp;
			tmp.reserve( len );
			for( int i = pos; i < pos+len; i++ )
			{
				char c = Get<char>( i );
				if( c == 0 )
					break;
				tmp.push_back( c );
			}
			return tmp;
		}
	};

	template< typename T >
	T Get( )
	{
		T tmp = *((T*)&mBuffer[ mCursor ]);
		mCursor += sizeof( T );
		return tmp;
	};

	std::string GetString( int len = -1 )
	{
		if( len < 0 ) {
			char* str = &mBuffer[ mCursor ];
			mCursor += strlen( str ) + 1;
			return str;
		} else {
			std::string tmp;
			tmp.reserve( len );
			for( int i = 0; i < len; i++ )
			{
				char c = Get<char>( );
				if( c == 0 )
					break;
				tmp.push_back( c );
			}
			return tmp;
		}
	};

	void SetCursor( int pos )
	{
		mCursor = pos + 6;
	};

	bool Eof( )
	{
		return mCursor >= mSize;
	};

protected:
	union
	{
		struct
		{
			unsigned short mSize;
			unsigned short mCommand;
			unsigned short mChecksum;
			char mData[ 1 ];
		};
		char mBuffer[ 0x800 ];
	};
	unsigned short mCursor;
};