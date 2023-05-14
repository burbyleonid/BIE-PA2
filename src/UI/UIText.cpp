#include "UIText.hpp"
#include <SDL2/SDL_ttf.h>


void UIText::draw()
{
  if (m_texture && isVisible())
  {
    renderTexture();
  }
}

UIText::~UIText() {
  if (m_texture)
    SDL_DestroyTexture(m_texture);
}

void UIText::setText(const std::string &text)
{
  elementData.text = text;

  createTextTexture(elementData.text, SDL_Color{255, 255, 255});
}

void UIText::setFontSize(int fontSize)
{
  m_fontSize = fontSize;
  // call the setText function again with the current text
  setText(elementData.text);
}

void UIText::createTextTexture(const std::string &text, const SDL_Color &textColor)
{
  std::string fontFName = "Coffee_Extra.ttf";
  std::string fontPath = "./src/" + fontFName;



  TTF_Font *font = TTF_OpenFont( fontPath.c_str(), m_fontSize);

  if (!font)
      printf("TTF_OpenFont: %s\n", TTF_GetError());

  // destroy texture first to prevent memleaks
  if (m_texture)
    SDL_DestroyTexture(m_texture);

  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
  TTF_CloseFont(font);

  SDL_Rect _textRect{0, 0, 0, 0};

  m_texture = SDL_CreateTextureFromSurface(getRenderer(), textSurface);

  // no surface exists but some shape has been drawn for that ui element
  SDL_QueryTexture(m_texture, nullptr, nullptr, &_textRect.w, &_textRect.h);

  _textRect.x = m_uiElementRect.x + (m_uiElementRect.w / 2) - (_textRect.w / 2);
  _textRect.y = m_uiElementRect.y + (m_uiElementRect.h / 2) - (_textRect.h / 2);
  m_uiElementRect = _textRect;

  /* Delete no longer needed surface */
  SDL_FreeSurface(textSurface);

}
