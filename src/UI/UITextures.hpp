#ifndef UITextures_hpp
#define UITextures_hpp

#include <stdio.h>
#include <map>
#include <SDL2/SDL.h>

using namespace std;

struct UITextures {
    UITextures() = default;
    ~UITextures();
    void load(SDL_Renderer *r);
    SDL_Texture *getTexture(int idx) { return m_textures[idx]; }
private:
    map<int, SDL_Texture*> m_textures;
    map<int, SDL_Surface*> m_surfaces;
};

#endif /* UITextures_hpp */
