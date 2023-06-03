#ifndef UIText_hpp
#define UIText_hpp

#include <stdio.h>
#include "UIBase.hpp"

class UIText : public UIBase
{
public:
  UIText(SDL_Window *w, SDL_Renderer *r) : UIBase({0, 0, 0, 0}, w, r){};

  ~UIText() override;
  
  void draw() override;

  void setText(const std::string &text);

  void setFontSize(int fontSize);

  int getFontSize() const { return m_fontSize; }

private:
  // Draws a text string to the screen. This could either be a standalone text, a text on another texture or a text on a GUI Element drawn with shape functions.
  void createTextTexture(const std::string &text, const SDL_Color &textColor);

  /// the font size of the text
  int m_fontSize = 20;
};


#endif /* UIText_hpp */
