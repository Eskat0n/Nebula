//-------------------------------------------------------------
// Saber Graphic Engine interface declaration file
//-------------------------------------------------------------

#include <windows.h>

#ifndef _SABER_GE_INTERFACE_
#define _SABER_GE_INTERFACE_

//-------------------------------------------------------------
// Definitions
//-------------------------------------------------------------

// Colors
#define SBR_WHITE		0xffffffff
#define SBR_BLACK		0xff000000
#define SBR_RED			0xffff0000
#define SBR_BLUE		0xff0000ff
#define SBR_GREEN		0xff00ff00
#define SBR_ALPHACH		0xffff00ff

/**
 *	Type of texture id (really pointer) supported by Saber Graphic Engine.
 */
typedef DWORD SBRTEXTURE;

typedef DWORD SBRFONT;
/**
 *	Pointer to function typedef.
 *	Type of pointer to function which takes void as parameters and returns nothing.
 *	Used to define reaction on some event.
 */
typedef void ( *SbrPlugFunc )();
/**
 *	Pointer to function typedef.
 *	Takes Windows API message as parameters.
 *	Used to add custom message processing to engine-managed window.
 */
typedef void ( *SbrPlugMsgFunc )( HWND, UINT, WPARAM, LPARAM );

//-------------------------------------------------------------
// Declarations
//-------------------------------------------------------------

/**
 *	Structure which represents single vertex with its parameters.
 */
typedef struct SaberVertex
{
	FLOAT	x, y;						/**< position on the screen. */
	FLOAT	z;							/**< z-buffer depth (0..1). */
	DWORD	color;						/**< color of vertex. */
	FLOAT	tx, ty;						/**< texture coordinates. */
}
*PVERTEX,
 VERTEX;

/**
 *	Structre which represents a textured quad as array of four vertices.
 */
typedef struct SaberQuad
{
	VERTEX		v[4];					/**< four vertices forming corners of quad. */
	SBRTEXTURE	tex;					/**< texture to place on quad. */
	INT			blend;					/**< reserved. */
}
*PQUAD,
 QUAD;

/**
 *	Structure which represents a texture with its data.
 */
typedef struct SaberTexture
{
	SBRTEXTURE			tex;			/**< pointer to texture. */
	INT					width, height;	/**< width and height of texture. */

	/**
	 *	Custom contructor.
	 *  Allows to parse init parameters to newly created SaberTexture instance.
	 *	@param tx pointer to texture 
	 *	@param w width of texture
	 *  @param h height of texture
	 */
	SaberTexture( SBRTEXTURE tx, INT w, INT h ): tex(tx), width(w), height(h) { }
}
*PTEXTURE,
 TEXTURE;

//-------------------------------------------------------------

/**
 * Unified interface to all Saber Graphic Engine implementations.
 * For method's details see SaberEngine documentation.
 */
interface ISaberGE
{
	virtual		void		__stdcall	Release() = 0;

	virtual		bool		__stdcall	SetUp() = 0;
	virtual		void		__stdcall	Halt() = 0;

	virtual		void		__stdcall	SetWndTitle( LPCWSTR value ) = 0;
	virtual		void		__stdcall	SetResX( int value ) = 0;
	virtual		void		__stdcall	SetResY( int value ) = 0;
	virtual		void		__stdcall	SetBD( int value ) = 0;

	virtual		bool		__stdcall	BeginScene() = 0;
	virtual		void		__stdcall	EndScene() = 0;
	virtual		void		__stdcall	Clear( DWORD color ) = 0;
	virtual		void		__stdcall	RenderQuad( const PQUAD quad ) = 0;

	virtual		void		__stdcall	SetOnClose( SbrPlugFunc pFunc ) = 0;
	virtual		void		__stdcall	SetOnFocusGain( SbrPlugFunc pFunc ) = 0;
	virtual		void		__stdcall	SetOnFocusLost( SbrPlugFunc pFunc ) = 0;
	virtual		void		__stdcall	PlugFunc( SbrPlugMsgFunc pFunc ) = 0;
	virtual		void		__stdcall	UnplugFunc( SbrPlugMsgFunc pFunc ) = 0;

	virtual		SBRFONT		__stdcall	CreateFont( LPCWSTR facename, int size ) = 0;

	virtual		SBRTEXTURE	__stdcall	LoadTexture( LPCWSTR filename ) = 0;
	virtual		int			__stdcall	GetTextureWidth( SBRTEXTURE tex ) = 0;
	virtual		int			__stdcall	GetTextureHeight( SBRTEXTURE tex ) = 0;
	virtual		void		__stdcall	ReleaseTexture( SBRTEXTURE tex ) = 0;
};

//-------------------------------------------------------------

#endif