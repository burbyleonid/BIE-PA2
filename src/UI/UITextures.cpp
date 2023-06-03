#include "UITextures.hpp"
#include <string>
#include <vector>
#include <SDL2/SDL_image.h>

void UITextures::load(SDL_Renderer *r) {
  vector<int> idx = { -3, -2, -1, 0, 1, 2, 3, 5 };
  
  for (auto key : idx) {
    string texDir = "./src/Textures";
    string fName = texDir + "/" + to_string(key) + ".png";
    
    SDL_Surface *surface = IMG_Load(fName.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);
    
    m_textures[key] = texture;
    m_surfaces[key] = surface;
  }
}

UITextures::~UITextures() {
  for (auto &p : m_surfaces) {
    if (p.second) {
      SDL_FreeSurface(p.second);
    }
  }
  for (auto &p : m_textures) {
    if (p.second) {
      SDL_DestroyTexture(p.second);
    }
  }
}
