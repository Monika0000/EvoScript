//
// Created by Nikita on 08.07.2021.
//

#ifndef EVOSCRIPT_FILESYSTEM_H
#define EVOSCRIPT_FILESYSTEM_H

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <EvoScript/Tools/StringUtils.h>

#include <sys/stat.h>
#include <fstream>

#include <vector>

#ifdef WIN32
    #include <Windows.h>
#endif

#include <direct.h>

namespace EvoScript::Tools {
    static bool RemoveFolder(const std::string& path) {
#ifdef WIN32
        return RemoveDirectoryA(path.c_str());
#else
        return false;
#endif
    }

    static std::vector<std::string> GetAllDirsInDir(const std::string& path) {
        auto dirs = std::vector<std::string>();

#ifdef WIN32
        std::string search_path = path + "/*.*";
        WIN32_FIND_DATA fd;
        HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
        if(hFind != INVALID_HANDLE_VALUE) {
            do {
                // read all (real) files in current folder
                // , delete '!' read other 2 default folder . and ..
                if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    const std::string name = std::string(fd.cFileName);

                    if (name.empty() || name == ".." || name == ".")
                        continue;

                    dirs.emplace_back(path + "/" + name);
                }
            }while(::FindNextFile(hFind, &fd));
            ::FindClose(hFind);
        }
#endif

        return dirs;
    }

    static bool IsDirectoryEmpty(const std::string& path) {
        return GetAllDirsInDir(path).empty();
    }

    static std::vector<std::string> GetAllFilesInDir(const std::string& path) {
        auto files = std::vector<std::string>();

#ifdef WIN32
        std::string search_path = path + "/*.*";
        WIN32_FIND_DATA fd;
        HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
        if(hFind != INVALID_HANDLE_VALUE) {
            do {
                // read all (real) files in current folder
                // , delete '!' read other 2 default folder . and ..
                if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                    files.emplace_back(path + "/" + std::string(fd.cFileName));
                }
            }while(::FindNextFile(hFind, &fd));
            ::FindClose(hFind);
        }
#endif

        return files;
    }

    static std::vector<std::string> GetAllFilesInDirWithExt(const std::string& path, const std::string& ext) {
        auto files = std::vector<std::string>();

        auto all = GetAllFilesInDir(path);

        for (const std::string& file : all)
            if (auto id = file.rfind('.'); id != std::string::npos)
                if (file.substr(id, file.size() - id) == ext)
                    files.emplace_back(file);

        return files;
    }

    static bool FileExists(const std::string& path) {
        struct stat buffer = {};
        return (stat(path.c_str(), &buffer) == 0);
    }

    static bool RemoveFile(const std::string& path) {
        return std::remove(path.c_str());
    }

    static bool CreateFolder(const std::string& directory) {
#ifdef __MINGW64__
        return mkdir(directory.c_str()) == 0;
#else
        return _mkdir(directory.c_str()) == 0;
#endif
    }

    static void WaitFile(const std::string& path) {
#ifdef WIN32
    repeat:
        HANDLE handle = CreateFile(path.c_str(), FILE_WRITE_DATA, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);

        if (handle == INVALID_HANDLE_VALUE) {
            Sleep(1);
            goto repeat;
        }

        CloseHandle(handle);
#endif
    }

    static bool Copy(const std::string& src, const std::string& dst) {
        if (!FileExists(src))
             return false;
        else {
            std::ifstream src_f(src, std::ios::binary);
            std::ofstream dst_f(dst, std::ios::binary);

            dst_f << src_f.rdbuf();

            src_f.close();
            dst_f.close();

            return true;
        }
    }

    //! path must be a fix
    static void CreatePath(std::string path, uint32_t offset = 0) {
        if (path.empty())
            return;

        if (path.back() != '/')
            path.append("/");

        auto pos = path.find('/', offset);
        if (pos != std::string::npos) {
            auto dir = Tools::Read(path, pos);
            Tools::CreateFolder(dir);
            CreatePath(path, pos + 1);
        }
    }
}

#endif //EVOSCRIPT_FILESYSTEM_H
