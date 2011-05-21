#include <windows.h>

#ifndef _IDATA_LAYER_
#define _IDATA_LAYER_

//----------------------------------------------------------------

/**
 * Interface to concrete data layers.
 */
interface IDataLayer
{
	/**
	 * Releases current reference to data layer.
	 */
	virtual		void	Release() = 0;

	/**
	 * Writes string to file with specified filename.
	 * @param wstrFName unicode null-terminated string which contains path and name of file.
	 * @param wstrToWrite unicode null-terminated string which will be written to file.
	 * @return true is writing is succesful, otherwise false.
	 */
	virtual		bool	WriteToFile( LPCWSTR wstrFName, LPCWSTR wstrToWrite ) = 0;
	/**
	 * Writes string to file with specified filename and places CR/LF (#13) byte at end of this string.
	 * @param wstrFName unicode null-terminated string which contains path and name of file.
	 * @param wstrToWrite unicode null-terminated string which will be written to file.
	 * @return true is writing is succesful, otherwise false.
	 */
	virtual		bool	PutLine( LPCWSTR wstrFName, LPCWSTR wstrToWrite ) = 0;
	/**
	 * Same as above but current date and time will be placed at beginning of the string.
	 * @param wstrFName unicode null-terminated string which contains path and name of file.
	 * @param wstrToWrite unicode null-terminated string which will be written to file.
	 * @return true is writing is succesful, otherwise false.
	 */
	virtual		bool	PutTimestampedLine( LPCWSTR wstrFName, LPCWSTR wstrToWrite ) = 0;
	/**
	 * Writes player game statistics to file.
	 * @param playmode unicode null-terminated string which contains current play mode.
	 * @param lives number of lives left.
	 * @param ammo amount of rockets left.
	 * @param score game score points gained by player.
	 */
	virtual		bool	WriteStat( LPCWSTR playmode, int lives, int ammo, int score ) = 0;
};

//----------------------------------------------------------------

#endif