//-------------------------------------------------------------
// Dispatchers (bridges and adapters) classes methods implementation file
// Current implementation supports audio and graphics engines
//-------------------------------------------------------------

#include "Dispatchers_Decl.h"

//-------------------------------------------------------------

AudioDispatcher*	AudioDispatcher::_self = 0;
int			   AudioDispatcher::_nRefCount = 0;		

//-------------------------------------------------------------

AudioDispatcher* AudioDispatcher::GetInterface()
{
	if ( !_self )
		_self = new AudioDispatcher;

	_nRefCount++;
	return _self;
}

//-------------------------------------------------------------

void AudioDispatcher::Release()
{
	_nRefCount--;
	if ( !_nRefCount )
	{
		delete _self;
		_self = 0;
	}
}

//-------------------------------------------------------------

AudioDispatcher::AudioDispatcher()
{
	_pSoundEngine = AzuEngine::Instance();

	_pSoundEngine->SetUp();
}

//-------------------------------------------------------------

AudioDispatcher::~AudioDispatcher()
{
	_pSoundEngine->Release();
}

//-------------------------------------------------------------
