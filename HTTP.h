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
#include <stdlib.h>
/* tamaho do buff size */
#define BUFFSIZE 5000000

using namespace std;

class HTTP
{
	public:
		RequestHeader* requestHeader;

		int responseLength;

		char* responseText;

		char* fileData;
		/* tamanhos */
		HTTP(RequestHeader* requestHeader)
		{
			this->requestHeader = requestHeader;
			this->responseText = new char[99999999];
			this->fileData = new char[99999999];
		}
		/* http GET e POST */
		char* execute(RequestHeader* requestHeader)
		{

			if(requestHeader->HTTPVersion == "HTTP/1.1")
			{
				/* GET */
				if(requestHeader->method == "GET")
				{
					return this->doGet();
				}
				/* POST */
				else if(requestHeader->method == "POST")
				{
					return this->doPost();
				}
				/* se nao der, bad request */
				else
				{
					Error::printError(badRequest);
					return this->doBadRequest();
				}
			}
			/* se nao der, versao nao suportada */
			else
			{
				Error::printError(versionNotSupported);
				return doVersionNotSupported();
			}
			/* bad request */
			Error::printError(badRequest);
			return doBadRequest();
		}

		char* doBadRequest();

		char* doVersionNotSupported();

		char* doNotFound();

		char* doGet();

		char* doPost();

		char* getData(int* dataLength);

		bool isDirectory(const char* path);

		bool isFile(const char* path);

		char* doGetDirectory();

		char* doGetFile();

};
