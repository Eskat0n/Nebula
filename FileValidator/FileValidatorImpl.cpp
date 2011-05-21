//-----------------------------------------------------
// File Validator class methods implementation file
//-----------------------------------------------------

#include "FileValidatorDecl.h"

//-----------------------------------------------------

FileValidator::FileValidator( int* reference, ... )
{
	int* pTemp = reference;
	int nCount = 0;

	while ( *pTemp )
	{
		nCount++;
		pTemp++;
	}

	_pnReference = new int[nCount];

	nCount = 0;
	pTemp = reference;

	while ( *pTemp )
	{
		_pnReference[nCount++] = *pTemp;
		pTemp++;
	}
}

//-----------------------------------------------------

FileValidator::~FileValidator()
{
}