#ifndef UIButton_hpp
#define UIButton_hpp

#include <stdio.h>
#include "UIText.hpp"
#include <vector>
#include <memory>


enum class TextLayout
{
    CENTERED,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};


enum ButtonState
{
  BUTTONSTATE_DEFAULT,
  BUTTONSTATE_HOVERING,
  BUTTONSTATE_CLICKED,
  BUTTONSTATE_DISABLED
};

class UIButton : public UIBase
{
public:
    UIButton(const SDL_Rect &uiBaseRect, SDL_Window *w, SDL_Renderer *r, bool ToggleButton = false);
    ~UIButton() override = default;

    void draw() override;

    // sets the button's text
    void setText(const std::string &text);

    bool onMouseButtonUp(const SDL_Event &event) override;
    bool onMouseButtonDown(const SDL_Event &event) override;
    void onMouseEnter(const SDL_Event &event) override;
    void onMouseLeave(const SDL_Event &event) override;

    void drawImageButtonFrame(bool drawFrame);
    void drawButtonFrame(SDL_Rect rect, bool isHighlightable = true);

    int getButtonState() { return m_buttonState; };
    void changeButtonState(int state);

    void registerCallbackFunction(std::function<void()> const &cb) override;


    void setPosition(int x, int y) override;
    void centerTextLabel() const;

    bool checkState() const { return m_checkState; };
    void setCheckState(bool state);

    // specifies if this is a toggle button
    bool isToggleButton = false; 

private:
    void runCallback();

private:
    SDL_Rect m_frameRect;

    /// a pointer to the button's text
    std::shared_ptr<UIText> m_buttonLabel = nullptr;

    bool m_isMouseButtonDown = false;

    bool m_drawFrame = false;
    bool m_checkState = false;

    int m_buttonState = BUTTONSTATE_DEFAULT;

    std::vector<std::function<void()>> m_callbackF;

};

#endif /* UIButton_hpp */
