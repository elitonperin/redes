/* arquivos necessarios */
#ifndef _DirectoryManager
#define _DirectoryManager
#endif

/* bibliotecas */
#include <dirent.h>
#include <string>
#include <sys/stat.h>

using namespace std;

/* verificar os diretorios */
class DirectoryManager
{
	private:
		/* ponteiro para um diretorio */
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
		/* diretorios existentes */
		void printDirectory()
		{
			while((directoryInfo = readdir(directoryPointer)))
			{
				cout << directoryInfo->d_name << '\n';
			}
		}
		/* verifica se eh arquivo */
		bool isFile(const char* path) {
		    struct stat buf;
		    stat(path, &buf);
		    return S_ISREG(buf.st_mode);
		}
		/* verifica se eh diretorio */
		bool isDirectory(const char* path) {
		    struct stat buf;
		    stat(path, &buf);
		    return S_ISDIR(buf.st_mode);
		}
		/* html de resposta */
		string createHTML(string path)
		{
			string html = "<!DOCTYPE html>\n<body>\n<h1>Index of</h1>\n";
			string aux;
			while((directoryInfo = readdir(directoryPointer)))
			{
				aux = directory + directoryInfo->d_name;
				cout << directoryInfo->d_name;
				html+= "<li>";
				if(isDirectory(aux.c_str()))
				{
					html+="/";
				}
				html+="<a href=";
				html+= path;
				html+= directoryInfo->d_name;
				if(isDirectory(aux.c_str()))
				{
					html+='/';
				}
				html+=">";
				html+=directoryInfo->d_name;
				html+="</a>";
				html+="\n";
			}
			html += "</ul>\n</body>\n</html>\n";
			return html;
		}
		/* fechar o diretorio */
		void closeDirectory()
		{
			closedir(directoryPointer);
		}
};
