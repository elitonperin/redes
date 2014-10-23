#ifndef _RequestHeader
#define _RequestHeader
#endif

/* bibliotecas */
#include <map>
#include <string>

using namespace std;

typedef map< string , string> Map;

/* construtor do header, inserir novos parametros do request */
class RequestHeader
{
	public:
		/* Para esta implementação o método pode ser GET ou POST */
		string method;
		string requestURI;
		/* Aonde serão guardados os parâmetros do request através de uma relação chave-valor(parâmetro-valor) */
		Map headerFields;
		string messageBody;
		string HTTPVersion;

		/* Construtor que recebe string
		\param method da requisição (GET ou POST)
		\param requestURI endereço da requisição
		*/
		RequestHeader(string method, string requestURI)
		{
			this->requestURI = requestURI;
			this->method =  method;
		}
		/* Construtor que recebe char*
		\param method da requisição (GET ou POST)
		\param requestURI endereço da requisição
		*/
		RequestHeader(char* method, char* requestURI)
		{
			this->requestURI = string(requestURI);
			this->method = string(method);
		}

		/**Insere novos parâmetros do request
		\param key Parâmetro a ser adicionado
		\ param value Valor do parâmetro a ser adicionado
		*/
		void insert(string key, string value)
		{
			headerFields.insert(make_pair(key,value));
		}

		/* Recebe dado opcional	*/
		void getMessageBody(string messageBody)
		{
			this->messageBody = messageBody;
		}
};
