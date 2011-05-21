//-------------------------------------------------------------

#include <time.h>
#include <windows.h>

#ifndef _SBRX_RANDOM_
#define _SBRX_RANDOM_

//-------------------------------------------------------------
// Macros
//-------------------------------------------------------------

#define DTRAD(a) ( 3.14 / 180 ) * ##a
#define TOPERC(a, m) ( ##a / ##m ) * 100.0f

//-------------------------------------------------------------

class SbrXRandom
{
public:
	static void Seed( unsigned int value = 0 )
	{
		if ( value )
			srand( value );
		else
			srand( (unsigned int)time( 0 ) );
	}

	static float RandomChance()
	{
		return ( rand() % 10000 ) / 100.0f;
	}
};

//-------------------------------------------------------------

class SbrXMath
{
private:
	SbrXMath() { }
public:
	static bool Around( float value, float x, float delta )
	{
		return ( abs( value - x ) <= delta );
	}
};

//-------------------------------------------------------------

#endif