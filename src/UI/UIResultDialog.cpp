#include "UIResultDialog.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "Defines.hpp"
#include "UIText.hpp"
#include "UIButton.hpp"


void UIResultDialog::run(int score, bool res) {
  namespace fs = std::filesystem;
  SDL_Window *m_window = nullptr;
  SDL_Renderer *m_renderer = nullptr;
  m_window = SDL_CreateWindow((res ? "Win" : "Lose"), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SAVE_DIALOG_SCREEN_WIDTH,
                              SAVE_DIALOG_SCREEN_HEIGHT, 0);

  if (!m_window)
    return ;

  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

  if (!m_renderer) {
    SDL_DestroyWindow(m_window);
    return ;
  }

  int elemHeight = (int)(0.20 * SAVE_DIALOG_SCREEN_HEIGHT);

  auto OkButton = UIButton({ (int)(SAVE_DIALOG_SCREEN_WIDTH * 0.45), (int)(0.7 * SAVE_DIALOG_SCREEN_HEIGHT),
                             (int)(SAVE_DIALOG_SCREEN_WIDTH * 0.1),  elemHeight},
                           m_window, m_renderer, true);

  OkButton.setText("OK");


  UIText resultText(m_window, m_renderer);
  resultText.setText(res ? "You win!" : "You lose");
  resultText.setPosition((int)(0.5 * (SAVE_DIALOG_SCREEN_WIDTH - resultText.getUiElementRect().w)), (int)(0.2 * SAVE_DIALOG_SCREEN_HEIGHT));

  UIText scoreText(m_window, m_renderer);
  scoreText.setText("Your score is " + to_string(score));
  scoreText.setPosition((int)(0.5 * (SAVE_DIALOG_SCREEN_WIDTH - scoreText.getUiElementRect().w)), (int)(0.5 * SAVE_DIALOG_SCREEN_HEIGHT));

  bool running = true;

  OkButton.registerCallbackFunction([&running] { running = false; });

  while ( running ) {
    SDL_Event event;
    SDL_RenderClear(m_renderer);
    while ( SDL_PollEvent( &event ) ) {
      switch (event.type)
      {
        case SDL_QUIT:
          return;
        case SDL_MOUSEBUTTONDOWN:
          OkButton.onMouseButtonDown(event);
          break;
        case SDL_MOUSEBUTTONUP:

          if (OkButton.onMouseButtonUp(event))
          {
          }
          break;
        case SDL_MOUSEMOTION:
          OkButton.onMouseMove(event);
        default:;
      }
    }

    OkButton.draw();
    resultText.draw();
    scoreText.draw();

    SDL_SetRenderDrawColor(m_renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
    SDL_RenderPresent(m_renderer);
  }

  if (m_renderer) {
    SDL_DestroyRenderer(m_renderer);
  }
  if (m_window) {
    SDL_DestroyWindow(m_window);
  }

  return;
}
