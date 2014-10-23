/* bibliotecas */
#include <stdio.h>          /* printf */
#include <stdlib.h>         /* exit */
#include <string.h>         /* bzero */
#include <sys/socket.h>     /* recv, send */
#include <arpa/inet.h>      /* struct sockaddr */
#include <unistd.h>         /* exit */
#include <signal.h>			/* comunicacao entre processos */
#include <sys/wait.h>		/* waitpid */

/*
Adicionar arquivos:
	git add .
	git commit -m hue
	git push -u origin master

Pegar:
	git pull origin master
*/
/*
 http://127.0.0.1:8081 -> GET /

 http://127.0.0.1:8081/home/karan/Documents/teste.txt -> GET /home/karan/Documents/teste.txt
 * */

/*
Para compilar: g++ -Wall -std=c++11 -o main main.cpp
Para executar: ./main [porta]
*/

/* define variaveis constantes e seus valores */
#define MAXPENDING 5

/* include dos arquivos */
#ifndef _Error
#include "Error.h"
#endif

#ifndef _RequestHeader
#include "RequestHeader.h"
#endif

#ifndef _HTTP
#include "HTTP.h"
#endif

#ifndef _ParserHTTP
#include "ParserHTTP.h"
#endif

#ifndef _ServerLog
#include "ServerLog.h"
#endif

#define SA struct sockaddr

/* thread status */
void sig_chld(int sinal)
{
	pid_t pid;
	int stat;

	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		printf("filho %d terminou com estado %d.\n", pid, stat);
	}
	return;
}

/* funcao principal */
int main(int argc, char** argv)
{
	/* variaveis locais */
	int listenfd, connfd, n;
	unsigned int clientlen;
	char buffer[BUFFSIZE];
	struct sockaddr_in servaddr, client;
	/* funcao para especificar o tipo do protocolo */
	if((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		Error::printError(createSocket);
		return 0;
	}
	char port[7];
	if(argc == 2)
	{
		strcpy(port, argv[1]);
	}
	else
	{
		strcpy(port, "8080");
	}
	/* populando os dados do servidor */
	/* zera a estrutura que armazena os dados */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      	= AF_INET;
	/* aceita qualquer fixa de IP que a maquina possa responder */
	servaddr.sin_addr.s_addr 	= htonl(INADDR_ANY);
	/* define a porta */
	servaddr.sin_port        	= htons(atoi(port));
	/* vincula um socket a um endereco */
	if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
	{
		Error::printError(bindSocket);
		return 0;
	}
	/* estipula a fila para o servidor */
	if(listen(listenfd, MAXPENDING) < 0)
	{
		Error::printError(listenSocket);
		return 0;
	}
	/* trata os filhos zumbis */
	signal(SIGCHLD, sig_chld);
	/* laco do servidor */
	int pid;
	while(true)
	{
		n = 0;
		clientlen = sizeof(client);
		/* aceita a conexao com o cliente */
		if((connfd = accept(listenfd, (SA *) &client, &clientlen)) < 0)
		{
			Error::printError(acceptConnection);
			return 0;
		}
		if((pid = fork()) == 0)
		{
			/* recebendo protocolo http do cliente */
			if((n = recv(connfd, buffer, BUFFSIZE, 0)) == -1)
			{
				Error::printError(receiveData);
				return 0;
			}
			/* parser http */
			RequestHeader* requestHeader = ParserHTTP::execute(buffer);
			ServerLog::saveLog(requestHeader, inet_ntoa(client.sin_addr));
			/* http request */
			HTTP* http = new HTTP(requestHeader);
			/* response */
			char* responseText = http->execute(requestHeader);
			cout << "\nResponse: \n" << responseText << endl;
			do
			{
				if (http->responseLength < BUFFSIZE)
					n = send(connfd, responseText, http->responseLength, 0);
				else
					n = send(connfd, responseText, BUFFSIZE, 0);
			}
			while(n > 0);
			cout << "\nTerminou\n";
		}
		close(connfd);
	}
	return 0;
}
