/* arquivos necessarios */
#ifndef _HTTP
#define _HTTP
#endif

#ifndef _Error
#include "Error.h"
#endif

#ifndef _RequestHeader
#include "RequestHeader.h"
#endif

#ifndef _GetExtension
#include "GetExtension.h"
#endif

#ifndef _DirectoryManager
#include "DirectoryManager.h"
#endif

/* bibliotecas */
#include <string>
#include <string.h>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

/* tamanbo max do BUFF*/
#define BUFFSIZE 8000000

using namespace std;

/* http request */
class HTTP
{
	public:
		RequestHeader* requestHeader;
		int responseLength;
		char* responseText;
		char* fileData;
		char* path;
		/* estrutura da http request */
		HTTP(RequestHeader* requestHeader)
		{
			this->requestHeader = requestHeader;
			this->responseText = new char[99999999];
			this->fileData = new char[99999999];
			this->path = new char[500];
		    getcwd(this->path, 255);
		    strcat(this->path, "/www/");
		    cout << this->path << endl;
		}
		/* executando o request http */
		char* execute(RequestHeader* requestHeader)
		{
			if(requestHeader->HTTPVersion == "HTTP/1.1")
			{
				if(requestHeader->method == "GET")
				{
					return this->doGet();
				}
				else if(requestHeader->method == "POST")
				{
					return this->doPost();
				}
				/* erro para atender a request */
				else
				{
					Error::printError(badRequest);
					return this->doBadRequest();
				}
			}
			else
			{
				Error::printError(versionNotSupported);
				return doVersionNotSupported();
			}
			Error::printError(badRequest);
			return doBadRequest();
		}
		char* doBadRequest();
		char* doVersionNotSupported();
		char* doNotFound();
		char* doGet();
		char* doPost();
		char* getData(int* dataLength);
		static bool isDirectory(const char* path);
		static bool isFile(const char* path);
		char* doGetDirectory();
		char* doGetFile();
};
