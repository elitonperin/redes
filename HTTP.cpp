#ifndef _HTTP
#include "HTTP.h"
#endif


/* error 400, requisicao nao atendida pelo servidor */
char* HTTP::doBadRequest()
{
	strcpy(responseText, "HTTP/1.1 400 Bad Request\r\nContent-type: text/html\r\n\r\nBad Request");
	responseLength = strlen(responseText);
	return responseText;
}
/* erro 505, versao do protocolo nao suportada pelo servidor */
char* HTTP::doVersionNotSupported()
{
	strcpy(responseText, "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-type: text/html\r\n\r\nVersion Not Supported");
	responseLength = strlen(responseText);
	return responseText;
}
/* erro 404, documento da requisicao nao existe no servidor */
char* HTTP::doNotFound()
{
	strcpy(responseText, "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\n\r\nNot Found");
	responseLength = strlen(responseText);
	return responseText;
}
/* fazer o GET */
char* HTTP::doGet()
{
	cout << "Entrei na doGet\n";
	/* atender seja documento ou diretorio */
	if(isFile(requestHeader->requestURI.c_str()))
	{
		return doGetFile();
	}
	else if(isDirectory(requestHeader->requestURI.c_str()))
	{
		return doGetDirectory();
	}
	/* caso de erro */
	return doNotFound();
}

//todo:
/* fazer o POST */
char* HTTP::doPost()
{
	cout << "Entrei na doPost\n";
	int i = 0, j = 0;
	char* fieldName = new char[80];
	char* fieldResponse = new char[80];
	char* htmlResponse = new char [500];
	string ws = " ";

	strcpy(htmlResponse, "<html><body>");
	/* pegar todo o conteudo  */
	do
	{
		j = 0;
		/* pegar o nome do conteudo */
		while(requestHeader->messageBody[i] != '=')
		{
			fieldName[j] = requestHeader->messageBody[i];
			i++;
			j++;
		}
		fieldName[j] = '\0';

		i++;
		j = 0;
		/* exibe nome */
		cout << "\nFIELD NAME: " << fieldName;
		/* resposta */
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
		/* exibir o conteudo */
		cout << "\nFIELD RESPONSE: " << fieldResponse;
		strcat(htmlResponse, fieldName);

		strcat(htmlResponse, ": ");

		strcat(htmlResponse, fieldResponse);

		strcat(htmlResponse, "<br>");
	}
	while ((unsigned) i < requestHeader->messageBody.size());

	strcat(htmlResponse, "</body></html>");

	ofstream logFile(requestHeader->requestURI.c_str(), ios:: out);

	if (logFile.is_open())
	{
		logFile << htmlResponse << endl;

		logFile.close();
	}
	/* desalocar memoria */
	delete [] fieldName;
	delete [] fieldResponse;
	delete [] htmlResponse;

	return doGetFile();
}

/* pegar a data local */
char* HTTP::getData(int* dataLength)
{
	cout << "Entrei aqui na getData\n";
	strcpy(fileData, "");
	char c;
	ifstream file(requestHeader->requestURI.c_str(), ios::binary | ios::in);

	file.seekg (0, file.end);
	*dataLength = file.tellg();
	file.seekg (0, file.beg);

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
/* se for arquivo */
bool HTTP::isFile(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}
/* se for diretorio */
bool HTTP::isDirectory(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

//todo:
/* atender GET */
char* HTTP::doGetFile()
{
	cout << "Entrei na doGetFile\n";
	/* pegar os dados necessarios para o GET */
	strcpy(responseText, "");

	int dataLength;

	char* data = getData(&dataLength);

	char* crlf = new char[sizeof("\r\n")];

	strcpy(crlf, "\r\n");

	char* type = new char[GetExtension::getExtension(requestHeader->requestURI).size()];

	strcpy(type, GetExtension::getExtension(requestHeader->requestURI).c_str());

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
	/* conteudo do GET */
	for (int i = 0; i < dataLength; i++)
	{
		responseText[i + strLength] = data[i];
	}
	cout << "Chegou3\n";
	responseLength = strLength + dataLength;
	/* liberar memoria */
	delete [] crlf;
	delete [] type;
	delete [] contentType;
	delete [] charDataLength;
	/* exibir resposta */
	return responseText;
}

//todo:
/* atender POST */
char* HTTP::doGetDirectory()
{
	strcpy(responseText, "");
	//cout << "entrei na doGetDirectory\n";

	DirectoryManager directoryManager;

	if(directoryManager.openDirectory(requestHeader->requestURI))
	{
		//directoryManager.printDirectory();
		string html =  directoryManager.createHTML();
		directoryManager.closeDirectory();
		char length[15];
		sprintf(length, "%d", static_cast<int>(html.length()));
		string crlf = "\r\n";

		string response;
		/* conteudo do response */
		response = "HTTP/1.1 200 OK";
		response += crlf;
		response += "Content-Type: text/html";
		response += crlf;
		response += "Content-Length: ";
		response += length;
		response += crlf;
		response += crlf;
		response += html;
		sprintf(responseText,"%s", response.c_str());
		cout << responseText<<'\n';

		responseLength = strlen(responseText);
		/* exbir response */
		return responseText;
	}
	else
		return doNotFound();
}
