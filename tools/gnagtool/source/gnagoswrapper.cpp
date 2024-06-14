//
// Created by Angus Goucher on 5/25/2024.
//

#include "gnagoswrapper.h"

#include <Shlwapi.h>
#include <Windows.h>
#include <algorithm>

std::thread GnagOSWrapper::m_BuildinatorThread;
std::atomic_bool GnagOSWrapper::m_IsBuildinating;

std::string GnagOSWrapper::GetWorkingDirectory() {
#if WIN32
    TCHAR path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, path);
    std::string directory = std::string(path) + GnagOSWrapper::GetPathSeparator();
    return directory;
#else
    return {"/"};
#endif
}

void GnagOSWrapper::GetFilesInDirectory(std::vector<FileEntry>& filesVec, const std::string& directory,
                                        bool* isDirectoryValid) {
    filesVec.clear();

    WIN32_FIND_DATA findData;

    HANDLE handle = FindFirstFile((directory + GnagOSWrapper::GetPathSeparator() + "*").c_str(), &findData);
    if (handle == INVALID_HANDLE_VALUE) {
        *isDirectoryValid = false;
        return;
    }

    constexpr size_t bufferSize = MAX_PATH + 20;
    char buffer[bufferSize] = {0};
    do {
        FileEntry entry = { };
        entry.FileName = std::string {findData.cFileName};
        entry.IsDirectory = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        if (entry.IsDirectory)
            entry.FileName += GnagOSWrapper::GetPathSeparator();
        filesVec.emplace_back(entry);
    } while (FindNextFile(handle, &findData) != FALSE);

    DWORD error = GetLastError();
    if (error != ERROR_NO_MORE_FILES) {
        if (isDirectoryValid != nullptr) {
            *isDirectoryValid = false;
        }
    } else {
        if (isDirectoryValid != nullptr) {
            *isDirectoryValid = true;
        }
    }

    FindClose(handle);
}


bool GnagOSWrapper::GetGnagPath(std::string& gnagPath) {
    constexpr uint32_t envVarBufferSize = 1024;
    CHAR envVarBuffer[envVarBufferSize] = {0};
    uint32_t bytesWritten = GetEnvironmentVariable("GNAG_PATH", envVarBuffer, envVarBufferSize);
    bool exists = bytesWritten > 0;
    gnagPath.clear();
    if (exists) {
        gnagPath.append(envVarBuffer, bytesWritten);
        gnagPath.append({GnagOSWrapper::GetPathSeparator()});
    }
    return exists;
}

void GnagOSWrapper::ShowMessageBox(const std::string& message) {
    MessageBox(nullptr, message.c_str(), "Ancient Scroll of GNAG Wisdom", MB_OK);
}

std::string GnagOSWrapper::GetFileExtension(const std::string& fileName) {
    size_t extensionIndex = fileName.find_last_of('.');
    if (extensionIndex == std::string::npos)
        return "";
    return fileName.substr(extensionIndex);
}

std::string GnagOSWrapper::PathFix(const std::string& a, const std::string& b) {
    CHAR outputCstr[MAX_PATH] = {0};
    std::string prePath = a + b;
    PathCanonicalize(outputCstr, prePath.c_str());
    return std::string {outputCstr};
}

char GnagOSWrapper::GetPathSeparator() {
    return '\\';
}

char GnagOSWrapper::GetOtherPathSeparator() {
    return '/';
}

void GnagOSWrapper::RunTheBuildinator() {
    if (!m_IsBuildinating) {
        if (m_BuildinatorThread.joinable()) {
            m_BuildinatorThread.join();
        }
        m_BuildinatorThread = std::thread(
            []() -> void {
                m_IsBuildinating = true;
                system("\"%GNAG_PATH%\\buildinator.bat\" /nopause");
                m_IsBuildinating = false;
            }
            );
    }
}

std::string GnagOSWrapper::GetDirectoryFromPath(const std::string& path) {
    std::string directory;
    size_t last_slash_idx = path.rfind(GetPathSeparator());
    if (last_slash_idx != std::string::npos) {
        directory = path.substr(0, last_slash_idx);
    }
    return directory;
}

void GnagOSWrapper::RunTheAddressFinder() {
    system("start cmd /c \"%GNAG_PATH%\\addressfinder.bat\"");
}

void GnagOSWrapper::RunTheBuildinatorCleaner() {
    system("\"%GNAG_PATH%\\cleanbuildinator.bat\" /nopause");
}

void GnagOSWrapper::CleanUpThreads() {
    if (m_IsBuildinating) {
        m_BuildinatorThread.join();
    }
}

bool GnagOSWrapper::IsTheBuildinatorBuildinating() {
    return m_IsBuildinating;
}

std::string GnagOSWrapper::CombinePaths(const std::string& base, const std::string& fileName) {
    CHAR combined[MAX_PATH] = {0};
    PathCombine(combined, base.c_str(), fileName.c_str());
    return std::string {combined};
}

bool GnagOSWrapper::DoesFileExist(const std::string& fileName) {
    return PathFileExists(fileName.c_str());
}

std::string GnagOSWrapper::MakePathAbsolute(const std::string& maybeAbsolutePath) {
    if (PathIsRelative(maybeAbsolutePath.c_str())) {
        return CombinePaths(GetWorkingDirectory(), maybeAbsolutePath);
    }
    return maybeAbsolutePath;
}

std::string GnagOSWrapper::LocalizePath(const std::string& path) {
    std::string newPath {path};
    std::replace(newPath.begin(), newPath.end(), GetOtherPathSeparator(), GetPathSeparator());
    return newPath;
}

void GnagOSWrapper::DeletePath(const std::string& path) {
    DeleteFileA(path.c_str());
}
