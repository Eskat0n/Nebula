//-------------------------------------------------------------
// Saber Graphic Engine animation class implementation file
// Written by Eskat0n
// Current engine version: 0.0.8
// Modified: 6 nov 2008
//-------------------------------------------------------------

#include "SaberGE_Animation.h"

//-------------------------------------------------------------

SbrAnim::SbrAnim( LPCWSTR texname, int nFrames, float FPS, float width, float height, float x, float y )
				: SbrSprite( texname, width, height, x, y, false )
{
	SBRTEXTURE tex = _sbr->LoadTexture( texname );
	
	_orig_width = _sbr->GetTextureWidth( tex );

	_fTimeDeltaPF = -1.0f;
	_fSpeed = ( FPS != 0 ) ? (1.0f / FPS):1.0f;
	_bIsPlaying = false;
	_nFrames = nFrames;

	_mode = SBRAP_FORWARD | SBRAP_LOOP | SBRAP_DNTINTER;
	_nDelta = 1;
	SetFrame( 0 );
}

//-------------------------------------------------------------

void SbrAnim::SetMode( int mode ) 
{
	_mode = mode;

	_nDelta = 1;
	SetFrame( 0 );
}

//-------------------------------------------------------------

void SbrAnim::Play()
{
	_bIsPlaying = true;
	_fTimeDeltaPF = -1.0f;
	_nDelta = 1;
	SetFrame( 0 );
}

//-------------------------------------------------------------

void SbrAnim::Update( float fTimeDelta )
{
	if ( !_bIsPlaying )
		return;

	if ( _fTimeDeltaPF == -1.0f )
		_fTimeDeltaPF = 0.0f;
	else
		_fTimeDeltaPF += fTimeDelta;

	while ( _fTimeDeltaPF >= _fSpeed )
	{
		_fTimeDeltaPF -= _fSpeed;

		if ( ( _nCurFrame + _nDelta == _nFrames ) && ( _mode & SBRAP_LOOP ) == 0 )
			_bIsPlaying = false;

		if ( _bIsPlaying ) SetFrame( _nCurFrame + _nDelta );
	}
}

//-------------------------------------------------------------

void SbrAnim::SetFrame( int n )
{
	float tx1, tx2, ty1, ty2;
	if ( _orig_width == 0 || _width == 0 )
		return;
	int nCols = int(_orig_width) / int(_width);

	// Handling (n > _nFrames) case
	n = n % _nFrames;
	if ( n < 0 ) n += _nFrames;
	_nCurFrame = n;

	// Calculating texture coords for frame n
	ty1 = _ty;
	tx1 = _tx + n * _width;
	
	// Handling line spacing in texture
	if ( tx1 > _orig_width - _width )
	{
		n -= int( _orig_width - _tx ) / int(_width);
		tx1 = _width * ( n % nCols );
		ty1 += _height * ( 1 + n / nCols );
	}

	tx2 = tx1 + _width;
	ty2 = ty1 + _height;

	tx1 /= _tex_width + _width;
	ty1 /= _tex_height;
	tx2 /= _tex_width + _width;
	ty2 /= _tex_height;

	_quad.v[0].tx = tx1;	_quad.v[0].ty = ty1;
	_quad.v[1].tx = tx2;	_quad.v[1].ty = ty1;
	_quad.v[2].tx = tx2;	_quad.v[2].ty = ty2;
	_quad.v[3].tx = tx1;	_quad.v[3].ty = ty2;
}

//-------------------------------------------------------------