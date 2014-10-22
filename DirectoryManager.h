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

		void printDirectory()
		{
			while((directoryInfo = readdir(directoryPointer)))
			{
				cout << directoryInfo->d_name << '\n';
			}

		}

		bool isFile(const char* path) {
		    struct stat buf;
		    stat(path, &buf);
		    return S_ISREG(buf.st_mode);
		}

		bool isDirectory(const char* path) {
		    struct stat buf;
		    stat(path, &buf);
		    return S_ISDIR(buf.st_mode);
		}

		string createHTML(string path)
		{

			string html = "<!DOCTYPE html>\n<body>\n<h1>Index of</h1>\n";
			string aux;

			while((directoryInfo = readdir(directoryPointer)))
			{

				cout << directoryInfo->d_name;
				html+= "<li>";
				html+="<a href=";
				html+= path;

				aux = directory + directoryInfo->d_name;


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

		void closeDirectory()
		{
			closedir(directoryPointer);
		}

};
