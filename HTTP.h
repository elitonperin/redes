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

#include <string>
#include <string.h>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#define BUFFSIZE 5000000

using namespace std;

class HTTP
{
	public:
		RequestHeader* requestHeader;

		int responseLength;

		char* responseText;

		char* fileData;

		char* path;

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
