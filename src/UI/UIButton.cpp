#include "UIButton.hpp"

UIButton::UIButton(const SDL_Rect &uiBaseRect, SDL_Window *w, SDL_Renderer *r, bool ToggleButton) : UIBase(uiBaseRect, w, r), m_frameRect(uiBaseRect), isToggleButton(ToggleButton)
{
    m_buttonLabel = std::make_shared<UIText>(w, r);
    centerTextLabel();
}

void UIButton::draw()
{
    // if the button is initialized with no dimension, don't draw a frame.
    if (m_frameRect.h != 0 && m_frameRect.w != 0)
    {
        drawButtonFrame(m_frameRect);
    }

    if (m_drawFrame)
    {
        drawButtonFrame(m_uiElementRect);
    }

    // render the buttons texture if available
    renderTexture();
    m_buttonLabel->draw();
}

void UIButton::drawButtonFrame(SDL_Rect rect, bool isHighlightable)
{
    Uint8 bgColor, bgColorFrame, bgColorFrameShade, bgColorBottomFrame, bgColorBottomFrameShade;

    if (getButtonState() == BUTTONSTATE_CLICKED && isHighlightable)
    {
        bgColor = 128;
        bgColorFrame = 106;
        bgColorFrameShade = 84;
        bgColorBottomFrame = 150;
        bgColorBottomFrameShade = 172;
    }
    else if ((getButtonState() == BUTTONSTATE_HOVERING) && isHighlightable)
    {
        bgColor = 228;
        bgColorFrame = 250;
        bgColorFrameShade = 255;
        bgColorBottomFrame = 206;
        bgColorBottomFrameShade = 184;
    }
    else
    {
        bgColor = 128;
        bgColorFrame = 150;
        bgColorFrameShade = 172;
        bgColorBottomFrame = 106;
        bgColorBottomFrameShade = 84;
    }

    drawSolidRect(rect, SDL_Color{bgColorFrameShade, bgColorFrameShade, bgColorFrameShade});
    drawSolidRect(SDL_Rect{rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4}, SDL_Color{bgColorFrame, bgColorFrame, bgColorFrame});
    // background
    drawSolidRect(SDL_Rect{rect.x + 4, rect.y + 4, rect.w - 8, rect.h - 8}, SDL_Color{bgColor, bgColor, bgColor});
    // bottom frame
    drawSolidRect(SDL_Rect{rect.x + 4, (rect.y + rect.h) - 4, rect.w - 4, 4},
                  SDL_Color{bgColorBottomFrame, bgColorBottomFrame, bgColorBottomFrame});
    drawSolidRect(SDL_Rect{(rect.x + rect.w) - 4, rect.y + 4, 4, rect.h - 4},
                  SDL_Color{bgColorBottomFrame, bgColorBottomFrame, bgColorBottomFrame});
    // bottom frame shade
    drawSolidRect(SDL_Rect{rect.x + 2, (rect.y + rect.h) - 2, rect.w - 2, 2},
                  SDL_Color{bgColorBottomFrameShade, bgColorBottomFrameShade, bgColorBottomFrameShade});
    drawSolidRect(SDL_Rect{(rect.x + rect.w) - 2, rect.y + 2, 2, rect.h - 2},
                  SDL_Color{bgColorBottomFrameShade, bgColorBottomFrameShade, bgColorBottomFrameShade});
}

void UIButton::setText(const std::string &text)
{
    m_buttonLabel->setText(text);
    elementData.text = text;
    centerTextLabel();
}

void UIButton::changeButtonState(int state)
{
    m_buttonState = state;
}

bool UIButton::onMouseButtonUp(const SDL_Event &event)
{
    if (isMouseOver(event.button.x, event.button.y))
    {
        if (!isToggleButton)
        {
            changeButtonState(BUTTONSTATE_DEFAULT);

            runCallback();
        }
        else
        {
            m_isMouseButtonDown = false;
            setCheckState(!m_checkState);

            runCallback();
        }
        return true;
    }
    return false;
}

void UIButton::runCallback() {
    for (auto &func : m_callbackF) {
        func();
    }
}

bool UIButton::onMouseButtonDown(const SDL_Event &event)
{
    if (isMouseOver(event.button.x, event.button.y))
    {
        if (!isToggleButton)
        {
            changeButtonState(BUTTONSTATE_CLICKED);
        }
        else
        {
            m_isMouseButtonDown = true;
        }
        return true;
    }
    return false;
}

void UIButton::onMouseEnter(const SDL_Event &event)
{
    if (event.button.button == SDL_BUTTON_LEFT && getButtonState() != BUTTONSTATE_CLICKED)
    {
        if (isToggleButton)
        {
            m_isMouseButtonDown = true;
        }
        changeButtonState(BUTTONSTATE_CLICKED);
    }
    else if (getButtonState() != BUTTONSTATE_HOVERING)
    {
        changeButtonState(BUTTONSTATE_HOVERING);
    }
}

void UIButton::onMouseLeave(const SDL_Event &)
{
    if (isToggleButton)
    {
        changeButtonState(m_checkState ? BUTTONSTATE_CLICKED : BUTTONSTATE_DEFAULT);
        m_isMouseButtonDown = false;
    }
    else if (getButtonState() != BUTTONSTATE_DEFAULT)
    {
        changeButtonState(BUTTONSTATE_DEFAULT);
    }
}

void UIButton::drawImageButtonFrame(bool drawFrame) { m_drawFrame = drawFrame; }

void UIButton::setCheckState(bool state)
{
    m_checkState = state;
    changeButtonState(m_checkState ? BUTTONSTATE_CLICKED : BUTTONSTATE_DEFAULT);
}

void UIButton::setPosition(int x, int y)
{
    m_uiElementRect.x = x;
    m_uiElementRect.y = y;
    m_frameRect.x = x;
    m_frameRect.y = y;
    centerTextLabel();
}

void UIButton::centerTextLabel() const
{
    int x = m_uiElementRect.x + m_uiElementRect.w / 2 - m_buttonLabel->getUiElementRect().w / 2;
    int y = m_uiElementRect.y + m_uiElementRect.h / 2 - m_buttonLabel->getUiElementRect().h / 2;

    m_buttonLabel->setPosition(x, y);
}

void UIButton::registerCallbackFunction(std::function<void()> const &cb) { m_callbackF.push_back(cb); }
