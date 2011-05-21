#include <stdio.h>
#include <windows.h>

#ifndef _SIMPLE_FILE_
#define _SIMPLE_FILE_

//----------------------------------------------------------------

/**
 * Simple file class. 
 * Uses base iostream to write and read data from file.
 */
class File
{
private:
	WCHAR	_wstrFName[255];					/**< Name of file linked with this instance of File class. */
	FILE*	_pFile;								/**< Pointer to this file iostream. */
	bool	_bOpened;							/**< True if file is opened for now, otherwise false. */

public:
	/**
	 * Creates file with specified name or opens if it's already existing.
	 * @param wstrFName unicode null-terminated string which contain path and name of the file.
	 */
	File( LPCWSTR wstrFName );
	/**
	 * Destructor.
	 * Closes file iostream.
	 */
	~File();

	/**
	 * Opens file associated with this instance of File class.
	 * @return true if file is succesfully opened otherwise false.
	 */
	bool	Open();
	/**
	 * Writes string to the end of the file.
	 * @param wstrToWrite unicode null-terminated string to be written to the file.
	 * @return true if writing is succesful, otherwise false.
	 */
	bool	Write( LPCWSTR wstrToWrite );
	/**
	 * Closes file associated with this instance of File class.
	 * @return true if file is succesfully closed otherwise false.
	 */
	bool	Close();
};

//----------------------------------------------------------------

#endif