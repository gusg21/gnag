//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_GNAGOSWRAPPER_H
#define GNAGTOOL_GNAGOSWRAPPER_H

#include <string>
#include <vector>
#include <thread>
#include <atomic>

struct FileEntry {
    bool IsDirectory;
    std::string FileName;
};

class GnagOSWrapper {
public:
    GnagOSWrapper() = delete;

    static std::string GetWorkingDirectory();
    static void GetFilesInDirectory(std::vector<FileEntry> &filesVec, const std::string &directory,
                                    bool *isDirectoryValid = nullptr);
    static bool GetGnagPath(std::string &gnagPath);
    static void ShowMessageBox(const std::string &message);
    static std::string GetFileExtension(const std::string &fileName);
    static std::string PathFix(const std::string &a, const std::string &b);
    static char GetPathSeparator();
    static char GetOtherPathSeparator();
    static std::string GetDirectoryFromPath(const std::string &path);
    static std::string CombinePaths(const std::string &base, const std::string &fileName);
    static std::string MakePathAbsolute(const std::string& maybeAbsolutePath);
    static std::string LocalizePath(const std::string& path);
    static bool DoesFileExist(const std::string& fileName);
    static void RunTheBuildinator();
    static void RunTheBuildinatorCleaner();
    static void RunTheAddressFinder();
    static bool IsTheBuildinatorBuildinating();

    static void CleanUpThreads();

private:
    static std::thread m_BuildinatorThread;
    static std::atomic_bool m_IsBuildinating;
};

#endif //GNAGTOOL_GNAGOSWRAPPER_H
