//-------------------------------------------------------------
// Saber Graphic Engine console implementation declarations file
//-------------------------------------------------------------

#include "SaberGE_Interface.h"

#include <iostream>

#ifndef _SBR_GE_CONSOLE_
#define _SBR_GE_CONSOLE_

//-------------------------------------------------------------

/**
 * Console version of ISagerGE implementation.
 * Only for testing purposes.
 */ 
class SaberGE_Console: public ISaberGE
{
private:
	SaberGE_Console() { }
	~SaberGE_Console() { }

	static		SaberGE_Console*	_self;
	static		int					_nRefCount;

public:
	static		ISaberGE*	Instance();
	void		__stdcall	Release();

	bool		__stdcall	SetUp() { return true; }
	void		__stdcall	Halt() { }

	void		__stdcall	SetWndTitle( LPCWSTR value ) { }
	void		__stdcall	SetResX( int value ) { }
	void		__stdcall	SetResY( int value ) { }
	void		__stdcall	SetBD( int value ) { }

	bool		__stdcall	BeginScene() { return true; }
	void		__stdcall	EndScene() { }
	void		__stdcall	Clear( DWORD color ) { }
	void		__stdcall	RenderQuad( const PQUAD quad );

	void		__stdcall	SetOnClose( SbrPlugFunc pFunc ) { }
	void		__stdcall	SetOnFocusGain( SbrPlugFunc pFunc ) { }
	void		__stdcall	SetOnFocusLost( SbrPlugFunc pFunc ) { }
	void		__stdcall	PlugFunc( SbrPlugMsgFunc pFunc ) { }
	void		__stdcall	UnplugFunc( SbrPlugMsgFunc pFunc ) { }

	SBRFONT		__stdcall	CreateFont( LPCWSTR facename, int size ) { return 0; }

	SBRTEXTURE	__stdcall	LoadTexture( LPCWSTR filename ) { return 0; }
	int			__stdcall	GetTextureWidth( SBRTEXTURE tex ) { return 0; }
	int			__stdcall	GetTextureHeight( SBRTEXTURE tex ) { return 0; }
	void		__stdcall	ReleaseTexture( SBRTEXTURE tex ) { }
};

//-------------------------------------------------------------

#endif