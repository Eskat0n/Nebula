#include "SimpleFile.h"
#include "IDataLayer.h"

#ifndef _FILE_ACCESS_LEVEL_
#define _FILE_ACCESS_LEVEL_

//----------------------------------------------------------------

/**
 * DataLayer which uses simple text files to store data.
 */
class DataLayer: public IDataLayer
{
private:
	static		DataLayer*		_self;
	static		int				_nRefCount;

	DataLayer();
	~DataLayer();

public:
	static		IDataLayer*		Instance();
	void		Release();

	bool		WriteToFile( LPCWSTR wstrFName, LPCWSTR wstrToWrite );
	bool		PutLine( LPCWSTR wstrFName, LPCWSTR wstrToWrite );
	bool		PutTimestampedLine( LPCWSTR wstrFName, LPCWSTR wstrToWrite );

	bool		WriteStat( LPCWSTR playmode, int lives, int ammo, int score );
};

//----------------------------------------------------------------

#endif