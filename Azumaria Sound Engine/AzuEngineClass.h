//-------------------------------------------------------------
// Azumaria Sound Engine declarations file
// Written by Eskat0n
// Current engine version: 0.1.0
// Modified: 2 nov 2008
//-------------------------------------------------------------

#define _WIN32_DCOM
#include <xact3.h>
#include <strsafe.h>
#include <process.h>
#include <vector>

#ifndef _AZUMARIA_ENGINE_
#define _AZUMARIA_ENGINE_

//-------------------------------------------------------------
// Definition of some constants
//-------------------------------------------------------------

/**
 *	Macro for creating pointer with name a to the Azumaria Engine instance
 */
#define AzuCreate(a) AzuEngine* ##a = AzuEngine::Instance()

/**
 *	Enum type which represents type of sound.
 */
enum SoundType
{
	AZUST_DEFAULT		= 0,		/**< sound plays when Play() method for this sound is executed w/o any restrictions. */
	AZUST_SINGLE		= 1,		/**< only one instance of this sound can be in playback at one time. */
	AZUST_LOOP			= 2,		/**< sound will be repeated after its playback overs, 
									 *	 playing of this sound begins immediately after WaveBank which 
									 *	 contains this sound are prepared for streaming. */
	AZUST_FAST_STREAM	= 3			/**< reserved for next versions. */
};

/**
 *	Enum type which represents current state of sound.
 */
enum SoundState
{
	AZUSS_READY			= 0,		/**< sound is ready for immediate playing. */
	AZUSS_PLAYING		= 1,		/**< sound are playing now. */
	AZUSS_STOPPED		= 2,		/**< sound are currently stopped since playback is over. */
	AZUSS_DELAYED		= 3,		/**< playback of sound is delayed in time, only valid for AZUST_LOOP sounds 
									 *	 and prevent sound from default on-ready play. */
	AZUSS_HALTED		= 4			/**< sound are currently stopped by user call of Stop() method. */
};

/**
 *	Enum type which represents initial state of newly created sound.
 */
enum SoundStartState
{
	AZUSM_INSTANT		= 0,		/**< ordinal sound playback behaviour. */
	AZUSM_DELAYED		= 3			/**< playback of sound is delayed in time, only valid for AZUST_LOOP sounds 
									 *	 and prevent sound from default on-ready play. */
};

//-------------------------------------------------------------
// Declaration of main Azumaria Sound Engine class and its built-in structures
//-------------------------------------------------------------

/**
 *	Azumaria Engine main class.
 *	Provides methods for playing sounds by XACT API, which are packed in .xwb files
 *	and supplied by cuesheet playlists packed in .xsb files.
 */
class AzuEngine 
{
private:
	static AzuEngine* _self;							/**< pointer to only instance of this class. */
	static int _nRefCount;								/**< number of references created to only instance of this class. */

	/**
	 *	Class default contructor.
	 *	Only initializes class fields.
	 */
	AzuEngine();					

	/**
	 *	Class destructor.
	 *	Releases resources used by XACT Engine and disallocates memory dedicated for files and used by dynamical containers.
	 */
	~AzuEngine();							

private:
	/** 
	 *	Structure which holds XACT Engine variables.
	 */
	struct EngineState 
	{
		IXACT3Engine*		pEngine;					/**< pointer to XACT Engine interface. */

		HANDLE				hStreamingWaveBankFile;		/**< handle of loaded streaming wave bank file. */
		IXACT3WaveBank*		pStreamingWaveBank;			/**< pointer to interface of loaded streaming wave bank. */
		IXACT3WaveBank*		pInMemoryWaveBank;			/**< pointer to interface of loaded in-memory wave bank. */
		IXACT3SoundBank*	pSoundBank;					/**< pointer to interface of loaded sound bank. */

		CRITICAL_SECTION	cs;							/**< critical section. */

		PVOID				pbInMemoryWaveBank;			/**< pointer to file data loaded to memory from in-memory wave bank file. */
		PVOID				pbSoundBank;				/**< pointer to file data loaded to memory from sound bank file. */
	};
	EngineState _state;									/**<  instance of EngineState struct for current AzumariaEngine. */

	/**
	 *	Structure which holds data of loaded sound
	 */
	struct AzuSound 
	{
		CHAR		id[32];								/**< string id of sound (is taken from it's friendly name in sound bank cuelist). */
		int			type;								/**< type of sound - can be one of SoundType enum values. */
		int			state;								/**< current state of sound - can be one of SoundState enum values. */
		XACTINDEX	index;								/**< index of sound as it loaded into XACT Engine. */
	};
	std::vector<AzuSound*>	_pAzuSound;					/**< vector of AzuSound structs, containing data of all sounds loaded into AzumariaEngine. */

	HANDLE	_hThread;									/**< handle of thread executed to perform update of sound states. */
	bool	_bStreamingWBPrepared;						/**< identifies when streaming wave bank is prepared for streaming. */ 
	bool	_bDeployed;									/**< true if AzuEngine initalized correctrly, otherwise false. */ 

	/**
	 *	Function which handles XACT Engine notifications callback.
	 *	@param pNotification pointer to XACT_NOTIFICATION structure parsed in function.
	 */
	static	void __stdcall	XACTNotificationCallback( const XACT_NOTIFICATION* pNotification );

	/**
	 *	Update callback running in new thread.
	 *	Executes UpdateAudio method in every 10ms to 
	 *	@param dummy is meanless since it's just required by beginthread callback parameter.
	 */
	static	void UpdateCallback( PVOID dummy );
	/**
	 *	Register notification in XACT Engine.
	 *	@param flags are flags of notification parsed as field of XACT_NOTIFICATION_DESCRIPTION structure.
	 *	@param type are type of registering notification.
	 *	@param cueIndex are index of cuesheet affected by this type of notification (0 for most sound purposes, otherwise XACTINDEX_INVALID).
	 */
			void RegNotification( BYTE flags, XACTNOTIFICATIONTYPE type, XACTINDEX cueIndex );
	/**
	 *	Advanced method which checks sound states (and update it is needed) and performs routine work of XACT Engine.
	 */
			void UpdateAudio();					

public:
	/**
	 *	Method used for receiving pointer to instance of AzumariaEngine.
	 *	@return pointer to instance of AzumariaEngine.
	 */
	static	AzuEngine* Instance();	
	/**
	 *	Releases reference to AzumariaEngine.
	 */
			void		Release();			

	/**
	 *	Azumaria Engine initialize function.
	 *	Initializes XACT engine, WaveBanks and SoundBanks pointers, begins thread for sound updating and registers needed XACT notifications.
	 *	@return S_OK if initialization is successful, otherwise returns E_FAIL.
	 */
	HRESULT SetUp();						

	/** 
	 *	Loads WaveBank file which supposed to be in-memory WaveBank file.
	 *	@param xwbFName is null-terminated unicode string which contains path to .xwb file.
 	 *	@return S_OK if loading is successful, otherwise returns error code.	
	 */
	HRESULT AddInMemoryWB( WCHAR* xwbFName );
	/** 
	 *	Loads WaveBank file which supposed to be streaming WaveBank file.
	 *	@param xwbFName is null-terminated unicode string which contains path to .xwb file.
 	 *	@return S_OK if loading is successful, otherwise returns error code.
	 */
	HRESULT AddStreamingWB( WCHAR* xwbFName );
	/** 
	 *	Loads SoundBank file.
	 *	@param xsbFName is null-terminated unicode string which contains path to .xsb file.
	 *	@return S_OK if loading is successful, otherwise returns error code.
	 */
	HRESULT AddSB( WCHAR* xsbFName );

	/**
	 *	Loads sound from WaveBank into Azumaria Engine creating its play parameters.
	 *	WARNING! This method must be called after loading SoundBank and any type of WaveBank into Azumaria Engine.
	 *	NOTE: You can't load two sounds with same friendly names. Second sound will not be loaded into Azumaria Engine.
	 *	@param frName null-terminated one-byte string which contains friendly name of sound as it defined in SoundBank.
	 *	@param eType type of loading sound- can be one of SoundType enum values.
	 *	@param eState initial state of loading sound - can be one of SoundStartState enum values.
	 *	@return true if loading is successful, otherwise false
	 */
	bool LoadSound( PCSTR frName, SoundType eType = AZUST_DEFAULT, SoundStartState eState = AZUSM_INSTANT );

	/**
	 *	Deletes sound from Azumaria Engine.
	 *	@param frName null-terminated one-byte string which contains friendly name of sound to delete.
	 *	@return true if sound is deleted, false if Azumaria Engine isn't initalized yet or sound with specified friendly name isn't loaded into Azumaria Engine.
	 */
	bool DeleteSound( PCSTR frName );
	
	/**
	 *	Immediately plays sound with specified friendly name.
	 *	NOTE: Avoid crossing play of long-time sounds, use Stop before start playing of new long-time sound.
	 *	@param frName null-terminated one-byte string which contains friendly name of sound to play.
	 *	@return true if sound is successfully started to play, false if 
	 *	Azumaria Engine isn't initalized yet or sound with specified friendly name isn't loaded into Azumaria Engine.
	 */
	bool Play( PCSTR frName );	

	/**
	 *	Stops sound with specified friendly name from playing.
	 *	@param frName null-terminated one-byte string which contains friendly name of sound to stop.
	 */
	bool Stop( PCSTR frName );	

	/**
	 *	Not implemented yet.
	 */
	bool CycleTo( PCSTR frName );
};

//-------------------------------------------------------------

#endif