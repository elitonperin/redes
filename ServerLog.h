/* arquivos utilizados */
#ifndef _ServerLog
#define _ServerLog
#endif

#ifndef _RequestHeader
#include "RequestHeader.h"
#endif

/* bibliotecas utilizadas*/
#include <fstream>

/* captar o log das requisicoes */
class ServerLog
{
	public:
		static void saveLog(RequestHeader* requestHeader, char* clientIP)
		{
			/* onde serah escrito os dados do log */
			fstream logFile("serverLog.txt", ios:: out | ios::app);
			/* os dados que serao guardados no arquivo de log */
			if (logFile.is_open())
			{
				logFile << "Request Received" << endl;
				logFile << "IP: " << clientIP << endl;
				logFile << "Method : " << requestHeader->method << endl;
				logFile << "URI Requested : " << requestHeader->requestURI << endl;
				logFile << "Date and Time : " << ServerLog::currentDateTime() << endl;
				logFile << "=======================================================" << endl;
				/* fechar o arquivo */
				logFile.close();
			}
		}

		/* pegar data e hora local */
		static string currentDateTime() {
		    time_t     now = time(0);
		    struct tm  tstruct;
		    char       buf[80];
		    tstruct = *localtime(&now);
		    strftime(buf, sizeof(buf), "%d-%m-%Y - %X", &tstruct);

		    return buf;
		}
};
