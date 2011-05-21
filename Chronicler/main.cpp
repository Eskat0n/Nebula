#include <conio.h>
#include "chronicler.h" 
#include <dos.h>

int main()
{
	LogClass mylog("TestLog");
	time_t t = time(0);
	printf("%s\n",ctime(&t));
	struct date d;
	getdate(&d);
	printf("Day: %d\n", d.da_day)
	
    _getch();
}
