//
// Created by Angus Goucher on 5/24/2024.
//

#ifndef GNAGTOOL_FILEEXPLORER_H
#define GNAGTOOL_FILEEXPLORER_H

#include "gnagoswrapper.h"
#include "toolgui.h"
#include <string>
#include <vector>

class GnagTool;

class FileExplorer : public ToolGUI {
public:
    explicit FileExplorer(GnagTool *gnagTool);

    void DoGUI() override;
    bool HasFileSelected() const;

private:
    GnagTool *m_GnagTool;

    std::string m_CurrentPath { };
    std::vector<FileEntry> m_FilesVec { };
    int32_t m_SelectedFileIndex = 0;
    bool m_IsCurrentPathValid = false;

    std::string GetSelectedFileExtension();
};


#endif //GNAGTOOL_FILEEXPLORER_H
