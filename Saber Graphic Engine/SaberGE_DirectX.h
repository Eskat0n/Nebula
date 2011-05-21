//-------------------------------------------------------------
// Saber Graphic Engine DirectX9 implementation declarations file
//-------------------------------------------------------------

#include "SaberGE_Interface.h"

#include <d3dx9.h>
#include <vector>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

#ifndef _SBR_GE_DIRECTX_
#define _SBR_GE_DIRECTX_

//-------------------------------------------------------------
// Definitions
//-------------------------------------------------------------

#define SBR_D3DFVF_VERTEX D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 
#define SBR_VERTEX_BUFFER_SIZE 4000

//-------------------------------------------------------------
// Declaration of main Saber Graphics Engine class and its build-in classes and structures
//-------------------------------------------------------------

/**
 * Saber Graphic Engine (SbrGE) implementation for use of DirectX API.
 * This class isn't a blob one. I agree, it have a lot of methods and fields but exits only for one purpose - render some funny graphins onto your screen. Nothing else.
 */
class SaberEngine: public ISaberGE
{
private:
	static SaberEngine* _self;				/**< pointer to only instance of this class. */
	static int			_nRefCount;			/**< number of references created to only instance of this class. */

	/**
	 *	Class default contructor.
	 *	Only initializes class fields.
	 */
	SaberEngine();	
	/**
	 *	Class destructor.
	 *	Releases resources used by DirectX and disallocates memory used by dynamical containers.
	 */
	~SaberEngine();							

private:
	// Overall parameters
	HINSTANCE					_hInst;					/**< handle of current application instance. */
	HWND						_hwnd;					/**< handle of window managed by Saber Engine. */
	BOOL						_bWindowed;				/**< true if running in windowed mode, false if in fullscreen mode. */
	INT							_nScreenResX;			/**< resolution along X axis (in pixels). */
	INT							_nScreenResY;			/**< resolution along Y axis (in pixels). */
	INT							_nScreenBD;				/**< reserved */
	BOOL						_bRunning;				/**< true if instance of Saber Engine is 
														 *	 currently initialized, otherwise false. */
	BOOL						_bActive;				/**< true if managed window is in focus, otherwise false. */
	BOOL						_bVisibleCursor;		/**< true if cursor is visible over surface of managed window, otherwise false. */

	// String parameters
	WCHAR						_wstrWinTitle[256];		/**< null-terminated unicode string which contains caption of managed window. */

	// Dynamical arrays
	std::vector<PTEXTURE>		_vTextures;				/**< vector of pointers to SaberTexture structures. */
	std::vector<SbrPlugMsgFunc>	_vFuncs;				/**< vector of pointers to functions of SbrPlugMsgFunc type. */
	PVERTEX						_pVertices;				/**< pointer to vertex on which are vertex buffer are locking. */

	SbrPlugFunc					_pOnCloseFunc;			/**< pointer to function which executes when managed window are closing. */
	SbrPlugFunc					_pOnFocusLostFunc;		/**< pointer to function which executes every time when managed window losts focus. */
	SbrPlugFunc					_pOnFocusGainFunc;		/**< pointer to function which executes every time when managed window gains focus. */

	// Internal graphic params
	RECT						_rectW;					/**< rectangle structure which contain size of managed window. */
	LONG						_styleW;				/**< style of managed window if running in windowed mode. */
	INT							_nPrim;					/**< reserved for batch vertex processing. */
	INT							_curPrimType;			/**< reserved for batch vertex processing. */
	SBRTEXTURE					_curTexture;			/**< reserved for batch vertex processing. */
	D3DXMATRIX					_mtProj;				/**< matrix which contains parameters of current projection: 
														 *	 translation, scale and rotate. */
	D3DXMATRIX					_mtView;				/**< matrix which contains parameters of view transformation. */

	// D3D pointers
	D3DPRESENT_PARAMETERS		_d3dpp;					/**< structure which contains parameters of Direct3dDevice initialization. */
	LPDIRECT3D9					_pD3d;					/**< pointer to IDirect3D9 interface. */
	LPDIRECT3DDEVICE9			_pD3dDevice;			/**< pointer to IDirect3DDevice9 interface. */
	LPDIRECT3DVERTEXBUFFER9		_pVB;					/**< pointer to interface of vertex buffer. */
	LPDIRECT3DINDEXBUFFER9		_pIB;					/**< not currently used */
	LPDIRECT3DSURFACE9			_pScreenSurf;			/**< not currently used */
	LPDIRECT3DSURFACE9			_pScreenDepth;			/**< not currently used */

	/**
	 *	Window message processing function.
	 *	Invokes all SbrPlugMsgFunc's parsing them its parameters (if any is plugged in SbrGE). 
	 *	This function also checks when managed window losts or gains focus, 
	 *	closes and invokes corresponding functions.
	 */
	static	LRESULT	__stdcall	WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	// SLAVE METHODS
	/**
	 *	Projection matrix adjustion method.
	 *	Calculates projection matrix factors corresponding to current resolution 
	 *	of the rendering surface (i.e. managed window).
	 *	@param width resolution of the rendering surface along X axis.
	 *	@param height resolution of the rendering surface along Y axis.
	 */
	void		_SetProjMatrix( int width, int height );
	/**
	 *	Not implemented yet.
	 */
	void		_RenderAll( bool bSceneEnded = false );
	/**
	 *	DirectX components initialization method.
	 *	Receive access to IDirect3D9, IDirect3DDevice9, IDirect3DVertexBuffer9 interfaces and adjusting rendering parameters.
	 *	@return true if initialization is successful, otherwise false.
	 */
	bool		_Initialize();
	/**
	 *	Method support abandoned.
	 */
	void		_CalibrateWindow();
	/**
	 *	Provides reaction at focus lost/gain events.
	 *	Invokes _pOnFocusLostFunc() or _pOnFocusGainFunc() when managed window losts or receives focus respectively.
	 *	NOTE: If this pointers are 0, nothing happened.
	 *	@param active new state of managed window' focus: true if focused, false if muted.
	 */
	void		_FocusChange( bool active );

public:
	// PRIMARY METHODS
	/**
	 *	Method used for receiving pointer to instance of SaberEngine.
	 *	@return pointer to instance of SaberEngine.
	 */
	static	ISaberGE*		Instance();
	/**
	 *	Releases reference to SaberEngine.
	 */
	void		__stdcall	Release();

	/**	Main initialization method.
	 *	Creates engine-managed window, initializes DirectX components via call to _Initialize() 
	 *	method and unlocks access to the potential of Saber Graphic Engine.
	 *	@return true if initialization is successful, otherwise false.
	 */
	bool		__stdcall	SetUp();
	/**	
	 *	Destroy rendering surface method.
	 *	Destroys managed window which invokes _pOnCloseFunc() (if defined) and releases all resources.
	 */
	void		__stdcall	Halt();

	/**
	 *	Sets caption of managed-window.
	 *	NOTE: caption isn't visible in fullscreen mode, but visible in Alt-Tab application switching and in task-bar.
	 *	@param value null-terminated unicode string.
	 */
	void		__stdcall	SetWndTitle( LPCWSTR value );
	/**
	 *	Sets resolution along X axis.
	 *	@param value new resolution in pixels (only positive values).
	 */
	void		__stdcall	SetResX( int value );
	/**
	 *	Sets resolution along Y axis.
	 *	@param value new resolution in pixels (only positive values).
	 */
	void		__stdcall	SetResY( int value );
	/**
	 *	Not implemented yet.
	 */
	void		__stdcall	SetBD( int value );

	/**
	 *	Begins scene of rendering at rendering surface.
	 *	WARNING! Any call to BeginScene() method must be supplied with respective call to EndScene() method 
	 *	and both calls must be placed in the same function (separate calls are allowed but strongly unrecommended).
	 *	@return true is scene is started successfully, otherwise false (eg. scene of already started for now).
	 */
	bool		__stdcall	BeginScene();
	/**
	 *	Ends currently started scene.
	 */
	void		__stdcall	EndScene();
	/**
	 *	Clears backbuffer(s) and fills it with selected color.
	 *	NOTE: Should be called before calling to BeginScene() method to avoid overdrawing.
	 *	@param color 32-bit formated color (ALPHA, REG, GREEN, BLUE).
	 */
	void		__stdcall	Clear( DWORD color );
	/**
	 *	Renders quad.
	 *	WARNING! Any call to this method should be placed in code between calls to BeginScene() and EndScene() methods.
	 *	@param quad pointer to SaberQuad structure which contains verticies forming the quad and its texture.
	 */
	void		__stdcall	RenderQuad( const PQUAD quad );

	/**
	 *	Sets function which invokes when managed window is closing.
	 *	@param pFunc pointer to function.
	 */
	void		__stdcall	SetOnClose( SbrPlugFunc pFunc );
	/**
	 *	Sets function which invokes when managed window losts focus.
	 *	@param pFunc pointer to function.
	 */
	void		__stdcall	SetOnFocusGain( SbrPlugFunc pFunc );
	/**
	 *	Sets function which invokes when managed window gains focus.
	 *	@param pFunc pointer to function.
	 */
	void		__stdcall	SetOnFocusLost( SbrPlugFunc pFunc );
	/**
	 *	Adds selected function to list of functions which parse messages received by managed window.
	 *	@param pFunc pointer to function to plug.
	 */
	void		__stdcall	PlugFunc( SbrPlugMsgFunc pFunc );
	/**
	 *	Remove selected function from list of functions which parse messages received by managed window.
	 *	@param pFunc pointer to function to unplug.
	 */
	void		__stdcall	UnplugFunc( SbrPlugMsgFunc pFunc );

	/**
	 *	Creates font by specified parameters.
	 *	@param facename null-terminated unicode string which contains facename of the font to be created.
	 *	@param size size of the font to be created.
	 *	@return pointer to newly created font.
	 */
	SBRFONT		__stdcall	CreateFont( LPCWSTR facename, int size );

	/**
	 *	Texture loading method.
	 *	Creates texture from file and adds this texture data to Saber Engine.
	 *	NOTE: .bmp, .png, .jpg, .tiff textures are supported.
	 *	@param filename null-terminated unicode string which contains path to texture file.
	 *	@return id of newly created texture (not texture data!).
	 */
	SBRTEXTURE	__stdcall	LoadTexture( LPCWSTR filename );
	/**
	 *	Returns width of texture by its id.
	 *	@param tex id of texture.
	 *	@return width of texture or 0, if texture with this specified pointer doesn't exist.
	 */
	int			__stdcall	GetTextureWidth( SBRTEXTURE tex );
	/**
	 *	Returns height of texture by its id.
	 *	@param tex id of texture.
	 *	@return height of texture or 0, if texture with this specified pointer doesn't exist.
	 */
	int			__stdcall	GetTextureHeight( SBRTEXTURE tex );
	/**
	 *	Releases texture and deletes its data.
	 *	@param tex id of texture.
	 */
	void		__stdcall	ReleaseTexture( SBRTEXTURE tex );
};

#endif