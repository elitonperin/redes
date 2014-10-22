//http://www.dreamincode.net/forums/topic/59943-accessing-directories-in-cc-part-i/

#ifndef _DirectoryManager
#define _DirectoryManager
#endif

#include <dirent.h>
#include <string>
#include <sys/stat.h>

using namespace std;
class DirectoryManager
{
	private:
		//ponteiro para um diretorio
		DIR* directoryPointer;
		struct dirent *directoryInfo;
		string directory;
	public:
		DirectoryManager()
		{
			directoryInfo = 0;
			directoryPointer = 0;
		}
		/* abrir o diretorio */
		bool openDirectory(string directory)
		{
			this->directory = directory;
			directoryPointer = opendir(directory.c_str());
			if(directoryPointer == 0)
			{
				Error::printError(openDir);
				return false;
			}
			return true;
		}
		/* nome do diretorio */
		void printDirectory()
		{
			while((directoryInfo = readdir(directoryPointer)))
			{
				cout << directoryInfo->d_name << '\n';
			}

		}
		/* verifica se eh diretorio, pelo caminho */
		bool isDirectory(const char* path) {
		    struct stat buf;
		    stat(path, &buf);
		    return S_ISDIR(buf.st_mode);
		}

		/* criar o html */
		string createHTML()
		{
			string html = "<!DOCTYPE html>\n<body>\n<h1>Index of</h1>\n";
			string aux;
			/* ponteiro para o diretorio */
			while((directoryInfo = readdir(directoryPointer)))
			{
				aux = directory + directoryInfo->d_name;
				/* pegar o nome do diretorio */
				cout << directoryInfo->d_name;
				html+= "<li>";
				html+="<a href=";
				html+= directory;
				html+= directoryInfo->d_name;

				if(isDirectory(aux.c_str()))
				{
					html+="/";
				}

				html+=">";
				html+=directoryInfo->d_name;
				html+="</a>";
				html+="\n";

			}
			html += "</ul>\n</body>\n</html>\n";
			return html;
		}
		/* fechar o diretorio, liberar memoria */
		void closeDirectory()
		{
			closedir(directoryPointer);
		}



};
