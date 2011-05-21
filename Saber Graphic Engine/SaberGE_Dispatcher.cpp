#include "SaberGE_Dispatcher.h"

//-------------------------------------------------------------

SaberGE*	SaberGE::_self		= 0;
int			SaberGE::_nRefCount = 0;

//-------------------------------------------------------------

SaberGE::SaberGE( Api eApi )
{
	switch ( eApi )
	{
			case NONE:		_pSaberGE = 0;
				break;
			case CONSOLE:	_pSaberGE = SaberGE_Console::Instance();							
				break;
			case DIRECTX:	_pSaberGE = SaberEngine::Instance();	
				break;
		//	case OPENGL:	_pSaberGE = 0;
	}
}

//-------------------------------------------------------------

SaberGE::~SaberGE()
{
	_pSaberGE->Release();
}

//-------------------------------------------------------------

ISaberGE* SaberGE::Instance( Api eApi )
{
	if ( !_self )
		_self = new SaberGE( eApi );

	_nRefCount++;
	return static_cast<ISaberGE*>(_self);
}

//-------------------------------------------------------------

void __stdcall SaberGE::Release()
{
	--_nRefCount;
	if ( !_nRefCount )
	{
		delete _self;
		_self = 0;
	}
}

//-------------------------------------------------------------