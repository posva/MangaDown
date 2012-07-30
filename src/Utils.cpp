#include "Utils.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#if defined(_WIN32) || defined(__WIN32__)
// Windows
#define SYS_WINDOWS
#include <direct.h>     // _mkdir

#elif defined(linux) || defined(__linux)
// Linux
#define SYS_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
// Mac OS
#define SYS_MACOS

#endif

bool directoryExists(const std::string& dir)
{
	struct stat buf;
	if (stat(dir.c_str(), &buf) == 0)
	{
		return (buf.st_mode & S_IFDIR) != 0;
	}
	return false;
}


bool createDirectory(const std::string& dir)
{
	bool success = false;
	
#ifdef SYS_WINDOWS
	success = _mkdir(dir.c_str()) == 0;
#elif defined(SYS_LINUX) || defined(SYS_MACOS)
	success = mkdir(dir.c_str(), 0755) == 0;
#endif
	
	if (!success)
		std::cout<<"Cannot create directory "<<dir<<std::endl;
	
	return success;
	
}

int getFiles(const std::string &dir, std::vector<std::string> &files, const std::string& ext)
{
	DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        printf("Cannot get acces to dir\n");
        return 1;
    }
	
	files.clear();
	
    while ((dirp = readdir(dp)) != NULL) {
		if (strstr(dirp->d_name, ext.c_str()) != NULL)
			files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;

}