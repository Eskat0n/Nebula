//-------------------------------------------------------------
// Saber Graphic Engine main functions desciption file
// Written by Eskat0n
// Current engine version: 0.0.8
// Modified: 6 nov 2008
//-------------------------------------------------------------

#include "SaberGE_DirectX.h"
#include <boost/foreach.hpp>

//-------------------------------------------------------------

SaberEngine* SaberEngine::_self = 0;
int SaberEngine::_nRefCount = 0;

//-------------------------------------------------------------
//	SaberEngine class constructor
//	Func: sets default values to internal vars
//-------------------------------------------------------------

ISaberGE* SaberEngine::Instance() 
{
	if ( !_self )
		_self = new SaberEngine();
	_nRefCount++;
	return static_cast<ISaberGE*>(_self);
}

//-------------------------------------------------------------

void __stdcall SaberEngine::Release()
{
	_nRefCount--;
	if ( !_nRefCount )
	{
		delete _self;
		_self = 0;
	}
}

//-------------------------------------------------------------

SaberEngine::SaberEngine()
{
	// Nulifies private class fields
	_hInst = GetModuleHandle( 0 );
	_hwnd = 0;
	_bRunning = false;

	_pD3d = 0;
	_pD3dDevice = 0;
	//_d3dpp = 0;
	_pVB = 0;
	_pVertices = 0;

	_pOnCloseFunc = 0;

	_nScreenResX = 800;
	_nScreenResY = 600;
//	_nScreenBD = 32;
	_bVisibleCursor = false;

	wcscpy( _wstrWinTitle, L"DefaultWindow" );
}

//-------------------------------------------------------------
//	SaberEngine class destructor
//	Func: releases resources used by D3D Object, D3D Device and Vertex Buffer
//-------------------------------------------------------------

SaberEngine::~SaberEngine()
{
	_nRefCount = 0;

	_vTextures.clear();
	_vFuncs.clear();

	if ( _pD3d )
		_pD3d->Release();

	if ( _pD3dDevice )
		_pD3dDevice->Release();

	if ( _pVB )
		_pVB->Release();

	UnregisterClass( L"SaberEngineWnd", _hInst );
}

//-------------------------------------------------------------

void SaberEngine::_SetProjMatrix( int width, int height )
{
	D3DXMATRIX tmp;
	D3DXMatrixScaling( &_mtProj, 1.0f, -1.0f, 1.0f );
	D3DXMatrixTranslation( &tmp, -0.5f, height + 0.5f, 0.0f );
	D3DXMatrixMultiply( &_mtProj, &_mtProj, &tmp);
	D3DXMatrixOrthoOffCenterLH( &tmp, 0, (float)width, 0, (float)height, 0.0f, 1.0f);
	D3DXMatrixMultiply( &_mtProj, &_mtProj, &tmp);
}

//-------------------------------------------------------------

void SaberEngine::_RenderAll( bool bSceneEnded )
{
	if ( _pVertices )
	{
		_pVB->Unlock();

		if ( _nPrim )
		{
			_pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, _nPrim<<2, 0, _nPrim<<1, _nPrim * 2 );
			_nPrim = 0;
		}

		if ( bSceneEnded ) _pVertices = 0;
		else _pVB->Lock( 0, 0, (VOID**)&_pVertices, 0 );
	}
}

//-------------------------------------------------------------

bool SaberEngine::_Initialize()
{
	D3DADAPTER_IDENTIFIER9 AdID;
	D3DDISPLAYMODE Mode;
	D3DFORMAT Format = D3DFMT_UNKNOWN;

	// Init D3D
	_pD3d = Direct3DCreate9( D3D_SDK_VERSION );
	if ( _pD3d == 0 )
		return false;

	// Get adapter info
	_pD3d->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &AdID );

	// Set up for windowed mode
	if ( FAILED( _pD3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &Mode ) ) || Mode.Format == D3DFMT_UNKNOWN )
	{
		if ( _bWindowed ) return false;
	}

	ZeroMemory( &_d3dpp, sizeof(_d3dpp) );

	_d3dpp.BackBufferWidth = _nScreenResX;
	_d3dpp.BackBufferHeight = _nScreenResY;
	_d3dpp.BackBufferCount = 1;
	_d3dpp.BackBufferFormat = Mode.Format;
	_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	_d3dpp.hDeviceWindow = _hwnd;
	_d3dpp.Windowed = TRUE;

	_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;				// ??????
	
	_d3dpp.EnableAutoDepthStencil = TRUE;
	_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Set up for fullscreen mode
	// skipped

	// Create D3D Device
	if ( FAILED( _pD3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hwnd, 
										D3DCREATE_HARDWARE_VERTEXPROCESSING,
										&_d3dpp, &_pD3dDevice ) ) )
		return false;

	// TODO: in next versions
	//_pScreenSurf=0;
	//_pScreenDepth=0;

	//_pD3dDevice->GetRenderTarget( &_pScreenSurf );
	//_pD3dDevice->GetDepthStencilSurface( &_pScreenDepth );

	// Skipped in performance purposes
	//_CalibrateWindow();

	// Initialize vertex pointer
	_pVertices = 0;

	// Create vertex buffer
	if ( FAILED( _pD3dDevice->CreateVertexBuffer( SBR_VERTEX_BUFFER_SIZE * sizeof(VERTEX),
													D3DUSAGE_WRITEONLY,
													SBR_D3DFVF_VERTEX,
													D3DPOOL_DEFAULT, &_pVB, 0 ) ) )
		return false;

	//_pD3dDevice->SetVertexShader( 0 );
	_pD3dDevice->SetFVF( SBR_D3DFVF_VERTEX );
	_pD3dDevice->SetStreamSource( 0, _pVB, 0, sizeof(VERTEX) );

	// Setting main render states
	_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	_pD3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	_pD3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	_pD3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	_pD3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x01 );
	_pD3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	_nPrim = 0;
	_curTexture = 0;

	// Other stuff
	_SetProjMatrix( _nScreenResX, _nScreenResY );
	D3DXMatrixIdentity( &_mtView );
	_pD3dDevice->SetTransform(D3DTS_VIEW, &_mtView);
	_pD3dDevice->SetTransform( D3DTS_PROJECTION, &_mtProj );

	Clear( 0 );

	return true;
}

//-------------------------------------------------------------

void SaberEngine::_CalibrateWindow()
{
	RECT* rect;
	LONG style;

	if ( _bWindowed ) 
	{ 
		rect = &_rectW;
		style = _styleW;
	}
	else
	{
		// FS code goes here
	}
	SetWindowLong( _hwnd, GWL_STYLE, style );

	style = GetWindowLong( _hwnd, GWL_EXSTYLE );
	if ( _bWindowed )
	{
		SetWindowLong( _hwnd, GWL_EXSTYLE, style & (~WS_EX_TOPMOST) );
		SetWindowPos( _hwnd, HWND_TOPMOST, rect->left, rect->top, 
						rect->right - rect->left, rect->bottom - rect->top, SWP_FRAMECHANGED );
	}
	else 
	{
		// FS code goes here
	}
}

//-------------------------------------------------------------

void SaberEngine::_FocusChange( bool active )
{
	_bActive = active;
	
	if ( _bActive )
	{
		if ( _pOnFocusGainFunc ) _pOnFocusGainFunc();
	}
	else
	{
		if ( _pOnFocusLostFunc ) _pOnFocusLostFunc();
	}
}

//-------------------------------------------------------------

bool __stdcall SaberEngine::SetUp()
{
	WNDCLASS wnd;
	INT width, height;

	wnd.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc		= SaberEngine::WindowProc;
	wnd.cbClsExtra		= 0;
	wnd.cbWndExtra		= 0;
	wnd.hInstance		= _hInst;
	wnd.hCursor			= LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd.lpszMenuName	= 0; 
	wnd.lpszClassName	= L"SaberEngineWnd";
	wnd.hIcon			= LoadIcon( _hInst, IDI_APPLICATION );

	if ( !RegisterClass( &wnd ) )
		return false;

	width = _nScreenResX;
	height = _nScreenResY;

	_rectW.left = ( GetSystemMetrics( SM_CXSCREEN ) - width ) / 2;
	_rectW.top = ( GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;
	_rectW.right = _rectW.left + width;
	_rectW.bottom = _rectW.top + height;

	_styleW = WS_POPUP | WS_MINIMIZEBOX | WS_VISIBLE | WS_CAPTION;

	_hwnd = CreateWindowEx( 0, L"SaberEngineWnd", _wstrWinTitle, _styleW, _rectW.left, _rectW.top,
							width, height, 0, 0, _hInst, 0 );

	if ( !_hwnd )
		return false;

	ShowWindow( _hwnd, SW_SHOW );
	UpdateWindow( _hwnd );

	if ( !_Initialize() ) return false;

	_bActive = true;

	return true;
}

//-------------------------------------------------------------

void __stdcall SaberEngine::Halt()
{
	if ( _hwnd )
		DestroyWindow( _hwnd );
}

//-------------------------------------------------------------

void __stdcall SaberEngine::SetWndTitle( LPCWSTR value )
{
	wcscpy( _wstrWinTitle, value );
}

//-------------------------------------------------------------

void __stdcall SaberEngine::SetResX( int value )
{
	if ( !_pD3dDevice ) 
		if ( value > 0 )
			_nScreenResX = value;
}

void __stdcall SaberEngine::SetResY( int value )
{
	if ( !_pD3dDevice )
		if ( value > 0 )
			_nScreenResY = value; 
}

void __stdcall SaberEngine::SetBD( int value )
{
	if ( !_pD3dDevice ) 
		_nScreenBD = value;
}

//-------------------------------------------------------------

bool __stdcall SaberEngine::BeginScene()
{
	//if ( _pVertices )
	//	return false;

	_pD3dDevice->BeginScene();
	//if ( FAILED( _pVB->Lock( 0, 0, (VOID**)&_pVertices, 0 ) ) )
	//	return false;

	return true;
}

//-------------------------------------------------------------

void __stdcall SaberEngine::EndScene()
{
	//_RenderAll( true );
	_pD3dDevice->EndScene();
	_pD3dDevice->Present( 0, 0, 0, 0 );
}

//-------------------------------------------------------------

void __stdcall SaberEngine::Clear( DWORD color )
{
	_pD3dDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0 );
}

//-------------------------------------------------------------

void __stdcall SaberEngine::RenderQuad( const PQUAD quad )
{
	// TODO: put batching code here

	//if ( _pVertices )
	//{
	//	if ( _curTexture != quad->tex || _nPrim >= SBR_VERTEX_BUFFER_SIZE / 4 )
	//	{
	//		_RenderAll();

	//		if ( _curTexture != quad->tex ) 
	//		{
	//			_pD3dDevice->SetTexture( 0, quad->tex );
	//			_curTexture = quad->tex;
	//		}
	//	}

	//	memcpy( &_pVertices[ _nPrim * 4 ], quad->v, sizeof(SbrVertex) * 4 );
	//	_nPrim++;
	//}

	PVOID pVertices;
	_pVB->Lock( 0, 0, (void**)&pVertices, 0 );
	memcpy( pVertices, quad->v, sizeof(quad->v));
	_pVB->Unlock();

	_pD3dDevice->SetTexture( 0, (LPDIRECT3DTEXTURE9)(quad->tex) );
	_pD3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
}

//-------------------------------------------------------------

void __stdcall SaberEngine::SetOnClose( SbrPlugFunc pFunc )
{
	_pOnCloseFunc = pFunc;
}

void __stdcall SaberEngine::SetOnFocusGain( SbrPlugFunc pFunc )
{
	_pOnFocusGainFunc = pFunc;
}

void __stdcall SaberEngine::SetOnFocusLost( SbrPlugFunc pFunc )
{
	_pOnFocusLostFunc = pFunc;
}

//-------------------------------------------------------------

void __stdcall SaberEngine::PlugFunc( SbrPlugMsgFunc pFunc )
{
	_vFuncs.push_back( pFunc );
}

//-------------------------------------------------------------

void __stdcall SaberEngine::UnplugFunc( SbrPlugMsgFunc pFunc )
{
	std::vector<SbrPlugMsgFunc>::iterator iTemp;
	for ( iTemp = _vFuncs.begin(); iTemp != _vFuncs.end(); iTemp++ )
	{
		if ( *iTemp = pFunc )
		{
			_vFuncs.erase( iTemp );
			break;
		}
	}
}

//-------------------------------------------------------------

SBRFONT SaberEngine::CreateFont( LPCWSTR facename, int size )
{
	ID3DXFont* pFont;

	if ( FAILED (	D3DXCreateFont( _pD3dDevice, size, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
					facename, &pFont ) ) )
		return 0;

	return (SBRFONT)pFont;
}

//-------------------------------------------------------------

SBRTEXTURE __stdcall SaberEngine::LoadTexture( LPCWSTR filename )//, DWORD size, bool bMipmap ) 
{
	LPDIRECT3DTEXTURE9 pTex;
	D3DXIMAGE_INFO info;

	if ( !_pD3dDevice )
		return 0;

	if ( FAILED( D3DXCreateTextureFromFileEx( _pD3dDevice, filename, 
												0, 0, 0, 0,
												D3DFMT_A8R8G8B8, 
												D3DPOOL_MANAGED,
												D3DX_FILTER_NONE,
												D3DX_FILTER_NONE,
												SBR_ALPHACH,
												&info, 0, &pTex ) ) )
		return 0;

	_vTextures.push_back( new TEXTURE( (SBRTEXTURE)pTex, info.Width, info.Height ) );

	return (SBRTEXTURE)pTex;
}

//-------------------------------------------------------------

INT __stdcall SaberEngine::GetTextureWidth( SBRTEXTURE tex )
{	
	BOOST_FOREACH( PTEXTURE pTex, _vTextures )
	{
		if ( pTex->tex == tex ) 
			return pTex->width;
	}
	return 0;
}

//-------------------------------------------------------------

INT __stdcall SaberEngine::GetTextureHeight( SBRTEXTURE tex )
{
	BOOST_FOREACH( PTEXTURE pTex, _vTextures )
	{
		if ( pTex->tex == tex ) 
			return pTex->height;
	}
	return 0;
}

//-------------------------------------------------------------

void __stdcall SaberEngine::ReleaseTexture( SBRTEXTURE tex )
{
	if ( !tex )
		return;

	std::vector<PTEXTURE>::iterator iTemp;
	for ( iTemp = _vTextures.begin(); iTemp != _vTextures.end(); iTemp++ )
	{
		if ( (*iTemp)->tex == tex )
		{
			((LPDIRECT3DTEXTURE9)((*iTemp)->tex))->Release();
			_vTextures.erase( iTemp );
			return;
		}
	}
};

//-------------------------------------------------------------

LRESULT __stdcall SaberEngine::WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if ( _self )
	{
		BOOST_FOREACH( SbrPlugMsgFunc pFunc, _self->_vFuncs )
		{
			pFunc( hWnd, msg, wParam, lParam );
		}

		switch ( msg )
		{
			case WM_SETCURSOR:
				if ( !_self->_bVisibleCursor )
					SetCursor( 0 );
				else 
					SetCursor( LoadCursor( 0, IDC_ARROW ) );
				return 0;

			case WM_CLOSE:
				if ( _self->_pOnCloseFunc ) 
					_self->_pOnCloseFunc();
				return 0;

			case WM_ACTIVATE:
				{
					bool bActivating = ( LOWORD(wParam) != WA_INACTIVE ) && ( HIWORD(wParam) == 0 );
					if( _self->_pD3d && _self->_bActive != bActivating ) 
						_self->_FocusChange( bActivating );
				}
		}
	}

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-------------------------------------------------------------