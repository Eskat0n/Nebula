//-------------------------------------------------------------
// Saber Graphic Engine instance dispatcher (bridge)
//-------------------------------------------------------------

#include "SaberGE_Interface.h"
#include "SaberGE_DirectX.h"
#include "SaberGE_Console.h"

#ifndef _SABER_GE_DISPATCHER_
#define	_SABER_GE_DISPATCHER_

//-------------------------------------------------------------

enum Api
{
	NONE		= 0,
	CONSOLE		= 1,
	DIRECTX		= 2
};

//-------------------------------------------------------------

/**
 * Guarding proxy class.
 * Manages creation of different engine classes.
 */
class SaberGE: public ISaberGE
{
private:
	SaberGE( Api eApi );
	~SaberGE();

	static		SaberGE*	_self;
	static		int			_nRefCount;

				ISaberGE*	_pSaberGE;
public:
	static		ISaberGE*	Instance( Api eApi = NONE );
	void		__stdcall	Release();

	bool		__stdcall	SetUp() { if ( _pSaberGE ) return _pSaberGE->SetUp(); }
	void		__stdcall	Halt() { if ( _pSaberGE ) _pSaberGE->Halt(); }

	void		__stdcall	SetWndTitle( LPCWSTR value ) { if ( _pSaberGE ) _pSaberGE->SetWndTitle( value ); }
	void		__stdcall	SetResX( int value ) { if ( _pSaberGE ) _pSaberGE->SetResX( value ); }
	void		__stdcall	SetResY( int value ) { if ( _pSaberGE ) _pSaberGE->SetResY( value ); }
	void		__stdcall	SetBD( int value ) { if ( _pSaberGE ) _pSaberGE->SetBD( value ); }

	bool		__stdcall	BeginScene() { if ( _pSaberGE ) return _pSaberGE->BeginScene(); }
	void		__stdcall	EndScene() { if ( _pSaberGE ) _pSaberGE->EndScene(); }
	void		__stdcall	Clear( DWORD color ) { if ( _pSaberGE ) _pSaberGE->Clear( color ); }
	void		__stdcall	RenderQuad( const PQUAD quad ) { if ( _pSaberGE ) _pSaberGE->RenderQuad( quad ); }

	void		__stdcall	SetOnClose( SbrPlugFunc pFunc ) { if ( _pSaberGE ) _pSaberGE->SetOnClose( pFunc ); }
	void		__stdcall	SetOnFocusGain( SbrPlugFunc pFunc ) { if ( _pSaberGE ) _pSaberGE->SetOnFocusGain( pFunc ); }
	void		__stdcall	SetOnFocusLost( SbrPlugFunc pFunc ) { if ( _pSaberGE ) _pSaberGE->SetOnFocusLost( pFunc ); }
	void		__stdcall	PlugFunc( SbrPlugMsgFunc pFunc ) { if ( _pSaberGE ) _pSaberGE->PlugFunc( pFunc ); }
	void		__stdcall	UnplugFunc( SbrPlugMsgFunc pFunc ) { if ( _pSaberGE ) _pSaberGE->UnplugFunc( pFunc ); }

	SBRFONT		__stdcall	CreateFont( LPCWSTR facename, int size ) 
							{ if ( _pSaberGE ) return _pSaberGE->CreateFont( facename, size ); }
	SBRTEXTURE	__stdcall	LoadTexture( LPCWSTR filename )
							{ if ( _pSaberGE ) return _pSaberGE->LoadTexture( filename ); }
	int			__stdcall	GetTextureWidth( SBRTEXTURE tex )
							{ if ( _pSaberGE ) return _pSaberGE->GetTextureWidth( tex ); }
	int			__stdcall	GetTextureHeight( SBRTEXTURE tex )
							{ if ( _pSaberGE ) return _pSaberGE->GetTextureHeight( tex ); }
	void		__stdcall	ReleaseTexture( SBRTEXTURE tex )
							{ if ( _pSaberGE ) _pSaberGE->ReleaseTexture( tex ); }
};

//-------------------------------------------------------------

#endif