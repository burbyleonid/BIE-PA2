#include "UIBase.hpp"



void UIBase::setTextureID(SDL_Texture *texture, const SDL_Rect &clipRect, const SDL_Rect &textureRect)
{
  m_texture = texture;
  m_directTexture = true;
  m_uiElementClipRect = clipRect;
  m_uiTextureRect = textureRect;
}



void UIBase::renderTexture() const
{
  if (m_texture)
  {
    if (m_directTexture) //if this texture was set directly then add the clipping rect
    {
      SDL_Rect newRect{m_uiElementRect.x + m_uiTextureRect.x, m_uiElementRect.y + m_uiTextureRect.y, m_uiTextureRect.w,
                       m_uiTextureRect.h};
      SDL_RenderCopy(m_renderer, m_texture, &m_uiElementClipRect, &newRect);
    }
    else //otherwise, leave it as null and it'll figure itself out
    {
      SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_uiElementRect);
    }
  }
}

bool UIBase::isMouseOver(int x, int y)
{
  return x > m_uiElementRect.x && x < m_uiElementRect.x + m_uiElementRect.w && y > m_uiElementRect.y &&
         y < m_uiElementRect.y + m_uiElementRect.h;
}

bool UIBase::isMouseOverHoverableArea(int x, int y)
{
  return x > m_uiElementRect.x && x < m_uiElementRect.x + m_uiElementRect.w && y > m_uiElementRect.y &&
         y < m_uiElementRect.y + m_uiElementRect.h;
}

SDL_Point UIBase::screenPointToLocalPointInRect(SDL_Point screenCoordinates)
{
  SDL_Point result;
  result.x = screenCoordinates.x - m_uiElementRect.x;
  result.y = screenCoordinates.y - m_uiElementRect.y;
  return result;
}



void UIBase::drawSolidRect(SDL_Rect rect, const SDL_Color &color) const
{
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(m_renderer, &rect);
}

void UIBase::drawLine(int x1, int y1, int x2, int y2, const SDL_Color &color) const
{
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}
