//-------------------------------------------------------------

#include "SaberHUDClass.h"

//-------------------------------------------------------------
// SbrHUDObject
//-------------------------------------------------------------

SbrHUDObject::SbrHUDObject()
{
	_pSbr = SaberGE::Instance();

	_dwFontColor = SBR_BLACK;

	_rcFontPos.left = 
	_rcFontPos.top =
	_rcFontPos.right = 
	_rcFontPos.bottom = 0;

	_bVisible = true;
}

//-------------------------------------------------------------

SbrHUDObject::~SbrHUDObject()
{
	_pFont->Release();
	_pSbr->Release();
}

//-------------------------------------------------------------

void SbrHUDObject::SetPos( int x, int y )
{
	_rcFontPos.left = x;
	_rcFontPos.top = y;
}

//-------------------------------------------------------------

bool SbrHUDObject::SetFont( LPCWSTR facename, int size )
{
	_pFont = (ID3DXFont*)_pSbr->CreateFont( facename, size );
	if ( _pFont )
		return true;
	else
		return false;
}

//-------------------------------------------------------------
// SbrHUDMessage
//-------------------------------------------------------------

SbrHUDMessage::SbrHUDMessage()
{
	wcscpy( _wsText, L"" );
}

//-------------------------------------------------------------

bool SbrHUDMessage::Draw()
{
	if ( !_pFont || !_bVisible )
		return false;

	_pFont->DrawTextW( 0, _wsText, -1, &_rcFontPos, DT_LEFT | DT_VCENTER | DT_NOCLIP, _dwFontColor );

	return true;
}

//-------------------------------------------------------------

void SbrHUDMessage::SetText( LPCWSTR wsText )
{
	wcscpy( _wsText, wsText );
}

//-------------------------------------------------------------

LPWSTR SbrHUDMessage::GetText() const
{
	static WCHAR wsStr[256];

	wcscpy( wsStr, _wsText );
	return wsStr;
}

//-------------------------------------------------------------
// SbrHUDCounter
//-------------------------------------------------------------

SbrHUDCounter::SbrHUDCounter()
{
	_dwValue = 0;
}

//-------------------------------------------------------------

bool SbrHUDCounter::Draw()
{
	if ( !_pFont || !_bVisible )
		return false;

	WCHAR wsStr[256];
	StringCchPrintfW( wsStr, 256, L"%d", _dwValue );

	_pFont->DrawTextW( 0, wsStr, -1, &_rcFontPos, DT_LEFT | DT_VCENTER | DT_NOCLIP, _dwFontColor );

	return true;
}

//-------------------------------------------------------------