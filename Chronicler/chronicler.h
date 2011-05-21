#include <stdio.h>
#include <time.h>
#include <string.h>

#define LOGP_STD 0x

class LogClass {
private:
        FILE* _logfile;
 //       long _prpts;
public: 
        LogClass(char *fname);
		~LogClass() { fclose(_logfile); };
        void NewLogfile(char *fname);
 //       Set(LOG_PROPERTY);
        void Write(char* str);              
};

LogClass::LogClass(char *fname)
{
	char logname[80];
	time_t t = time(0);
	strcpy(logname, fname);
	//strcat(logname, ctime(&t));
	strcat(logname,".txt");
	_logfile = fopen(logname, "at");
	fprintf(_logfile, "Logging started\n");
}
