#include "UISaveDialog.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include "Defines.hpp"
#include "UIText.hpp"


std::string UISaveDialog::getFileName() {
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    m_window = SDL_CreateWindow("Save", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SAVE_DIALOG_SCREEN_WIDTH,
                                SAVE_DIALOG_SCREEN_HEIGHT, 0);

    if (!m_window)
        return "";

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    if (!m_renderer) {
        SDL_DestroyWindow(m_window);
        return "";
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
