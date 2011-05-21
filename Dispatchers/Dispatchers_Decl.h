//-------------------------------------------------------------
// Dispatchers (bridges and adapters) classes declaration file
// Current implementation supports audio and graphics engines
//-------------------------------------------------------------

#include "../Azumaria Sound Engine/AzuEngineClass.h"

#ifndef _DISPATCHERS_
#define	_DISPATCHERS_

//-------------------------------------------------------------

class AudioDispatcher
{
protected:
	static	AudioDispatcher*	_self;
	static	int					_nRefCount;

	AudioDispatcher();
	~AudioDispatcher();

public:
	AzuEngine*					_pSoundEngine;

	static	AudioDispatcher*	GetInterface();
			void				Release();
};

//-------------------------------------------------------------

#endif
