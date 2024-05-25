//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_GNAGOSWRAPPER_H
#define GNAGTOOL_GNAGOSWRAPPER_H

#include <string>
#include <vector>

struct FileEntry {
    bool IsDirectory;
    std::string FileName;
};

class GnagOSWrapper {
public:
    GnagOSWrapper() = delete;

    static std::string GetWorkingDirectory();
    static void GetFilesInDirectory(std::vector<FileEntry> &filesVec, const std::string &directory, bool* isDirectoryValid = nullptr);
    static bool GetGnagPath(std::string &gnagPath);
    static void ShowMessageBox(const std::string& message);
};

#endif //GNAGTOOL_GNAGOSWRAPPER_H
