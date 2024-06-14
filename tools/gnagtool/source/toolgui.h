//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_TOOLGUI_H
#define GNAGTOOL_TOOLGUI_H

class GnagTool;

class ToolGUI {
public:
    explicit ToolGUI(GnagTool *gnagTool, int guiID);
    ToolGUI(ToolGUI &) = delete;
    ToolGUI &operator=(ToolGUI &) = delete;

    virtual void Update(float deltaTime) { };
    virtual void DoGUI() = 0;

    int GetGUIID() { return m_GUIID; }

protected:
    GnagTool *m_GnagTool;

private:
    int m_GUIID;
};


#endif // GNAGTOOL_TOOLGUI_H
