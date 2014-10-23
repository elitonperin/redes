#ifndef _HTTP
#include "HTTP.h"
#endif

/* erro 400 bad request, requisicao nao atendida */
char* HTTP::doBadRequest()
{
	char* charDataLength = new char[sizeof("%d")];

	sprintf(charDataLength, "%d", (int) strlen("Bad Request"));

	strcpy(responseText, "HTTP/1.1 400 Bad Request\r\nContent-type: text/html\r\nContent-Length: ");

	strcat(responseText, charDataLength);

	strcat(responseText, "\r\n\r\nBad Request");

	responseLength = strlen(responseText);

	delete charDataLength;

	return responseText;
}
/* erro 505, versao do protocolo de requisicao nao suportada pelo servidor*/
char* HTTP::doVersionNotSupported()
{
	char* charDataLength = new char[sizeof("%d")];

	sprintf(charDataLength, "%d", (int) strlen("Version Not Supported"));

	strcpy(responseText, "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-type: text/html\r\nContent-Length: ");

	strcat(responseText, charDataLength);

	strcat(responseText, "\r\n\r\nVersion Not Supported");

	responseLength = strlen(responseText);

	delete charDataLength;

	return responseText;
}
/* erro 404 bad request, requisicao nao atendida pelo servidor */
char* HTTP::doNotFound()
{
	char* charDataLength = new char[sizeof("%d")];

	sprintf(charDataLength, "%d", (int) strlen("Not Found"));

	strcpy(responseText, "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\nContent-Length: ");

	strcat(responseText, charDataLength);

	strcat(responseText, "\r\n\r\nNot Found");

	responseLength = strlen(responseText);

	delete charDataLength;

	return responseText;
}
/* atendendo o get */
char* HTTP::doGet()
{
	cout << "Entrei na doGet\n";
	string path = this->path + requestHeader->requestURI;
	if(isFile(path.c_str()))
	{
		return doGetFile();
	}
	else if(isDirectory(path.c_str()))
	{
		return doGetDirectory();
	}

	return doNotFound();
}

/* atendendo o post */
char* HTTP::doPost()
{
	cout << "Entrei na doPost\n";
	int i = 0, j = 0;
	char* fieldName = new char[80];
	char* fieldResponse = new char[80];
	char* htmlResponse = new char [500];
	char* charDataLength = new char[sizeof("%d")];
	string ws = " ";

	strcpy(responseText, "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-Length: ");

	/* pegar o response html */
	strcpy(htmlResponse, "\r\n\r\n<html><body>");
	/* enquanto nao atender toda a requisicao */
	do
	{
		j = 0;
		/* nome do conteudo */
		while(requestHeader->messageBody[i] != '=')
		{
			fieldName[j] = requestHeader->messageBody[i];
			i++;
			j++;
		}
		fieldName[j] = '\0';
		i++;
		j = 0;
		/* exibir nome do conteudo header*/
		cout << "\nFIELD NAME: " << fieldName;
		/* conteudo response */
		while(requestHeader->messageBody[i] != '&' && requestHeader->messageBody[i] != '\0')
		{
			if (requestHeader->messageBody[i] == '+')
				requestHeader->messageBody[i] = ws[0];

			fieldResponse[j] = requestHeader->messageBody[i];
			i++;
			j++;
		}
		fieldResponse[j] = '\0';
		i++;
		/* exibir conteudo response header */
		cout << "\nFIELD RESPONSE: " << fieldResponse;
		strcat(htmlResponse, fieldName);

		strcat(htmlResponse, ": ");

		strcat(htmlResponse, fieldResponse);

		strcat(htmlResponse, "<br>");
	}
	while ((unsigned) i < requestHeader->messageBody.size());

	strcat(htmlResponse, "</body></html>");

	sprintf(charDataLength, "%d", (int) strlen(htmlResponse));

	strcat(responseText, charDataLength);

	strcat(responseText, htmlResponse);

	delete fieldName;
	delete fieldResponse;
	delete htmlResponse;
	delete charDataLength;

	return responseText;
}
/* pegar a data */
char* HTTP::getData(int* dataLength)
{
	cout << "Entrei aqui na getData\n";
	strcpy(fileData, "");
	char c;
	string path = this->path + requestHeader->requestURI;
	ifstream file(path.c_str(), ios::binary | ios::in);

	file.seekg (0, file.end);
	*dataLength = file.tellg();
	file.seekg (0, file.beg);

	if (*dataLength > BUFFSIZE)
		*dataLength = BUFFSIZE;

	if (file.is_open())
	{
		for (int i = 0; i < *dataLength; i++)
		{
			file.get(c);

			fileData[i] = c;
		}

		file.close();
	}
	return fileData;
}
/* verificar se eh arquivo */
bool HTTP::isFile(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}
/* verificar se eh diretorio */
bool HTTP::isDirectory(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

//todo:
/* manipular o arquivo */
char* HTTP::doGetFile()
{
	cout << "Entrei na doGetFile\n";

	/* pegar dados do header */
	strcpy(responseText, "");

	int dataLength;

	char* data = getData(&dataLength);

	char* crlf = new char[sizeof("\r\n")];

	strcpy(crlf, "\r\n");

	char* type = new char[GetExtension::getExtension(path + requestHeader->requestURI).size()];

	strcpy(type, GetExtension::getExtension(path + requestHeader->requestURI).c_str());

	char* contentType = new char[GetExtension::getMIME(type).size()];

	strcpy(contentType, GetExtension::getMIME(type).c_str());

	strcpy(responseText, "HTTP/1.1 200 OK");

	strcat(responseText, crlf);

	strcat(responseText, "Content-Type: ");

	strcat(responseText, contentType);

	strcat(responseText, crlf);

	strcat(responseText, "Content-Length: ");

	char* charDataLength = new char[sizeof("%d")];

	sprintf(charDataLength, "%d", dataLength);

	strcat(responseText, charDataLength);

	strcat(responseText, crlf);

	strcat(responseText, crlf);

	cout << "Chegou2\n";

	int strLength = strlen(responseText);

	if (dataLength > (BUFFSIZE - strLength))
		dataLength = BUFFSIZE - strLength;
	/* gerar resultados */
	for (int i = 0; i < dataLength; i++)
	{
		responseText[i + strLength] = data[i];
	}
	cout << "Chegou3\n";
	responseLength = strLength + dataLength;
	/* desalocar memoria */
	delete crlf;
	delete type;
	delete contentType;
	delete charDataLength;
	/* exibir resultados */
	return responseText;
}

//todo:
/* manipular o diretorio */
char* HTTP::doGetDirectory()
{
	strcpy(responseText, "");
	//cout << "entrei na doGetDirectory\n";

	DirectoryManager directoryManager;

	if(directoryManager.openDirectory(path + requestHeader->requestURI))
	{
		//directoryManager.printDirectory();

		/* pegar dados */
		string html =  directoryManager.createHTML(requestHeader->requestURI);
		directoryManager.closeDirectory();
		char length[15];
		sprintf(length, "%d", static_cast<int>(html.length()));
		string crlf = "\r\n";

		string response;
		/* response para o diretorio */
		response = "HTTP/1.1 200 OK";
		response += crlf;
		response += "Content-Type: text/html";
		response += crlf;
		response += "Content-Length: ";
		response += length;
		response += crlf;
		response += crlf;
		response += html;

		/* exibir os resultados */
		sprintf(responseText,"%s", response.c_str());
		cout << path + requestHeader->requestURI << endl;
		cout << responseText<<'\n';

		responseLength = strlen(responseText);
		/* exibir os resultados */
		return responseText;
	}
	else
		return doNotFound();
}
