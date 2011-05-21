//-------------------------------------------------------------
// Azumaria Sound Engine function desciption file
// Written by Eskat0n
// Current engine version: 0.1.0
// Modified: 2 nov 2008
//-------------------------------------------------------------

#include "AzuEngineClass.h"
#include <boost/foreach.hpp>

//-------------------------------------------------------------
// Singleton class 
AzuEngine* AzuEngine ::_self = NULL;
int AzuEngine ::_nRefCount = 0;

//-------------------------------------------------------------

AzuEngine* AzuEngine::Instance()
{
	if ( !_self )
		_self = new AzuEngine();
	_nRefCount++;
	return _self;
}

//-------------------------------------------------------------

void AzuEngine::Release()
{
	_nRefCount--;
	if ( !_nRefCount )
	{
		delete this;
		_self = NULL;
	}
}

//-------------------------------------------------------------

AzuEngine::AzuEngine()
	: _hThread( 0 ),
	  _bStreamingWBPrepared( false ),
	  _bDeployed( false )
{ }

//-------------------------------------------------------------

AzuEngine::~AzuEngine() 
{
	// terminating thread with update audio callback
	if ( _hThread )
		TerminateThread( _hThread, 0 );

	// releasing XACT resources
	if ( _state.pEngine )
	{
		_state.pEngine->ShutDown();
		_state.pEngine->Release();
	}

	if ( _state.hStreamingWaveBankFile != INVALID_HANDLE_VALUE && _state.hStreamingWaveBankFile != 0 )
		CloseHandle( _state.hStreamingWaveBankFile );

	if ( _state.pbSoundBank )
		delete[] _state.pbSoundBank;
	_state.pbSoundBank = 0;

	if ( _state.pbInMemoryWaveBank )
		UnmapViewOfFile( _state.pbInMemoryWaveBank );
	_state.pbInMemoryWaveBank = 0;

	DeleteCriticalSection( &_state.cs );

	CoUninitialize();

	BOOST_FOREACH( AzuSound* pSound, _pAzuSound )
	{
		delete pSound;
	}
	_pAzuSound.clear();
}

//-------------------------------------------------------------

HRESULT AzuEngine::SetUp()		  
{
	if ( _bDeployed )
		return S_OK;

	ZeroMemory( &_state, sizeof(EngineState) );
	InitializeCriticalSection( &_state.cs );

	if ( FAILED( CoInitializeEx( 0, COINIT_MULTITHREADED ) ) )
		return E_FAIL;

	DWORD dwCreationFlags = 0;
	if ( FAILED( XACT3CreateEngine( dwCreationFlags, &_state.pEngine ) ) )
		return E_FAIL;

	XACT_RUNTIME_PARAMETERS xrParams = {0};
	xrParams.lookAheadTime = 250;
	xrParams.fnNotificationCallback = AzuEngine::XACTNotificationCallback;
	_state.pEngine->Initialize( &xrParams );

	// Register XACT notifications
	RegNotification( XACT_FLAG_NOTIFICATION_PERSIST, XACTNOTIFICATIONTYPE_WAVEBANKPREPARED, 0 );
	RegNotification( XACT_FLAG_NOTIFICATION_PERSIST, XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED, 0 );
	RegNotification( XACT_FLAG_NOTIFICATION_PERSIST, XACTNOTIFICATIONTYPE_CUEPREPARED, XACTINDEX_INVALID ); 
	RegNotification( XACT_FLAG_NOTIFICATION_PERSIST, XACTNOTIFICATIONTYPE_CUESTOP, XACTINDEX_INVALID ); 

	_bDeployed = true;

	_hThread = (HANDLE)_beginthread( AzuEngine::UpdateCallback, 0, 0 );

	return S_OK;
}

//-------------------------------------------------------------

HRESULT AzuEngine::AddInMemoryWB( WCHAR* xwbFName ) 
{
	if ( !_bDeployed )
		return E_FAIL;

	HRESULT hr;
	HANDLE hFile;
	HANDLE hMapFile;
	DWORD dwFileSize;
	DWORD dwBytesRead;

	hr = E_FAIL;
	hFile = CreateFile( xwbFName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if ( hFile != INVALID_HANDLE_VALUE ) 
	{
		dwFileSize = GetFileSize( hFile, 0 );
		if ( dwFileSize != -1 ) 
		{
			hMapFile = CreateFileMapping( hFile, 0, PAGE_READONLY, 0, dwFileSize, 0 );
			if ( hMapFile )
			{
				_state.pbInMemoryWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0);
				if ( _state.pbInMemoryWaveBank )
				{
					hr = _state.pEngine->CreateInMemoryWaveBank( _state.pbInMemoryWaveBank, dwFileSize, 0, 0, &_state.pInMemoryWaveBank );
				}
			}
		}
		CloseHandle( hFile );
	}
	if ( FAILED( hr ) )
		return hr;

	return S_OK;
}

//-------------------------------------------------------------

HRESULT AzuEngine::AddStreamingWB( WCHAR* xwbFName )
{
	if ( !_bDeployed )
		return E_FAIL;

	HRESULT hr;
	
	hr = E_FAIL;
	_state.hStreamingWaveBankFile = CreateFile( xwbFName, 
												GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
												FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, 0 );

	if ( _state.hStreamingWaveBankFile != INVALID_HANDLE_VALUE ) 
	{
		XACT_WAVEBANK_STREAMING_PARAMETERS wsParams;
		ZeroMemory( &wsParams, sizeof( XACT_WAVEBANK_STREAMING_PARAMETERS ) );
		wsParams.file = _state.hStreamingWaveBankFile;
		wsParams.offset = 0;
		// allocating 64 * 2k buffer for streaming
		wsParams.packetSize = 64;

		hr = _state.pEngine->CreateStreamingWaveBank( &wsParams, &_state.pStreamingWaveBank );
	}
	if ( FAILED( hr ) )
		return hr;
	
	return S_OK;
}

//-------------------------------------------------------------

HRESULT AzuEngine::AddSB( WCHAR* xsbFName )
{
	if ( !_bDeployed )
		return E_FAIL;

	HRESULT hr;
	HANDLE hFile;
	HANDLE hMapFile;
	DWORD dwFileSize;
	DWORD dwBytesRead;

	hFile = CreateFile( xsbFName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if ( hFile != INVALID_HANDLE_VALUE )
	{
		dwFileSize = GetFileSize( hFile, 0 );
		if ( dwFileSize != -1 )
		{
			_state.pbSoundBank = new BYTE[dwFileSize];
			if ( _state.pbSoundBank )
			{
				if ( 0 != ReadFile( hFile, _state.pbSoundBank, dwFileSize, &dwBytesRead, 0 ) )
				{
					hr = _state.pEngine->CreateSoundBank( _state.pbSoundBank, dwFileSize, 0, 0, &_state.pSoundBank );
				}
			}
		}
		CloseHandle( hFile );
	}
	if ( FAILED( hr ) )
		return hr;

	return S_OK;
}

//-------------------------------------------------------------
// Operations with sound structures inside the AzuEngine class
//-------------------------------------------------------------

bool AzuEngine::LoadSound( PCSTR frName, SoundType sType, SoundStartState sState )
{
	if ( !_bDeployed )
		return false;

	if ( _state.pSoundBank->GetCueIndex( frName ) == XACTINDEX_INVALID )
		return false;

	BOOST_FOREACH( AzuSound* pSound, _pAzuSound )
	{
		if ( !strcmp( pSound->id, frName ) )
			return true;
	}

	AzuSound* pSound = new AzuSound;
	pSound->index = _state.pSoundBank->GetCueIndex( frName );
	pSound->type = sType;
	pSound->state = sState;
	strcpy( pSound->id, frName );

	_pAzuSound.push_back( pSound );

	return true;
}

//-------------------------------------------------------------

bool AzuEngine::DeleteSound( PCSTR frName )
{
	if ( !_bDeployed )
		return false;

	std::vector<AzuSound*>::iterator iTemp;

	for ( iTemp = _pAzuSound.begin(); iTemp != _pAzuSound.end(); iTemp++ )
	{
		if ( !strcmp( (*iTemp)->id, frName ) )
		{
			delete *iTemp;
			_pAzuSound.erase( iTemp );
			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------

bool AzuEngine::CycleTo( PCSTR frName )
{
	if ( !_bDeployed )
		return false;

	return true;
}

//-------------------------------------------------------------

bool AzuEngine::Stop( PCSTR frName )
{
	if ( !_bDeployed )
		return false;

	BOOST_FOREACH( AzuSound* pSound, _pAzuSound )
	{
		if ( !strcmp( pSound->id, frName ) )
		{
			EnterCriticalSection( &_state.cs );
			pSound->state = AZUSS_HALTED;
			LeaveCriticalSection( &_state.cs );
			_state.pSoundBank->Stop( pSound->index, 0 );
			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------

void AzuEngine::RegNotification( BYTE flags, XACTNOTIFICATIONTYPE type, XACTINDEX cueIndex )
{
	XACT_NOTIFICATION_DESCRIPTION desc = {0};
	desc.flags = flags;
	desc.type = type;
	desc.cueIndex = cueIndex;
	_state.pEngine->RegisterNotification( &desc );
}

//-------------------------------------------------------------

void AzuEngine::UpdateAudio()
{
	if ( !_bDeployed )
		return;

	BOOST_FOREACH( AzuSound* pSound, _pAzuSound )
	{
		if ( pSound->state != AZUSS_PLAYING && pSound->state != AZUSS_HALTED 
			&& pSound->state != AZUSS_DELAYED && pSound->type == AZUST_LOOP 
			&& ( pSound->state == AZUSS_STOPPED || _bStreamingWBPrepared ) ) 
		{
			EnterCriticalSection( &_state.cs );
			pSound->state = AZUSS_PLAYING;
			_bStreamingWBPrepared = false;
			LeaveCriticalSection( &_state.cs );
			_state.pSoundBank->Play( pSound->index, 0, 0, 0 );
		}
	}

	if ( _state.pEngine )
		_state.pEngine->DoWork();
}

//-------------------------------------------------------------

bool AzuEngine::Play( PCSTR frName )
{
	if ( !_bDeployed )
		return false;

	AzuSound* pCurSound = 0;

	BOOST_FOREACH( AzuSound* pSound, _pAzuSound )
	{
		if ( !strcmp( pSound->id, frName ) )
		{
			pCurSound = pSound;
			break;
		}
	}
	if ( !pCurSound )
		return false;

	if ( pCurSound->type == AZUST_DEFAULT || pCurSound->state != AZUSS_PLAYING )
	{
		EnterCriticalSection( &_state.cs );
		pCurSound->state = AZUSS_PLAYING;
		LeaveCriticalSection( &_state.cs );
		_state.pSoundBank->Play( pCurSound->index, 0, 0, 0 );
	}

	return true;
}

//-------------------------------------------------------------
// Notification function for handling XACT Engine notifications by AzuEngine
//-------------------------------------------------------------

void __stdcall AzuEngine::XACTNotificationCallback( const XACT_NOTIFICATION* pNotification )
{
	// Handling looped and stand-alone sounds
	if ( pNotification->type == XACTNOTIFICATIONTYPE_CUESTOP )
	{
		BOOST_FOREACH( AzuSound* pSound, _self->_pAzuSound )
		{
			if ( pNotification->cue.cueIndex == pSound->index &&
				( pSound->type == AZUST_LOOP || pSound->type == AZUST_SINGLE ) && 
				pSound->state != AZUSS_HALTED )
			{
				EnterCriticalSection( &_self->_state.cs );
				pSound->state = AZUSS_STOPPED;
				LeaveCriticalSection( &_self->_state.cs );
				return;
			}
		}
	}

	// Handling cleanup of SB memory on destroying
	if ( pNotification->type == XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED &&
		pNotification->soundBank.pSoundBank == _self->_state.pSoundBank )
	{
		if ( _self->_state.pbSoundBank )
		{
			delete[] _self->_state.pbSoundBank;
			_self->_state.pbSoundBank = NULL;
		}
	}

	// Handling preparing of streming wave bank
	if ( pNotification->type == XACTNOTIFICATIONTYPE_WAVEBANKPREPARED && 
		pNotification->waveBank.pWaveBank == _self->_state.pStreamingWaveBank )
	{
		EnterCriticalSection( &_self->_state.cs );
		_self->_bStreamingWBPrepared = true;
		LeaveCriticalSection( &_self->_state.cs );
	}
}

//-------------------------------------------------------------
// Thread callback function for handling update audio routine
//-------------------------------------------------------------

void __cdecl AzuEngine::UpdateCallback( PVOID dummy )
{
	while ( 1 )
	{
		if ( _self )
		{
			_self->UpdateAudio();
			Sleep( 10 );
		}
	}
}
