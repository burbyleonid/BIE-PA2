#ifndef UIBase_hpp
#define UIBase_hpp

#include <stdio.h>
#include <string>
#include <functional>
#include <SDL2/SDL.h>


class UIBase
{
protected:
    struct ElementData
    {
        std::string elementID;       // id to reference to this element
        std::string text;            // text that is shown on the button
        std::string tooltipText;     // tooltip text that is shown when hovering over the button
        std::string actionID;        // id of the action this button invokes
        std::string actionParameter; // parameter for the function that's been called when the button is clicked.
        std::string textureID;       // id of the texture this item uses
        std::string buildMenuID;
        std::string layoutGroupName;
        UIBase *parent = nullptr;     // the object that toggles this items visibility
    };

    ElementData elementData;

public:
    //Initializes variables
//    UIBase() = delete;
    UIBase(SDL_Window *w, SDL_Renderer *r) : m_window(w), m_renderer(r) { };
    explicit UIBase(const SDL_Rect &uiElementRect, SDL_Window *w, SDL_Renderer *r) : m_uiElementRect(uiElementRect), m_window(w), m_renderer(r) {};
    virtual ~UIBase() = default;

    virtual void registerCallbackFunction(std::function<void()> const &){};
    virtual void registerCallbackFunction(std::function<void(UIBase *sender)> const &){};
    virtual void registerCallbackFunction(std::function<void(const std::string &, UIBase *sender)> const &){};

    // empty virtual function that can be overridden in the derived Ui Elements
    virtual bool onMouseButtonUp(const SDL_Event &) { return false; };
    virtual bool onMouseButtonDown(const SDL_Event &) { return false; };
    virtual void onMouseEnter(const SDL_Event &){};
    virtual void onMouseLeave(const SDL_Event &){};
    virtual void onMouseMove(const SDL_Event &){};
    virtual bool onKeyDown(const SDL_Event &) { return false; };

    // draw the UI Element and/or render it's textures to the screen
    // renders the texture of the Ui element. 
    virtual void draw(){};

    // sets the x,y position of this ui element
    virtual void setPosition(int x, int y)
    {
        m_uiElementRect.x = x;
        m_uiElementRect.y = y;
    };

    // set the position and the size of this ui element. Returns position and size as SDL_Rect
     const SDL_Rect &getUiElementRect() const { return m_uiElementRect; };

    // checks if the mouse cursor is over the current UI Element (if the coordinates match the ones stored in m_uiElementRect)
    virtual bool isMouseOver(int x, int y);

    SDL_Renderer *getRenderer() { return m_renderer; }

    
    // check if the mouse cursor is hovering over the element. Virtual function to provide custom handling if necessary
    virtual bool isMouseOverHoverableArea(int x, int y);

    // check the UI Element's visibility.
    bool isVisible() { return m_visible; };

    // sets the UI Element's visibility.
    void setVisibility(bool visibility) { m_visible = visibility; };

    // sets the ID of the action the UI Element should execute when it's clicked.
    void setActionID(const std::string &actionID) { elementData.actionID = actionID; };

    // sets the Parameter of the action the UI Element should execute when it's clicked.
    void setActionParameter(const std::string &actionParameter) { elementData.actionParameter = actionParameter; };

    // set the tooltip text for this ui element. Tooltip is shown when hovering over a ui Element.
    void setTooltipText(const std::string &text) { elementData.tooltipText = text; };

    // get the uiElements tooltip text
    void setUIElementID(const std::string &elementID) { elementData.elementID = elementID; };

    // set the uiElements texture ID
    void setTextureID(const std::string &textureID);

    // just incase you want to set the texture directly
    void setTextureID(SDL_Texture *texture, const SDL_Rect &clipRect, const SDL_Rect &textureRect);

    // sets the element's parent element
    void setParent(UIBase *parent) { elementData.parent = parent; };

    void setMenuGroupID(const std::string &buildMenuID) { elementData.buildMenuID = buildMenuID; };
    void setLayoutGroupName(const std::string &layoutGroupName) { elementData.layoutGroupName = layoutGroupName; };

    const ElementData &getUiElementData() const { return elementData; };


    virtual SDL_Point screenPointToLocalPointInRect(SDL_Point screenCoordinates);

    // Change the Texture of the button
    void changeTexture(SDL_Texture *texture) { m_texture = texture; };

private:
    SDL_Renderer *m_renderer = nullptr;
    SDL_Window *m_window = nullptr;


    bool m_visible = true;

    bool m_directTexture = false; //was this texture set directly

protected:
    SDL_Texture *m_texture = nullptr; // a pointer to the element's texture
    SDL_Rect m_uiElementRect{0, 0, 0, 0};
    SDL_Rect m_uiElementClipRect{0, 0, 0, 0};
    SDL_Rect m_uiTextureRect{0, 0, 0, 0};

    void renderTexture() const;

    // draws a filled rectangle to the screen.
    void drawSolidRect(SDL_Rect rect, const SDL_Color &color) const;

    // draws a line to the screen.
    void drawLine(int x1, int y1, int x2, int y2, const SDL_Color &color) const;
};




#endif /* UIBase_hpp */
