//-------------------------------------------------------------
// SbrXFramer class methods implementation file
//-------------------------------------------------------------

#include "SbrXFramerClass.h"

//-------------------------------------------------------------

SbrXFramer* SbrXFramer::_self = 0;
int SbrXFramer::_nRefCount = 0;

//-------------------------------------------------------------

SbrXFramer* SbrXFramer::Instance()
{
	if ( !_self )
		_self = new SbrXFramer();
	_nRefCount++;

	return _self;
}

//-------------------------------------------------------------

void SbrXFramer::Release()
{
	_nRefCount--;
	if ( !_nRefCount )
	{
		delete _self;
		_self = 0;
	}
}

//-------------------------------------------------------------

SbrXFramer::SbrXFramer()
: _pGameLogicFunc( 0 ),
  _pGameRenderFunc( 0 ),

  _nOverallFPS( SBR_FPS_INFINITE ),
  _fTime( 0.0f ),
  _fTimeDelta( 0.0f ),
  _nFPS( 0 ),
  _nFixedDelta( 0 ),

  _dt( 0 ), _cfps( 0 )
{
	memset( &_keys, 0, sizeof(_keys) );
	_pInputQueue = 0;

	timeBeginPeriod(1);

	_t0 = _t0fps = timeGetTime();
}

//-------------------------------------------------------------

SbrXFramer::~SbrXFramer()
{
	timeEndPeriod(1);

	_ClearQueue();
}

//-------------------------------------------------------------

void SbrXFramer::_MakeEvent( int evType, int key, int scan, int x, int y )
{
	SbrInputEvent *pTempLast, *pTemp = new SbrInputEvent;
	BYTE kbState[256];
	POINT pt;

	pTemp->type = evType;
	pTemp->chr = 0;
	pt.x = x;
	pt.y = y;

	GetKeyboardState( kbState );

	if ( evType == SBRET_KEYDN )
	{
		_keys[key] |= 1;
		ToAscii( key, scan, kbState, (unsigned short *)&pTemp->chr, 0 );
	}

	if ( evType == SBRET_KEYUP )
	{
		_keys[key] |= 2;
		ToAscii( key, scan, kbState, (unsigned short *)&pTemp->chr, 0 );
	}

	if ( evType == SBRET_MBTNDN )
	{
		_keys[key] |= 1;
	//	SetCapture( _hwnd );
	//	_bCapture = true;
	}

	if ( evType == SBRET_MBTNUP )
	{
		_keys[key] |= 2;
		//ReleaseCapture();
		//SetMousePos( posX, posY );
		//_bCapture = false;
		pt.x = (int)_posX; 
		pt.y = (int)_posY;
	}

	// reserved section for handling macroses

	if ( pt.x == -1 ) 
	{
		// Saving cursor pos if mouse doesn't moved
		pTemp->x = _posX;
		pTemp->y = _posY;
	}
	else 
	{
		// Handling when cursor leaves window
		if ( pt.x < 0 ) pt.x = 0;								// correcting left-leave
		if ( pt.y < 0 ) pt.y = 0;								// correcting top-leave
//		if ( pt.x >= _nScreenResX ) pt.x = _nScreenResX - 1;	// correcting right-leave
//		if ( pt.y >= _nScreenResY ) pt.y = _nScreenResY - 1;	// correcting bottom-leave

		pTemp->x = (float)pt.x;
		pTemp->y = (float)pt.y;
	}

	pTemp->pNext = NULL;

	// Adding created SbrInputEvent to Saber Engine' input queue
	if ( !_pInputQueue ) 
		_pInputQueue = pTemp;
	else
	{
		pTempLast = _pInputQueue;
		while ( pTempLast->pNext ) 
			pTempLast = pTempLast->pNext;
		pTempLast->pNext = pTemp;
	}

	// Memorize cursor coordinates
	if ( evType == SBRET_MMOVE )
	{
		_posX = x;
		_posY = y;
	}
}

//-------------------------------------------------------------

void SbrXFramer::_ClearQueue()
{
	SbrInputEvent *pTemp, *pTempPrev = _pInputQueue;

	memset( &_keys, 0, sizeof(_keys) );							// nulifies key buffer;

	while ( pTempPrev )
	{
		pTemp = pTempPrev->pNext;
		delete pTempPrev;
		pTempPrev = pTemp;
	}

	_pInputQueue = 0;
}

//-------------------------------------------------------------

void SbrXFramer::SetFPS( int FPS )
{
	if ( _bRunning )
		return;

	_nOverallFPS = FPS;

	if ( _nOverallFPS > 0 )
		_nFixedDelta = int( 1000.0f / FPS );
	else
		_nFixedDelta = 0;
}

//-------------------------------------------------------------

void SbrXFramer::SetLogicFunc( SbrXCallback value )
{
	_pGameLogicFunc = value;
}

void SbrXFramer::SetRenderFunc( SbrXCallback value )
{
	_pGameRenderFunc = value;
}

//-------------------------------------------------------------

void SbrXFramer::MakeEventFromMSG( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if ( !_self )
		return;

	switch( msg )
    {
		case WM_SYSKEYDOWN:
			_self->_MakeEvent( SBRET_KEYDN, wParam, HIWORD(lParam) & 0xFF );
			return;

		case WM_KEYDOWN:
			_self->_MakeEvent( SBRET_KEYDN, wParam, HIWORD(lParam) & 0xFF );
			return;

		case WM_SYSKEYUP:
			_self->_MakeEvent( SBRET_KEYUP, wParam, HIWORD(lParam) & 0xFF );
			return;

		case WM_KEYUP:
			_self->_MakeEvent( SBRET_KEYUP, wParam, HIWORD(lParam) & 0xFF );
			return;

		case WM_MOUSEMOVE:
			_self->_MakeEvent( SBRET_MMOVE, 0, 0, LOWORD(lParam), HIWORD(lParam) );
    }
}

//-------------------------------------------------------------

bool SbrXFramer::EnterMessageLoop()
{
	MSG msg;

	if ( !_pGameLogicFunc )
		return false;

	_bRunning = true;

	// Main message loop

	while (1)
	{
		if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT ) break;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			continue;
		}

		if ( _bRunning )
		{
			// Do not confuse used with zero!
			do { _dt = timeGetTime() - _t0; } while ( _dt < 1 );

			if ( _dt >= _nFixedDelta )
			{
				_fTimeDelta = _dt / 1000.0f;

				if ( _fTimeDelta > 0.2f )
				{
					_fTimeDelta = _nFixedDelta ? _nFixedDelta / 1000.0f : 0.1f;	
				}

				_fTime += _fTimeDelta;
				_t0 = timeGetTime();

				if ( _t0 - _t0fps <= 1000 ) _cfps++;
				else 
				{
					_nFPS = _cfps;
					_cfps = 0;
					_t0fps = _t0;
				}

				if ( _pGameLogicFunc() ) break;
				if ( _pGameRenderFunc ) _pGameRenderFunc();

				_ClearQueue();

				Sleep(1);
			}
			else
			{
				if ( _nFixedDelta && _dt+3 < _nFixedDelta ) Sleep(1);
			}
		}
		else 
			Sleep(1);
	}

	_ClearQueue();

	_bRunning = false;

	return true;
}

//-------------------------------------------------------------

bool SbrXFramer::IsKeyDown( int key )
{
	return ( _keys[key] & 1 ) != 0;
}

//-------------------------------------------------------------

bool SbrXFramer::IsKeyUp( int key )
{
	return ( _keys[key] & 2 ) != 0;
}

//-------------------------------------------------------------