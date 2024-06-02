//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_TOOLGUI_H
#define GNAGTOOL_TOOLGUI_H

class GnagTool;

class ToolGUI {
public:
    explicit ToolGUI(GnagTool *gnagTool);
    ToolGUI(ToolGUI &) = delete;
    ToolGUI &operator=(ToolGUI &) = delete;

    virtual void Update(float deltaTime) { };
    virtual void DoGUI() = 0;

protected:
    GnagTool *m_GnagTool;
};


#endif // GNAGTOOL_TOOLGUI_H
