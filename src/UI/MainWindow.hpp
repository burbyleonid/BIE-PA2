#ifndef MainWindow_hpp
#define MainWindow_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include "../Game/Game.hpp"
#include "UITextures.hpp"


using namespace std;


class MainWindow
{
public:
  MainWindow() = default;
  ~MainWindow() ;

  bool initialize();  // creates a window
  int menu();
  std::string loadMenu();
  int lvlMenu();
  bool run(int lvl, const std::string &fileName = "");
private:
  SDL_Window *m_window = nullptr;     // pointer to the SDL_Window
  SDL_Renderer *m_renderer = nullptr; // pointer to the SDL_Renderer
  UITextures m_textures;              // textures to draw
};

#endif /* MainWindow_hpp */
