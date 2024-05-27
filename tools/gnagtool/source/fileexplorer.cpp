//
// Created by Angus Goucher on 5/24/2024.
//

#include "fileexplorer.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_internal.h"
#include "gnagoswrapper.h"
#include "scenario.h"
#include "scenarioeditor.h"
#include "gnagtool.h"

#if WIN32

#include <Windows.h>

#endif

FileExplorer::FileExplorer(GnagTool *gnagTool) : ToolGUI(gnagTool) {
    m_GnagTool = gnagTool;

    std::string gnagPath{};
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
                    FileEntry *entry = &m_FilesVec[fileIndex];
                    if (ImGui::Selectable(entry->FileName.c_str(), &selected,
                                          ImGuiSelectableFlags_AllowDoubleClick)) {
                        if (ImGui::IsMouseDoubleClicked(0)) {
                            if (entry->IsDirectory) {
                                m_CurrentPath = GnagOSWrapper::PathFix(m_CurrentPath, entry->FileName);
                            }
                        }
                    }
                    if (selected) {
                        m_SelectedFileIndex = (int32_t) fileIndex;
                    }
                }
            }
            ImGui::EndChild();
            ImGui::BeginDisabled(!m_IsCurrentPathValid);
            {
                ImGui::Button("New Scenario...");
                ImGui::SameLine();
                ImGui::BeginDisabled(!HasFileSelected());
                { ImGui::Button("Delete File..."); }
                ImGui::EndDisabled();
            }
            ImGui::EndDisabled();

        }
        ImGui::EndGroup();
        ImGui::NextColumn();
        ImGui::BeginGroup();
        {
            // File options
            ImGui::SeparatorText("File Options");
            if (HasFileSelected()) {
                FileEntry *entry = &m_FilesVec[m_SelectedFileIndex];
                ImGui::Text("Selected File: %s %s", entry->FileName.c_str(), entry->IsDirectory ? "(directory)" : "");

                if (GetSelectedFileExtension() == ".json") {
                    if (ImGui::Button("Open as Scenario")) {
                        std::string absolutePath = m_CurrentPath + entry->FileName;
                        Scenario scenario = Scenario::LoadScenarioFromJSON(absolutePath);
                        m_GnagTool->AddGUI(
                                new ScenarioEditor(m_GnagTool, m_GnagTool->GetRenderer(), scenario, absolutePath));
                    }
                }
            } else {
                ImGui::Text("No file selected");
            }
        }
        ImGui::EndGroup();
    }
    ImGui::End();
}

bool FileExplorer::HasFileSelected() const {
    return m_SelectedFileIndex != -1;
}

std::string FileExplorer::GetSelectedFileExtension() {
    if (!HasFileSelected()) return "";
    return GnagOSWrapper::GetFileExtension(m_FilesVec[m_SelectedFileIndex].FileName);
}



