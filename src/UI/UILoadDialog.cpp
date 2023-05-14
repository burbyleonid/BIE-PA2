#include "UILoadDialog.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "Defines.hpp"
#include "UIText.hpp"
#include "UIButton.hpp"


std::string UILoadDialog::getFileName() {
    namespace fs = std::filesystem;
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    m_window = SDL_CreateWindow("Load", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SAVE_DIALOG_SCREEN_WIDTH,
                                SAVE_DIALOG_SCREEN_HEIGHT, 0);

    if (!m_window)
        return "";

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    if (!m_renderer) {
        SDL_DestroyWindow(m_window);
        return "";
    }

    int fileIdx = -1;

    fs::path dbDirPath = "./src/SavedGames";
    if (!fs::is_directory(dbDirPath)) {
        std::cout << "throw error" << std::endl;
        return "";
    }

    std::vector<UIButton> filesUI;


    int  controlHeight = 0.15 * SCREEN_HEIGHT;

    int buldingDx = SCREEN_WIDTH * 0.1;

    for (int i = 0; i < 4; ++i) {
        filesUI.push_back(UIButton({ 0,  buldingDx, controlHeight }, m_window, m_renderer, true));
    }

    UIText text(m_window, m_renderer);
    text.setText("File Name: ");
    text.setPosition(0, (SAVE_DIALOG_SCREEN_HEIGHT - text.getUiElementRect().h) / 2);


    UIText fileName(m_window, m_renderer);
    fileName.setText("");
    fileName.setPosition(text.getUiElementRect().w, (SAVE_DIALOG_SCREEN_HEIGHT - text.getUiElementRect().h) / 2);

    SDL_StartTextInput();
    std::string in;
    bool running = true;

    while ( running ) {
        SDL_Event ev;
        SDL_RenderClear(m_renderer);
        while ( SDL_PollEvent( &ev ) ) {
            if ( ev.type == SDL_TEXTINPUT ) {
                in += ev.text.text;
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && in.size()) {
                in.pop_back();
            } else if ( ev.type == SDL_QUIT || ev.key.keysym.sym == SDLK_RETURN) {
                running = false;
            }
        }

        fileName.setText(in);
        fileName.setPosition(text.getUiElementRect().w, (SAVE_DIALOG_SCREEN_HEIGHT - text.getUiElementRect().h) / 2);

        text.draw();
        fileName.draw();

        SDL_SetRenderDrawColor(m_renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
        SDL_RenderPresent(m_renderer);
    }

    SDL_StopTextInput();

    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }

    return in;
}
