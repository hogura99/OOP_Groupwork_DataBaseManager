#pragma once

#ifdef WIN32
#include <io.h>
#include <direct.h>
#else unix
#include <unistd.h>
#include <dirent.h>
#endif

/**
 * List all files in the specific directory
 * @param dirName specific directory
 * @return all files in the directory
 */
static std::vector<std::string> listDir(const std::string &dirName)
{
    std::vector<std::string> files; // file names

#ifdef WIN32
    _finddata_t file;
    long long lf;
    if ((lf = _findfirst((dirName + "*").c_str(), &file)) == -1) // find fir
    {
        std::cerr << dirName << " not found!!!" << std::endl;
    }
    else
    {
        while (_findnext(lf, &file) == 0)
        {
            if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
                continue;
            files.emplace_back(file.name);
        }
    }
    _findclose(lf);
#elif unix
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir = opendir(dirName.c_str())) == nullptr)
    {
        perror("Open dir error...");
        exit(1);
    }
    while ((ptr = readdir(dir)) != nullptr)
    {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) // current dir OR parrent dir
            continue;
        else if (ptr->d_type == 8) // file
            files.push_back(ptr->d_name);
        else if (ptr->d_type == 10) // link file
            continue;
        else if (ptr->d_type == 4) // dir
            files.push_back(ptr->d_name);
    }
    closedir(dir);
#endif
    sort(files.begin(), files.end());
    return files;
}

/**
 * Indicate whether the given file exists
 * @param fname input file name
 * @return true if file exists, false otherwise.
 */
static inline bool isFile(const std::string &fname)
{
    std::fstream in(fname, std::ios::in);
    return in.is_open();
}

/**
 * Indicate whether the given directory exists
 * @param dir input directory name
 * @return true if directory exists, false otherwise.
 */
static inline bool isDir(const std::string &dir)
{
#ifdef WIN32
    return access(dir.c_str(), 0) == 0;
#elif unix
    return opendir(dir.c_str()) != nullptr;
#endif
}

/**
 * Create a new directory
 * @param dir new directory name
 * @return true if success, false if failure
 */
static inline bool createDir(const std::string &dir)
{
#ifdef WIN32
    return !isDir(dir.c_str()) && mkdir(dir.c_str()) == 0;
#elif unix
    return !isDir(dir.c_str()) && system(("mkdir " + dir).c_str()) == 0;
#endif
}

/**
 * Create a new file
 * @param fname new file name
 * @return true if success, false if failure
 */
static inline bool createFile(const std::string &fname)
{
    return std::fstream(fname, std::ios::out).is_open();
}

/**
 * Remove an existing directory
 * @param dir directory name
 * @return true if success, false if failure
 */
static inline bool removeDir(const std::string &dir)
{
    return rmdir(dir.c_str()) == 0;
}

/**
 * remove an existing file
 * @param fname file name
 * @return true if success, false if failure
 */
static inline bool removeFile(const std::string &fname)
{
    return remove(fname.c_str()) == 0;
}

/**
  * get current working directory
  * @return current working directory
  */
static inline std::string getCwd()
{
    char buf[256];
    return std::string(getcwd(buf, 256));
}

/**
 * change current working directory
 * @param dir destination directory
 * @return true if success, false if failure
 */
static inline bool changeCwd(const std::string &dir)
{
    return chdir(dir.c_str()) == 0;
}