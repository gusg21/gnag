#include <cstdio>

#include "imgui.h"
#include "SDL.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "backends/imgui_impl_sdl2.h"
#include "gnagtool.h"

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("the glorious gnagtool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600,
                                          900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    auto *gnagTool = new GnagTool(renderer);

    SDL_Event event;
    bool running = true;
    uint64_t lastPCTicks = 0;
    float deltaTime = 0.f;
    while (running) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                default:
                    break;
            }
        }

        gnagTool->Update(deltaTime);

        // GUI
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        gnagTool->DoGUI();

        // RENDERING
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);

        deltaTime = (float)(SDL_GetPerformanceCounter() - lastPCTicks) / (float)SDL_GetPerformanceFrequency();
        lastPCTicks = SDL_GetPerformanceCounter();
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    SDL_Quit();
    return 0;
}