//-------------------------------------------------------------
// Saber Graphic Engine sprite class declaration file
// Written by Eskat0n
// Current engine version: 0.0.8
// Modified: 6 nov 2008
//-------------------------------------------------------------

#include "SaberGE_Dispatcher.h"

#ifndef	_SBR_SPRITE_CLASS_
#define _SBR_SPRITE_CLASS_

//-------------------------------------------------------------

/**
 *	Sprite class.
 *	Sprite class used by Saber Graphic Engine.
 *	Represents a textured (or not) quad on the screen and operations for its processing, morphing and rendering.
 */
class SbrSprite 
{
protected:
	/**
	 *	Default constructor.
	 */
	SbrSprite();

	ISaberGE*			_sbr;							/**< Pointer to instance of Saber Graphic Engine. */

	QUAD				_quad;							/**< Quad which represents sprite on the screen. */
	FLOAT				_tx, _ty;						/**< Coordinates of sprite' texture. */
	FLOAT				_width;							/**< Sprite width. */
	FLOAT				_height;						/**< Sprite height. */
	FLOAT				_tex_width;						/**< Width of sprite' texture. */
	FLOAT				_tex_height;					/**< Height of sprite' texture. */
	FLOAT				_hotX;							/**< X coordinate of sprite' hotpoint. */
	FLOAT				_hotY;							/**< Y coordinate of sprite' hotpoint. */

public:
	/** 
	 * Custom constructor.
	 * @param textname null-terminated unicode string which contains path to the texture of new sprite.
	 * @param w width of new sprite.
	 * @param h height of new sprite.
	 * @param fit if true texture will be scaled to fit entire sprite size, otherwise it will be placed "as is".
	 * @param x displacement of texture along X axis (upper-left corner relative).
	 * @param y displacement of texture along Y axis (upper-left corner relative).
	 */
	SbrSprite( LPCWSTR texname, float w, float h, bool fit = true, float x = 0, float y = 0 );
	//SbrSprite( const SbrSprite &spr );
	/**
	 * Sprite class destructor.
	 * Releases obtained interface to Saber Engine instance.
	 */
	~SbrSprite();

	/**
	 * Renders sprite at specified position. 
	 * NOTE: Sprites are rendered by their hot-spot coordinates.
	 * @param x coordinate of the position sprite will be rendered at.
	 * @param y coordinate of the position sprite will be rendered at.
	 */
	void			Draw( FLOAT x, FLOAT y );
	/**
	 * Render sprite at specified position with additional parameters such as rotation angle and scaling.
	 * WARNING: Not tested yet, because this method isn't nesessary for current project.
	 * @param x coordinate of the position sprite will be rendered at.
	 * @param y coordinate of the position sprite will be rendered at.
	 * @param ang angle of rotation of the sprite (in radians).
	 * @param hscale horizontal scaling factor (along x axis). 
	 * @param vscale vertical scaling factor (along y axis, if set to 0.0f vertical scaling isn't applied).
	 */
	void			DrawEx( FLOAT x, FLOAT y, FLOAT ang, FLOAT hscale = 1.0f, FLOAT vscale = 0.0f );

	/**
	 * Sets texture for the sprite.
	 * @param textname null-terminated unicode string which contains path to the texture of the sprite.
 	 * @param fit if true texture will be scaled to fit entire sprite size, otherwise it will be placed "as is".
	 */
	void			SetTexture( LPCWSTR texname, bool fit = true);
	/**
	 * Sets texture for the sprite.
	 * @param tex id of the texture.
 	 * @param fit if true texture will be scaled to fit entire sprite size, otherwise it will be placed "as is".
	 */
	void			SetTexture( SBRTEXTURE tex, bool fit = true );
	/**
	 * Sets z-depth (z-axis coordinate) for this sprite.
	 * @param z z-axis coordinate.
 	 * @param i specifies vertex for which z-coordinate are setted. If value i has of -1 specified z coordinate will be applied to all of sprite' verticies.
	 */
	void			SetZ( FLOAT z, INT i = -1 );
	/**
	 * Sets color for this sprtie.
	 * @param color 32-bit value of color in ARGB format.
	 * @param i specifies vertex for which color are setted. If value i has of -1 specified color will be applied to all of sprite' verticies.
	 */
	void			SetColor( DWORD color, INT i = -1 );
	/** 
	 * Sets hotspot point position of the sprite.
	 * NOTE: Coordinates must be set in sprite' upper-left corner relative coordinate system.
	 * @param x hotspot coordinate along x axis.
	 * @param y hotspot coordinate along y axis.
	 */
	void			SetHotSpot( INT x, INT y ) { _hotX = x; _hotY = y; }

	/**
	 * Returns id of a texture used in the sprite.
	 * @return texture' id.
	 */
	SBRTEXTURE		GetTexture() const { return _quad.tex; }
	/**
	 * Returns z-depth of specified sprite quad corner.
	 * @param i index of sprite quad corner.
	 * @return z-depth of quad corner with index of i.
	 */
	FLOAT			GetZ( INT i = 0 ) const { return ( i > 3 || i < 0 ) ? 0.0f:_quad.v[i].z; }
	/**
	 * Returns 32-bit value of specified sprite quad corner' color.
	 * @param i index of sprite quad corner.
	 * @return 32-bit color value of quad corner with index of i.
	 */
	DWORD			GetColor( INT i = 0 ) const { return ( i > 3 || i < 0 ) ? 0.0f:_quad.v[i].color; }
	/** 
	 * Returns coordinates of sprite' hotspot point.
	 * @param x pointer to int-type variable which will receive value of x coordinate of the sprite' hotspot.
	 * @param y pointer to int-type variable which will receive value of y coordinate of the sprite' hotspot.
	 */
	void			GetHotSpot( INT* x, INT* y ) const { *x = _hotX; *y = _hotY; }
	/**
	 * Returns width of sprite.
	 * @reutnr width of sprite (in pixels).
	 */
	FLOAT			GetWidth() const { return _width; }
	/**
	 * Returns height of sprite.
	 * @reutnr height of sprite (in pixels).
	 */
	FLOAT			GetHeight() const { return _height; }
};

//-------------------------------------------------------------

#endif