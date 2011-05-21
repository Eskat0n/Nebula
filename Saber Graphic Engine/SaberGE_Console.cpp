//-------------------------------------------------------------

#include "SaberGE_Console.h"

//-------------------------------------------------------------

SaberGE_Console*	SaberGE_Console::_self		= 0;
int					SaberGE_Console::_nRefCount = 0;

//-------------------------------------------------------------

ISaberGE* SaberGE_Console::Instance()
{
	if ( !_self )
		_self = new SaberGE_Console();

	_nRefCount++;
	return static_cast<ISaberGE*>(_self);
}

//-------------------------------------------------------------

void __stdcall SaberGE_Console::Release()
{
	--_nRefCount;
	if ( !_nRefCount )
	{
		delete _self;
		_self = 0;
	}
}

//-------------------------------------------------------------

void __stdcall SaberGE_Console::RenderQuad( const PQUAD quad )
{
	printf( "Rendering quad with dimessions: (%.3f;%.3f), (%.3f;%.3f), (%.3f;%.3f), (%.3f;%.3f)\n",
		    quad->v[0].x, quad->v[0].y,
			quad->v[1].x, quad->v[1].y,
			quad->v[2].x, quad->v[2].y,
			quad->v[3].x, quad->v[3].y );
}

//-------------------------------------------------------------