//-------------------------------------------------------------
// Saber Graphic Engine sprite class implementation file
// Written by Eskat0n
// Current engine version: 0.0.8
// Modified: 6 nov 2008
//-------------------------------------------------------------

#include "SaberGE_Sprite.h"
#include <math.h>

//-------------------------------------------------------------

SbrSprite::SbrSprite( LPCWSTR texname, float w, float h, bool fit, float x, float y )
	: _tx( x ), _ty( y ),
	  _width( w ), _height( h )
{
	_sbr = SaberGE::Instance();		

	SBRTEXTURE tex = _sbr->LoadTexture( texname );

	if ( tex )
	{
		_tex_width = (FLOAT)_sbr->GetTextureWidth( tex );
		_tex_height = (FLOAT)_sbr->GetTextureHeight( tex );
	}
	else
	{
		_tex_width = 1.0f;
		_tex_height = 1.0f;
	}

	_hotX = 0; _hotY = 0;
	_quad.tex = tex;

	if ( fit )
	{
		_quad.v[0].tx = 0.0f;	_quad.v[0].ty = 0.0f;
		_quad.v[1].tx = 1.0f;	_quad.v[1].ty = 0.0f;
		_quad.v[2].tx = 1.0f;	_quad.v[2].ty = 1.0f;
		_quad.v[3].tx = 0.0f;	_quad.v[3].ty = 1.0f;
	}
	else
	if ( _tex_width != 0 && _tex_width != 0 )
	{
		FLOAT texx1, texx2, texy1, texy2;

		texx1 = x / _tex_width;
		texy1 = y / _tex_height;
		texx2 = ( x + w ) / _tex_width;
		texy2 = ( y + h ) / _tex_height;

		_quad.v[0].tx = texx1;	_quad.v[0].ty = texy1;
		_quad.v[1].tx = texx2;	_quad.v[1].ty = texy1;
		_quad.v[2].tx = texx2;	_quad.v[2].ty = texy2;
		_quad.v[3].tx = texx1;	_quad.v[3].ty = texy2;
	}

	_quad.v[0].z = 
	_quad.v[1].z = 
	_quad.v[2].z = 
	_quad.v[3].z = 0.0f;

	_quad.v[0].color =
	_quad.v[1].color =
	_quad.v[2].color =
	_quad.v[3].color = SBR_WHITE;

	_quad.blend = 0;						// WRYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY!
};

//-------------------------------------------------------------

SbrSprite::~SbrSprite()
{
	_sbr->Release();
}

//-------------------------------------------------------------

//SbrSprite::SbrSprite( const SbrSprite &spr )
//{
//	memcpy( this, &spr, sizeof( SbrSprite ) );
//};

//-------------------------------------------------------------

void SbrSprite::Draw( FLOAT x, FLOAT y )
{
	FLOAT tx1, tx2, ty1, ty2;

	tx1 = x - _hotX;
	ty1 = y - _hotY;
	tx2 = x + _width - _hotX;
	ty2 = y + _height - _hotY;

	_quad.v[0].x = tx1;	_quad.v[0].y = ty1;
	_quad.v[1].x = tx2;	_quad.v[1].y = ty1;
	_quad.v[2].x = tx2;	_quad.v[2].y = ty2;
	_quad.v[3].x = tx1;	_quad.v[3].y = ty2;

	_sbr->RenderQuad( &_quad );
};

//-------------------------------------------------------------

void SbrSprite::DrawEx( FLOAT x, FLOAT y, FLOAT ang, FLOAT hscale, FLOAT vscale )
{
	FLOAT tx1, tx2, ty1, ty2;
	FLOAT tsin, tcos;

	if ( vscale == 0 ) vscale = hscale;

	tx1 = -_hotX * hscale;
	ty1 = -_hotY * vscale;
	tx2 = ( _width - _hotX ) * hscale;
	ty2 = ( _height - _hotY ) * vscale;

	if ( ang != 0 )
	{
		tsin = sinf( ang );
		tcos = cosf( ang );

		_quad.v[0].x = tx1 * tcos - ty1 * tsin + x;
		_quad.v[0].y = tx1 * tsin - ty1 * tcos + y;

		_quad.v[1].x = tx2 * tcos - ty1 * tsin + x;
		_quad.v[1].y = tx2 * tsin - ty1 * tcos + y;

		_quad.v[2].x = tx2 * tcos - ty2 * tsin + x;
		_quad.v[2].y = tx2 * tsin - ty2 * tcos + y;

		_quad.v[3].x = tx1 * tcos - ty2 * tsin + x;
		_quad.v[3].y = tx1 * tsin - ty2 * tcos + y;
	}
	else 
	{
		_quad.v[0].x = tx1 + x;	_quad.v[0].y = ty1 + y;
		_quad.v[1].x = tx2 + x;	_quad.v[1].y = ty1 + y;
		_quad.v[2].x = tx2 + x;	_quad.v[2].y = ty2 + y;
		_quad.v[3].x = tx1 + x;	_quad.v[3].y = ty2 + y;	
	}

	_sbr->RenderQuad( &_quad );
};

//-------------------------------------------------------------

void SbrSprite::SetTexture( LPCWSTR texname, bool fit )
{
	SetTexture( _sbr->LoadTexture( texname ), fit );
}

//-------------------------------------------------------------

void SbrSprite::SetTexture( SBRTEXTURE tex, bool fit )
{
	FLOAT tx1, tx2, ty1, ty2;
	FLOAT tw, th;

	_quad.tex = tex;

	if ( tex )
	{
		tw = (FLOAT)_sbr->GetTextureWidth( tex );
		th = (FLOAT)_sbr->GetTextureHeight( tex );
	}
	else
	{
		tw = 1.0f;
		th = 1.0f;
	}

	if ( tw != _tex_width || th != _tex_height )
	{
		if ( fit )
		{
			_quad.v[0].tx = 0.0f;	_quad.v[0].ty = 0.0f;
			_quad.v[1].tx = 1.0f;	_quad.v[1].ty = 0.0f;
			_quad.v[2].tx = 1.0f;	_quad.v[2].ty = 1.0f;
			_quad.v[3].tx = 0.0f;	_quad.v[3].ty = 1.0f;
		}
		else
		{
			tx1 = _quad.v[0].tx * _tex_width;
			ty1 = _quad.v[0].ty * _tex_height;
			tx2 = _quad.v[2].tx * _tex_width;
			ty2 = _quad.v[2].ty * _tex_height;

			tx1 /= tw;	ty1 /= th;
			tx2 /= tw;	ty2 /= th;

			_quad.v[0].x = tx1;	_quad.v[0].y = ty1;
			_quad.v[1].x = tx2;	_quad.v[1].y = ty1;
			_quad.v[2].x = tx2;	_quad.v[2].y = ty2;
			_quad.v[3].x = tx1;	_quad.v[3].y = ty2;			
		}

		_tex_width = tw;
		_tex_height = th;
	}
};

//-------------------------------------------------------------

void SbrSprite::SetZ( FLOAT z, INT i )
{
	if ( i != -1 )
		_quad.v[i].z = z;
	else
		_quad.v[0].z = _quad.v[1].z = _quad.v[2].z = _quad.v[3].z = z;
};

//-------------------------------------------------------------

void SbrSprite::SetColor(DWORD color, INT i )
{
	if ( i != - 1 )
		_quad.v[i].color = color;
	else
		_quad.v[0].color = _quad.v[1].color = _quad.v[2].color = _quad.v[3].color = color;
};

//-------------------------------------------------------------


//-------------------------------------------------------------


//-------------------------------------------------------------


//-------------------------------------------------------------
