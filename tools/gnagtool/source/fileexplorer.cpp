//
// Created by Angus Goucher on 5/24/2024.
//

#include "fileexplorer.h"
#include "gnagoswrapper.h"
#include "gnagtool.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "scenario.h"
#include "scenarioeditor.h"
#include "jsonhelper.h"
#include "uilayout.h"
#include "uilayouteditor.h"

#if WIN32

#include <Windows.h>

#endif

FileExplorer::FileExplorer(GnagTool* gnagTool, int guiID)
    : ToolGUI(gnagTool, guiID) {
    m_GnagTool = gnagTool;

    std::string gnagPath { };
    bool pathFound = GnagOSWrapper::GetGnagPath(gnagPath);
    if (pathFound) {
        m_CurrentPath = gnagPath;
    } else {
        m_CurrentPath = GnagOSWrapper::GetWorkingDirectory();
        GnagOSWrapper::ShowMessageBox(
            "GNAG_PATH not set: make sure to double-click & run the gnagenvset.bat file in the repository folder!");
    }
}

void FileExplorer::DoGUI() {
    // Update the listing
    GnagOSWrapper::GetFilesInDirectory(m_FilesVec, m_CurrentPath, &m_IsCurrentPathValid);
    if (m_SelectedFileIndex >= m_FilesVec.size()) {
        m_SelectedFileIndex = -1;
    }

    ImGui::SetNextWindowSize({500, 500}, ImGuiCond_FirstUseEver);
    ImGui::Begin("File Explorer", nullptr);
    {
        if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
            m_SelectedFileIndex = -1;
        }

        // Show the listing
        ImGui::Columns(2);
        ImGui::BeginGroup();
        {
            ImGui::InputText("File Explorer", &m_CurrentPath);
            ImVec2 availableSize = ImGui::GetContentRegionAvail();
            availableSize.y -= 30;
            ImGui::BeginChild("File Explorer File List", availableSize);
            if (m_FilesVec.empty()) {
                ImGui::Text("Folder empty");
            } else {
                for (uint32_t fileIndex = 0; fileIndex < m_FilesVec.size(); fileIndex++) {
                    bool selected = m_SelectedFileIndex == fileIndex;
                    FileEntry* entry = &m_FilesVec[fileIndex];
                    if (ImGui::Selectable(entry->FileName.c_str(), &selected,
                                          ImGuiSelectableFlags_AllowDoubleClick)) {
                        if (ImGui::IsMouseDoubleClicked(0)) {
                            if (entry->IsDirectory) {
                                m_CurrentPath = GnagOSWrapper::PathFix(m_CurrentPath, entry->FileName);
                            }
                        }
                    }
                    if (selected) {
                        m_SelectedFileIndex = (int32_t)fileIndex;
                    }
                }
            }
            ImGui::EndChild();
            if (ImGui::Button("New File...")) {
                ImGui::OpenPopup("FileExplorerNewFile");
            }
        }
        ImGui::EndGroup();
        ImGui::NextColumn();
        ImGui::BeginGroup();
        {
            // File options
            ImGui::SeparatorText("File Options");
            if (HasFileSelected()) {
                FileEntry* entry = &m_FilesVec[m_SelectedFileIndex];
                ImGui::Text("Selected File: %s %s", entry->FileName.c_str(), entry->IsDirectory ? "(directory)" : "");

                if (GetSelectedFileExtension() == ".json") {
                    if (ImGui::Button("Open as Scenario")) {
                        std::string absolutePath = m_CurrentPath + entry->FileName;
                        Scenario scenario;
                        scenario.LoadFromJSON(absolutePath);
                        m_GnagTool->AddGUI(
                            new ScenarioEditor(m_GnagTool, m_GnagTool->GetNextGUIID(), m_GnagTool->GetRenderer(),
                                               scenario, absolutePath));
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Open as UI Layout")) {
                        std::string absolutePath = m_CurrentPath + entry->FileName;
                        m_GnagTool->AddGUI(new UILayoutEditor(m_GnagTool, m_GnagTool->GetNextGUIID(),
                                                              m_GnagTool->GetRenderer(), absolutePath));
                    }
                }

                if (!entry->IsDirectory) {
                    if (ImGui::Button("Delete File")) {
                        GnagOSWrapper::DeletePath(m_CurrentPath + entry->FileName);
                    }
                }
            } else {
                ImGui::Text("No file selected");
            }
        }
        ImGui::EndGroup();

        if (ImGui::BeginPopup("FileExplorerNewFile")) {
            ImGui::SeparatorText("New File");

            ImGui::BeginChild("NewFileDialog", {ImGui::GetContentRegionAvail().x, 30.f});
            {
                ImGui::RadioButton("Scenario", &m_NewFileType, 0);
                ImGui::SameLine();
                ImGui::RadioButton("UI Layout", &m_NewFileType, 1);
            }
            ImGui::EndChild();

            ImGui::InputText("File Name", &m_NewFileEntryText);
            if (ImGui::Button("Create")) {
                std::string createdPath = m_CurrentPath + m_NewFileEntryText;

                switch (m_NewFileType) {
                    case 0: {
                        scenario_t new_scenario;
                        // No Init() function
                        Scenario_SaveToJSON(&new_scenario, createdPath.c_str());
                        break;
                    }
                    case 1: {
                        ui_layout_t new_layout;
                        UILayout_InitEmpty(&new_layout);
                        UILayout_SaveToFile(&new_layout, createdPath.c_str());
                        break;
                    }

                    default:
                        break;
                }

                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
    ImGui::End();

}

bool FileExplorer::HasFileSelected() const {
    return m_SelectedFileIndex != -1;
}

std::string FileExplorer::GetSelectedFileExtension() {
    if (!HasFileSelected())
        return "";
    return GnagOSWrapper::GetFileExtension(m_FilesVec[m_SelectedFileIndex].FileName);
}
