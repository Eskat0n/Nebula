#include <stdio.h>
#include <string.h>
#include <tchar.h>

class FileValidator
{
private:
	unsigned int* _pnReference;

public:
	FileValidator( int* reference, ... );
	~FileValidator();

	bool	CheckDir( WCHAR dir );
};