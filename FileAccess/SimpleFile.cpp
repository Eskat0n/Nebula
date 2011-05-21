#include "SimpleFile.h"

//----------------------------------------------------------------

File::File( LPCWSTR wstrFName )
	: _pFile( 0 ),
	  _bOpened( false )
{
	wcscpy( _wstrFName, wstrFName );
	_pFile = _wfopen( _wstrFName, L"a+" );

	if ( _pFile )
	{
		_bOpened = true;
	}
}

//----------------------------------------------------------------

File::~File()
{
	if ( _bOpened )
		fclose( _pFile );
}

//----------------------------------------------------------------

bool File::Open()
{
	_pFile = _wfopen( _wstrFName, L"a+" );
	return ( _pFile != 0 );
}

//----------------------------------------------------------------

bool File::Close()
{
	if ( _bOpened )
		return !fclose( _pFile );

	return true;
}

//----------------------------------------------------------------

bool File::Write( LPCWSTR wstrToWrite )
{
	if ( !_bOpened ) 
		return false;

	return fputws( wstrToWrite, _pFile );	
}

//----------------------------------------------------------------