#include "DataLayer.h"
#include <time.h>
#include <strsafe.h>

//----------------------------------------------------------------

DataLayer*	DataLayer::_self		= 0;
int			DataLayer::_nRefCount	= 0;

//----------------------------------------------------------------

DataLayer::DataLayer()
{ }

//----------------------------------------------------------------

DataLayer::~DataLayer()
{ }

//----------------------------------------------------------------

IDataLayer* DataLayer::Instance()
{
	if ( !_self )
		_self = new DataLayer;

	_nRefCount++;
	return static_cast<IDataLayer*>(_self);
}

//----------------------------------------------------------------

void DataLayer::Release()
{
	_nRefCount--;
	if ( !_nRefCount )
	{
		delete _self;
		_self = 0;
	}
}

//----------------------------------------------------------------

bool DataLayer::WriteToFile( LPCWSTR wstrFName, LPCWSTR wstrToWrite ) 
{
	File pTempFile( wstrFName );
	
	return pTempFile.Write( wstrToWrite );
}

//----------------------------------------------------------------

bool DataLayer::PutLine( LPCWSTR wstrFName, LPCWSTR wstrToWrite )
{
	File pTempFile( wstrFName );

	pTempFile.Write( wstrToWrite );
	return pTempFile.Write( L"\n" );
}

//----------------------------------------------------------------

bool DataLayer::PutTimestampedLine( LPCWSTR wstrFName, LPCWSTR wstrToWrite )
{
	__time32_t timeBuffer;
	WCHAR wstrBuffer[26];

	_time32( &timeBuffer );
	errno_t error = _wctime32_s( wstrBuffer, 26, &timeBuffer );
	if ( error )
		return false;
	wstrBuffer[24] = L'\0';

	File pTempFile( wstrFName );

	pTempFile.Write( L"[" );
	pTempFile.Write( wstrBuffer );
	pTempFile.Write( L"] " );
	pTempFile.Write( wstrToWrite );
	return pTempFile.Write( L"\n" );
}

bool DataLayer::WriteStat( LPCWSTR playmode, int lives, int ammo, int score )
{
	WCHAR tempBuffer[256];

	StringCchPrintfW( tempBuffer, 255, L"\nPlay mode: %s\nLives left - %d, rockets left - %d\nTotal score: %d",
						playmode, lives, ammo, score );

	bool b = PutTimestampedLine( L"player_stat.txt", tempBuffer );
	b = b && PutLine( L"player_stat.txt", L"<<-------------------------------------" );

	return b;
}

//----------------------------------------------------------------