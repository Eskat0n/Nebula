
#include <windows.h>

#ifndef _FRAMER_CLASS_
#define _FRAMER_CLASS_

//-------------------------------------------------------------
// Main definitions
//-------------------------------------------------------------

/**
 * Defines pointer to function which takes no arguments and returns bool value (typical callback used in Framer subsystem).
 */
typedef bool ( *SbrXCallback )();

#define SBR_FPS_INFINITE 0

// Input type definitions
#define SBRET_KEYDN		1		
#define SBRET_KEYUP		2		
#define SBRET_MBTNDN	3	
#define SBRET_MBTNUP	4	
#define SBRET_MMOVE		5	

// Key codes definitions (only needed keys for now)

// Mouse buttons
#define SBRK_LBTN	0x01
#define SBRK_RBTN	0x02

// Control keys
#define SBRK_SHIFT	0x10
#define SBRK_CTRL	0x11
#define	SBRK_ALT	0x12

#define SBRK_ENTER	0x0D
#define SBRK_ESC	0x1B
#define SBRK_SPACE	0x20

// Arrows
#define SBRK_LEFT	0x25
#define SBRK_UP		0x26
#define SBRK_RIGHT	0x27
#define SBRK_DOWN	0x28

// Alphabet keys
#define SBRK_A			0x41
#define SBRK_B			0x42
#define SBRK_C			0x43
#define SBRK_D			0x44
#define SBRK_E			0x45
#define SBRK_F			0x46
#define SBRK_G			0x47
#define SBRK_H			0x48
#define SBRK_I			0x49
#define SBRK_J			0x4A
#define SBRK_K			0x4B
#define SBRK_L			0x4C
#define SBRK_M			0x4D
#define SBRK_N			0x4E
#define SBRK_O			0x4F
#define SBRK_P			0x50
#define SBRK_Q			0x51
#define SBRK_R			0x52
#define SBRK_S			0x53
#define SBRK_T			0x54
#define SBRK_U			0x55
#define SBRK_V			0x56
#define SBRK_W			0x57
#define SBRK_X			0x58
#define SBRK_Y			0x59
#define SBRK_Z			0x5A

// Other keys
#define SBRK_PAUSE		0x13

//-------------------------------------------------------------
// Declaration of support structures
//-------------------------------------------------------------

/**
 * Struct which holds data about any input event happened in the game window.
 */
struct SbrInputEvent
{
	int				type;				/**< event type */
	int				key;				/**< key code */
//	int				reserved;			/**< event flags */
	int				chr;				/**< character code */
	float			x;					/**< x coord of mouse cursor if needed */
	float			y;					/**< y coord of mouse cursor if needed */
	SbrInputEvent*	pNext;				/**< pointer to next event in list */
};

//-------------------------------------------------------------
// Main SbrXFramer class declaration
//-------------------------------------------------------------

/**
 * Class which handles frame changing and user input actions from frate to frame.
 * Originally it was a part of SaberGE, but was separated because of difference between input and graphic.
 */
class SbrXFramer
{
private:						
	static	SbrXFramer*		_self;						/**< pointer to only instance of this class. */
	static	int				_nRefCount;					/**< number of references created to only instance of this class. */

	/**
	 * Default constructor.
	 */
	SbrXFramer();
	/**
	 * Destructor.
	 */
	~SbrXFramer();

	SbrXCallback			_pGameLogicFunc;		/**< Pointer to function which invokes on every frame and represents game logic. */
	SbrXCallback			_pGameRenderFunc;		/**< Pointer to function which invokes on every frame and represents game graphic render logic. */

	BOOL					_bRunning;				/**< True if message loop has been started, otherwise false. */

	INT						_nOverallFPS;			/**< Overall value of FPS. */
	FLOAT					_fTime;					/**< Current time since creation of Saber Framer (in secs). */
	FLOAT					_fTimeDelta;			/**< Time elapsed since beginning of previous frame.*/
	DWORD					_nFixedDelta;			/**< Minimum time between two nearest frames. */	
	INT						_nFPS;					/**< Number of FPS Framer will try to preserve. */
	DWORD					_t0;					/**< Time of current frame. */
	DWORD					_t0fps;					/**< Time since of last _cfps counter' nullification. */
	DWORD					_dt;					/**< Temp parameter. */
	INT						_cfps;					/**< Number of fps showed during current second. */

	FLOAT					_posX;					/**< Current x position of mouse' cursor (not propertly used yet). */
	FLOAT					_posY;					/**< Current y position of mouse' cursor (not propertly used yet). */
	CHAR					_keys[256];				/**< Array wich hold data of key's pressing and releasing during frame. */

	SbrInputEvent*			_pInputQueue;			/**< Pointer to first event element in the input queue. */

	/**
	 * Clears queue of input events.
	 */
	void			_ClearQueue();	
	/**
	 * Makes event from its parameters and passes it to the end of the queue.
	 * @param evType type of the event.
	 * @param key code of pressed/released key.
	 * @param scancode of pressed/released key.
	 * @param x current mouse' cursor position alongside of x axis.
	 * @param y current mouse' cursor position alongside of y axis.
	 */
	void			_MakeEvent( int evType, int key, int scan, int x = -1, int y = -1 );

public:
	/**
	 * Method used for receiving pointer to instance of the Saber Framer.
	 *	@return pointer to instance of Saber Framer.
	 */
	static		SbrXFramer*		Instance();
	/**
	 *	Releases reference to the Saber Framer.
	 */
				void			Release();

	/**
	 * Sets number of FPS Framer will try to preserve..
	 * @param FPS number of FPS.
	 */
	void			SetFPS( int FPS );
	/**
	 * Sets function which helds game logic.
	 * Game logic function must be setted before calling to EnterMessageLoop() method.
	 * @param value pointer to function.
	 */
	void			SetLogicFunc( SbrXCallback value );
	/**
	 * Sets function which helds game logic.
	 * @param value pointer to function.
	 */
	void			SetRenderFunc( SbrXCallback value );

	/** 
	 * Makes event from WinAPI message.
	 * Valid only if there is instance of Saber Framer created.
	 * Used to plug into SaberGE and process it's messages.
	 */
	static	void	MakeEventFromMSG( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	/**
	 * Performs entering to message loop, which provides frame changing and invokation of game logic function.
	 * @return true if message loop is succesfully ended; false if game logic function isn't defined yet.
	 */
	bool			EnterMessageLoop();

	/**
	 * Determines what specified key is pressed during current frame or not.
	 * @param key code of key (defined alias).
	 * @return true if specified key is been pressed, otherwise false.
	 */
	bool			IsKeyDown( int key );
	/**
	 * Determines what specified key is released during current frame or not.
	 * @param key code of key (defined alias).
	 * @return true if specified key is been released, otherwise false.
	 */
	bool			IsKeyUp( int key );

	/**
	 * Returns time elapsed since creation of Saber Framer.
	 * @return time in secs.
	 */
	float			GetTime() { return _fTime; }
	/**
	 * Returns time elapsed since beginning of previous frame.
	 * @return time in secs.
	 */
	float			GetDelta() { return _fTimeDelta; }
	/**
	 * Returns current value of FPS.
	 * @return value of FPS.
	 */
	int				GetFPS() { return _nFPS; }
};

//-------------------------------------------------------------

#endif