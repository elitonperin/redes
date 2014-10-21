#ifndef _ServerLog
#define _ServerLog
#endif

#ifndef _RequestHeader
#include "RequestHeader.h"
#endif

#include <fstream>

class ServerLog
{
	public:
		static void saveLog(RequestHeader* requestHeader, char* clientIP)
		{
			fstream logFile("/home/douglas/serverLog.txt", ios:: out | ios::app);

			if (logFile.is_open())
			{
				logFile << "Request Received" << endl;
				logFile << "IP: " << clientIP << endl;
				logFile << "Method : " << requestHeader->method << endl;
				logFile << "URI Requested : " << requestHeader->requestURI << endl;
				logFile << "Date and Time : " << ServerLog::currentDateTime() << endl;
				logFile << "=======================================================" << endl;

				logFile.close();
			}
		}

		static string currentDateTime() {
		    time_t     now = time(0);
		    struct tm  tstruct;
		    char       buf[80];
		    tstruct = *localtime(&now);
		    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		    // for more information about date/time format
		    strftime(buf, sizeof(buf), "%d-%m-%Y - %X", &tstruct);

		    return buf;
		}
};
