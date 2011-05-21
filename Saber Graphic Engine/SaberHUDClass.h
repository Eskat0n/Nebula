//-------------------------------------------------------------
// Saber Graphic Engine HUD object classes declaration file
// Written by Eskat0n
// Current engine version: 0.0.8
// Modified: 6 nov 2008
//-------------------------------------------------------------

#include "SaberGE_Dispatcher.h"
#include <strsafe.h>

#ifndef	_SBR_HUD_CLASSES_
#define _SBR_HUD_CLASSES_

//-------------------------------------------------------------

/**
 * Abstract concept of any GUI (or HUD in game terms) object which appears on the screen.
 * Counters, messages, bars are goes here.
 */ 
class SbrHUDObject
{
protected:
	ISaberGE*			_pSbr;					/**< Pointer to SaberGE guarding proxy. */
	
	RECT				_rcFontPos;				/**< Rectangle structure which determines font position on the screen. */
	ID3DXFont*			_pFont;					/**< Pointer to font used to print messages. */
	INT					_nFontSize;				/**< Size of used font. */
	WCHAR				_wsFontName[256];		/**< Facename of currently used font. */
	DWORD				_dwFontColor;			/**< Color of used font. */
	BOOL				_bVisible;				/**< True is object are drawable for now, otherwise false. */

public:
	/**
	 * Default constructor.
	 */
	SbrHUDObject();
	/** 
	 * Destructor.
	 */
	~SbrHUDObject();

	/**
	 * Renders HUDObject on the screen. Must be called between calls to ISaberGE::BeginScene and ISaberGE::EndScene methods.
	 * @return true if rendering is succesful, otherwise false.
	 */
	virtual		bool	Draw() = 0;
	/**
	 * Sets position on the screen for this object.
	 * NOTE: position is setted for upper-left corner of object' enclosing rectangle.
	 */
				void	SetPos( int x, int y );
	/** 
	 * Sets font for object' text.
	 * @param wsFacename unicode null-terminated string which contains facename of new font.
	 * @param nSize size of new font.
	 * @return true if font is succesfully setted.
	 */
				bool	SetFont( LPCWSTR wsFacename, int nSize );
	/**
	 * Sets color of font used for printing message.
	 * @param dwColor 32-bit value of color.
	 */
				void	SetFontColor( DWORD dwColor ) { _dwFontColor = dwColor; }
	/**
	 * Allows rendering of the object on the screen.
	 */
				void	Show() { _bVisible = true; }
	/**
	 * Forbids rendering of the object on the screen.
	 */
				void	Hide() { _bVisible = false; }
};

//-------------------------------------------------------------

/**
 * Represents any text message appeared on the screen.
 * Used for game interface messages.
 */
class SbrHUDMessage: public SbrHUDObject
{
protected:
	WCHAR				_wsText[256];			/**< String contaning message text. */

public:
	/**
	 * Default constructor.
	 */
	SbrHUDMessage();
	/**
	 * Destructor.
	 */
	~SbrHUDMessage() { }

	bool		Draw();

	/** 
	 * Sets the text for this message.
	 * @param wsTest unicode null-terminated string which contains text to be setted.
	 */
	void		SetText( LPCWSTR wsText );
	/**
	 * Returns pointer to message text.
	 * @return pointer to unicode string.
	 */
	LPWSTR		GetText() const;
};

//-------------------------------------------------------------

/**
 * Counter which store DWORD value and show it in text format on the screen.
 */
class SbrHUDCounter: public SbrHUDObject
{
protected:
	DWORD				_dwValue;				/**< Value stored in this counter. */

public:
	/**
	 * Default constructor.
	 */
	SbrHUDCounter();
	/**
	 * Destructor.
	 */
	~SbrHUDCounter() { }

	bool		Draw();

	/** 
	 * Sets value stored by counter.
	 * @param dwValue new counter value.
	 */
	void		SetValue( DWORD dwValue ) { _dwValue = dwValue; };
	/**
	 * Return current counter' value.
	 * @return current counter' value.
	 */
	DWORD		GetValue() const { return _dwValue; }
};

//-------------------------------------------------------------

#endif