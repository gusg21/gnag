//
// Created by Angus Goucher on 5/25/2024.
//

#include "gnagoswrapper.h"

#include <Windows.h>

std::string GnagOSWrapper::GetWorkingDirectory() {
#if WIN32
    TCHAR path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, path);
    std::string directory = std::string(path) + "\\";
    return directory;
#else
    return {"/"};
#endif
}

void GnagOSWrapper::GetFilesInDirectory(std::vector<FileEntry> &filesVec, const std::string &directory,
                                        bool *isDirectoryValid) {
    filesVec.clear();

    WIN32_FIND_DATA findData;

    HANDLE handle = FindFirstFile((directory + "\\*").c_str(), &findData);
    if (handle == INVALID_HANDLE_VALUE) {
        *isDirectoryValid = false;
        return;
    }

    constexpr size_t bufferSize = MAX_PATH + 20;
    char buffer[bufferSize] = {0};
    do {
        FileEntry entry = {};
        entry.FileName = std::string {findData.cFileName};
        entry.IsDirectory = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        if (entry.IsDirectory) entry.FileName += "/";
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


bool GnagOSWrapper::GetGnagPath(std::string &gnagPath) {
    constexpr uint32_t envVarBufferSize = 1024;
    CHAR envVarBuffer[envVarBufferSize] = {0};
    uint32_t bytesWritten = GetEnvironmentVariable("GNAG_PATH", envVarBuffer, envVarBufferSize);
    gnagPath.clear();
    gnagPath.append(envVarBuffer, bytesWritten);
    return bytesWritten > 0;
}

void GnagOSWrapper::ShowMessageBox(const std::string &message) {
    MessageBox(nullptr, message.c_str(), "Ancient Scroll of GNAG Wisdom", MB_OK);
}
