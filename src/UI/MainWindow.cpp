#include "MainWindow.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "Defines.hpp"
#include <string>
#include <vector>
#include <filesystem>
#include "UIText.hpp"
#include "UIButton.hpp"
#include "UISaveDialog.hpp"


// create a window using SDl lib
bool MainWindow::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Failed to Init SDL";
        std::cerr << "SDL Error: " << SDL_GetError();
        return false;
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to Init SDL_TTF";
        std::cerr << "SDL Error: " << TTF_GetError();
        return false;
    }

    m_window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, 0);

    if (!m_window)
        return false;

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    m_textures.load(m_renderer);

    if (!m_renderer)
        return false;

    return true;
}

MainWindow::~MainWindow() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
}

// creates a basic main menu
int MainWindow::menu() {
    SDL_Event event;

    bool mainMenuLoop = true;
    int menuResult = 0;

    // Creates and markup  buttons
    UIText versionText(m_window, m_renderer);
    // versionText.setText("VERSION_1");
    versionText.setPosition(SCREEN_WIDTH - versionText.getUiElementRect().w, SCREEN_HEIGHT - versionText.getUiElementRect().h);

    UIButton newGameButton({SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 200, 40}, m_window, m_renderer);
    newGameButton.setText("New Game");
    newGameButton.setUIElementID("newgame");
    newGameButton.registerCallbackFunction(
                                           [this, &menuResult]()
                                           {
                                               std::cout << "New Game" << endl;
                                               menuResult = 1;   // game starts flag
                                           });

    UIButton loadGameButton({SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20 + newGameButton.getUiElementRect().h * 2, 200, 40}, m_window, m_renderer);
    loadGameButton.setText("Load Game");
    loadGameButton.registerCallbackFunction(
                                            [this, &menuResult]()
                                            {
                                                std::cout << "Load Game" << endl;
                                                menuResult = 2; // load game flag
                                            });

    UIButton quitGameButton({SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20 + loadGameButton.getUiElementRect().h * 4, 200, 40}, m_window, m_renderer);
    quitGameButton.setText("Quit Game");
    quitGameButton.registerCallbackFunction(
                                            [this, &menuResult]()
                                            {
                                                std::cout << "Quit Game" << endl;
                                                menuResult = 0; // quit game flag
                                            });

    // store elements in vector
    std::vector<UIBase *> uiElements;
    uiElements.push_back(&newGameButton);
    uiElements.push_back(&loadGameButton);
    uiElements.push_back(&quitGameButton);
    uiElements.push_back(&versionText);

    UIBase *m_lastHoveredElement = nullptr;

    while (mainMenuLoop)
    {
        SDL_RenderClear(m_renderer);

        while (SDL_PollEvent(&event) != 0)
        {
            for (const auto &it : uiElements)
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        return 0;
                    case SDL_MOUSEBUTTONDOWN:
                        it->onMouseButtonDown(event);
                        break;
                    case SDL_MOUSEBUTTONUP:

                        if (it->onMouseButtonUp(event))
                        {
                            mainMenuLoop = false;
                        }
                        break;
                    case SDL_MOUSEMOTION:
                        it->onMouseMove(event);

                        // if the mouse cursor left an element, we're not hovering any more and we need to reset the pointer to the last hovered
                        if (m_lastHoveredElement && !m_lastHoveredElement->isMouseOverHoverableArea(event.button.x, event.button.y))
                        {
                            m_lastHoveredElement->onMouseLeave(event);
                            m_lastHoveredElement = nullptr;
                        }

                        // if the element we're hovering over is not the same as the stored "lastHoveredElement", update it
                        if (it->isMouseOverHoverableArea(event.button.x, event.button.y) && it != m_lastHoveredElement)
                        {
                            it->onMouseMove(event);

                            if (m_lastHoveredElement != nullptr)
                            {
                                m_lastHoveredElement->onMouseLeave(event);
                            }
                            m_lastHoveredElement = it;
                            it->onMouseEnter(event);
                        }
                        break;
                    default:;
                }
            }
        }

        // respresent all interface e;emtns on screen and render
        for (const auto &element : uiElements)
        {
            element->draw();
        }

        SDL_SetRenderDrawColor(m_renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
        SDL_RenderPresent(m_renderer);
    }

    return menuResult;
}

// button load game
std::string MainWindow::loadMenu() {
    namespace fs = std::filesystem;
    UIBase *m_lastHoveredElement = nullptr;


    fs::path dbDirPath = "./src/SavedGames";
    if (!fs::is_directory(dbDirPath)) {
        return "";
    }

    vector<string> fileNames;

    for (auto const& dir_entry : std::filesystem::directory_iterator{dbDirPath})
    {
        if (dir_entry.path().extension() == ".game") {
            auto tmp = dir_entry.path().filename().string();
            tmp.resize(tmp.size() - 5);
            fileNames.push_back(tmp);
        }
    }

    if (fileNames.size() == 0) {
        return "";
    }
    
    // markup exiting files
    int fileIdx = -1;
    int dh = SCREEN_HEIGHT / 11, dx = SCREEN_WIDTH / 10;
    int verShift = 0;

    std::vector<UIButton> filesUI;

    // if files < 10, print all of them, if > 10, pront only 10
    for (int i = 0; i < min(10lu, fileNames.size()); ++i) {
        filesUI.push_back(UIButton({ 0, i * dh, SCREEN_WIDTH, dh }, m_window, m_renderer, true));
        filesUI[i].setText(fileNames[i]);
    }

    for (int i = 0; i < filesUI.size(); ++i) {
        filesUI[i].registerCallbackFunction(
                                            [this, i, &verShift, &filesUI, &fileIdx]()
                                            {
                                                if (filesUI[i].checkState()) {
                                                    fileIdx = i + verShift;
                                                    for (int j = 0; j < filesUI.size(); ++j) {
                                                        if (i != j) {
                                                            filesUI[j].setCheckState(false);
                                                        }
                                                    }
                                                } else {
                                                    fileIdx = -1;
                                                }
                                            });
    }

    // button up for scrolling files
    UIButton up({dx * 8, 10 * dh, dx, dh}, m_window, m_renderer);
    up.setText("Up");
    up.registerCallbackFunction(
                                [this, &verShift, &filesUI, &fileIdx, &fileNames]()
                                {
                                    if (verShift + 10 < fileNames.size()) {
                                        verShift++;

                                        fileIdx = -1;

                                        for (int j = 0; j < filesUI.size(); ++j) {
                                            filesUI[j].setCheckState(false);
                                            filesUI[j].setText(fileNames[j + verShift]);
                                        }
                                    }
                                });

    // button down for scrolling files
    UIButton down({dx * 9, 10 * dh, dx, dh}, m_window, m_renderer);
    down.setText("Down");
    down.registerCallbackFunction(
                                  [this, &verShift, &filesUI, &fileIdx, &fileNames]()
                                  {
                                      if (verShift != 0) {
                                          verShift--;

                                          fileIdx = -1;

                                          for (int j = 0; j < filesUI.size(); ++j) {
                                              filesUI[j].setCheckState(false);
                                              filesUI[j].setText(fileNames[j + verShift]);
                                          }
                                      }
                                  });

    bool running = true;

    // button ok to select loading game
    UIButton ok({0, 10 * dh, dx, dh}, m_window, m_renderer);
    ok.setText("Ok");
    ok.registerCallbackFunction(
                                [this, &running]()
                                {
                                    running = false;
                                });

    std::vector<UIBase *> uiElements;

    uiElements.push_back(&ok);
    uiElements.push_back(&up);
    uiElements.push_back(&down);
    for (int i = 0; i < filesUI.size(); ++i) {
        uiElements.push_back(&filesUI[i]);
    }

    while(running){
        SDL_Event event;
        SDL_RenderClear(m_renderer);
        while(SDL_PollEvent(&event)) {
            for (const auto &it : uiElements)
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        //            quit();
                        return "";
                    case SDL_MOUSEBUTTONDOWN:
                        it->onMouseButtonDown(event);
                        break;
                    case SDL_MOUSEBUTTONUP:

                        if (it->onMouseButtonUp(event))
                        {
//                            running = false;
                        }
                        break;
                    case SDL_MOUSEMOTION:
                        it->onMouseMove(event);

                        // if the mouse cursor left an element, we're not hovering any more and we need to reset the pointer to the last hovered
                        if (m_lastHoveredElement && !m_lastHoveredElement->isMouseOverHoverableArea(event.button.x, event.button.y))
                        {
                            m_lastHoveredElement->onMouseLeave(event);
                            m_lastHoveredElement = nullptr;
                        }

                        // if the element we're hovering over is not the same as the stored "lastHoveredElement", update it
                        if (it->isMouseOverHoverableArea(event.button.x, event.button.y) && it != m_lastHoveredElement)
                        {
                            it->onMouseMove(event);

                            if (m_lastHoveredElement != nullptr)
                            {
                                m_lastHoveredElement->onMouseLeave(event);
                            }
                            m_lastHoveredElement = it;
                            it->onMouseEnter(event);
                        }
                        break;
                    default:;
                }
            }
        }
        for (const auto &element : uiElements)
        {
            element->draw();
        }

        SDL_SetRenderDrawColor(m_renderer, 34, 169, 199, SDL_ALPHA_OPAQUE);

        // render the frame
        SDL_RenderPresent(m_renderer);
    }


    if (fileIdx == -1) {
        return "";
    } else {
    	return fileNames[fileIdx];
    }
}

// starting saved game
bool MainWindow::run(const std::string &fileName) {
    bool running = true;

    std::vector<UIBase *> uiElements;
    UIBase *m_lastHoveredElement = nullptr;

    vector<vector<UIButton>> uiMap;

    Game game;
    if (fileName.size() != 0) {
        game.load("./src/SavedGames/" + fileName);
    }

    auto &map = game.getMap();

    std::vector<UIButton> buildingsUI;

    int controlHeight = 0.15 * SCREEN_HEIGHT;

    int buldingDx = SCREEN_WIDTH * 0.1;

    int buildingsIdx = -1;

    // 
    UIText daysScoreText(m_window, m_renderer);
    daysScoreText.setText("Days: " + std::to_string(game.getDaysCount()) + " Score: " + std::to_string(game.getScore()));
    daysScoreText.setPosition(SCREEN_WIDTH - daysScoreText.getUiElementRect().w, (controlHeight / 2 - daysScoreText.getUiElementRect().h) / 2);

    for (int i = 0; i < 4; ++i) {
        buildingsUI.push_back(UIButton({ int(i * buldingDx), 0, buldingDx, controlHeight }, m_window, m_renderer, true));
    }


    UIText resourcesText(m_window, m_renderer);
    resourcesText.setText(game.getResourcesText());
    resourcesText.setPosition(SCREEN_WIDTH - resourcesText.getUiElementRect().w, controlHeight / 2 + (controlHeight / 2 - resourcesText.getUiElementRect().h) / 2);


    UIButton next({ (SCREEN_WIDTH - buldingDx) / 2, 0, buldingDx, controlHeight }, m_window, m_renderer);
    next.setText("Next");

    size_t dx = SCREEN_WIDTH / game.getWidth(), dy = (SCREEN_HEIGHT - controlHeight) / game.getHeight();

    auto mB = game.getMainBuildCord();
    
    for (int i = 0; i < map.size(); ++i) {
        uiMap.push_back(std::vector<UIButton>());
        for (int j = 0; j < map[i].size(); ++j) {
            if (mB.first == i && mB.second == j) {
                uiMap[i].emplace_back(UIButton({ int(i * dx), int(controlHeight + j * dy), int(dx), int(dy) }, m_window, m_renderer, true));


                uiMap[i][j].registerCallbackFunction(
                                                     [this, i, j, &uiMap, &buildingsIdx, &buildingsUI, &resourcesText, controlHeight]()
                                                     {
                                                         if (uiMap[i][j].checkState()) {
                                                             buildingsIdx = (int)buildingsUI.size();
                                                             for (int j = 0; j < buildingsUI.size(); ++j) {
                                                                 buildingsUI[j].setCheckState(false);
                                                             }
                                                         } else {
                                                             buildingsIdx = -1;
                                                         }

                                                     });
            } else {

                uiMap[i].emplace_back(UIButton({ int(i * dx), int(controlHeight + j * dy), int(dx), int(dy) }, m_window, m_renderer));
                // uiMap[i][j].setText(game.getCellInfo(i, j));


                uiMap[i][j].registerCallbackFunction(
                                                     [this, &game, i, j, &uiMap, &buildingsIdx, &resourcesText, controlHeight, &daysScoreText]()
                                                     {
                                                         if (buildingsIdx == 4) {
                                                             auto mB = game.getMainBuildCord();
                                                            //  if (   game.getMap()[i][j]->m_type >= -1
                                                                //  && game.getMap()[i][j]->m_workerCount == 0
                                                                //  && game.getMap()[mB.first][mB.second]->m_workerCount > 0
                                                                // ) {

                                                                //  game.getMap()[i][j]->m_workerCount = 1;
                                                                //  game.getMap()[mB.first][mB.second]->m_workerCount--;

                                                                //  uiMap[i][j].setText(game.getCellInfo(i, j));
                                                                //  uiMap[mB.first][mB.second].setText(game.getCellInfo(mB.first, mB.second));
                                                            //  }
                                                         } if (buildingsIdx != -1 && game.getMap()[i][j]->m_type == -1 && game.checkCost(buildingsIdx)) {
                                                             game.addBuild(buildingsIdx, i, j);

                                                            //  uiMap[i][j].setText(game.getCellInfo(i, j));

                                                             uiMap[i][j].changeTexture(m_textures.getTexture(game.getCellType(i, j)));

                                                             resourcesText.setText(game.getResourcesText());
                                                             resourcesText.setPosition(SCREEN_WIDTH - resourcesText.getUiElementRect().w, controlHeight / 2 + (controlHeight / 2 - resourcesText.getUiElementRect().h) / 2);

                                                             daysScoreText.setText("Days: " + std::to_string(game.getDaysCount()) + " Score: " + std::to_string(game.getScore()));
                                                             daysScoreText.setPosition(SCREEN_WIDTH - daysScoreText.getUiElementRect().w, (controlHeight / 2 - daysScoreText.getUiElementRect().h) / 2);
                                                         }
                                                     });
            }
        }
    }

    for (int i = 0; i < 4; ++i) {
        buildingsUI[i].changeTexture(m_textures.getTexture(i));
        buildingsUI[i].registerCallbackFunction(
                                             [this, &game, i, &uiMap, &buildingsUI, &buildingsIdx, controlHeight]()
                                                {
                                                    if (buildingsUI[i].checkState()) {
                                                        buildingsIdx = i;
                                                        for (int j = 0; j < buildingsUI.size(); ++j) {
                                                            if (i != j) {
                                                                buildingsUI[j].setCheckState(false);
                                                            }
                                                        }
                                                        auto mB = game.getMainBuildCord();
                                                        uiMap[mB.first][mB.second].setCheckState(false);
                                                    } else {
                                                        buildingsIdx = -1;
                                                    }
                                                });
    }


    // UIButton buyWorker({ (SCREEN_WIDTH - buldingDx) / 2+buldingDx, 0, buldingDx, controlHeight }, m_window, m_renderer);
    // buyWorker.setText("Buy W");
    // buyWorker.registerCallbackFunction(
    //                               [this, &game, &uiMap, &resourcesText, controlHeight]()
    //                               {
    //                                   game.buyWorker();
    //                                   resourcesText.setText(game.getResourcesText());
    //                                   resourcesText.setPosition(SCREEN_WIDTH - resourcesText.getUiElementRect().w, controlHeight / 2 + (controlHeight / 2 - resourcesText.getUiElementRect().h) / 2);
    //                                   auto mB = game.getMainBuildCord();
    //                                   uiMap[mB.first][mB.second].setText(game.getCellInfo(mB.first, mB.second));
    //                               });

    next.registerCallbackFunction(
                                  [this, &game, &running, &resourcesText, &uiMap, controlHeight, &daysScoreText]()
                                  {
                                      auto dC = game.nextStep();

                                      resourcesText.setText(game.getResourcesText());
                                      resourcesText.setPosition(SCREEN_WIDTH - resourcesText.getUiElementRect().w, controlHeight / 2 + (controlHeight / 2 - resourcesText.getUiElementRect().h) / 2);

                                      if (dC.first != -1) {
                                        //   uiMap[dC.first][dC.second].setText(game.getCellInfo(dC.first, dC.second));
                                          uiMap[dC.first][dC.second].changeTexture(m_textures.getTexture(game.getCellType(dC.first, dC.second)));
                                      }
                                      daysScoreText.setText("Days: " + std::to_string(game.getDaysCount()) + " Score: " + std::to_string(game.getScore()));
                                      daysScoreText.setPosition(SCREEN_WIDTH - daysScoreText.getUiElementRect().w, (controlHeight / 2 - daysScoreText.getUiElementRect().h) / 2);

                                    //   running = game.isRunning();
                                  });

    UIButton save({ (SCREEN_WIDTH - buldingDx) / 2+buldingDx*2, 0, buldingDx, controlHeight / 2 }, m_window, m_renderer);
    save.setText("Save");
    save.registerCallbackFunction(
                                  [this, &game]()
                                  {
                                      auto fileName = UISaveDialog().getFileName();
                                      std::cout << fileName << std::endl;

                                      game.save("./src/SavedGames/" + fileName + ".game");
                                  });



    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            uiMap[i][j].changeTexture(m_textures.getTexture(game.getCellType(i, j)));
            uiElements.push_back(&uiMap[i][j]);
        }
    }

    for (size_t i = 0; i < buildingsUI.size(); ++i) {
        uiElements.push_back(&buildingsUI[i]);
    }

    uiElements.push_back(&daysScoreText);
    uiElements.push_back(&resourcesText);
    uiElements.push_back(&next);
    // uiElements.push_back(&buyWorker);
    uiElements.push_back(&save);

    while(running){
        SDL_Event event;
        SDL_RenderClear(m_renderer);
        while(SDL_PollEvent(&event)) {
            for (const auto &it : uiElements)
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        //            quit();
                        return true;
                    case SDL_MOUSEBUTTONDOWN:
                        it->onMouseButtonDown(event);
                        break;
                    case SDL_MOUSEBUTTONUP:

                        if (it->onMouseButtonUp(event))
                        {
//                            running = false;
                        }
                        break;
                    case SDL_MOUSEMOTION:
                        it->onMouseMove(event);

                        // if the mouse cursor left an element, we're not hovering any more and we need to reset the pointer to the last hovered
                        if (m_lastHoveredElement && !m_lastHoveredElement->isMouseOverHoverableArea(event.button.x, event.button.y))
                        {
                            m_lastHoveredElement->onMouseLeave(event);
                            m_lastHoveredElement = nullptr;
                        }

                        // if the element we're hovering over is not the same as the stored "lastHoveredElement", update it
                        if (it->isMouseOverHoverableArea(event.button.x, event.button.y) && it != m_lastHoveredElement)
                        {
                            it->onMouseMove(event);

                            if (m_lastHoveredElement != nullptr)
                            {
                                m_lastHoveredElement->onMouseLeave(event);
                            }
                            m_lastHoveredElement = it;
                            it->onMouseEnter(event);
                        }
                        break;
                    default:;
                }
            }
        }
        for (const auto &element : uiElements)
        {
            element->draw();
        }

        SDL_SetRenderDrawColor(m_renderer, 34, 169, 199, SDL_ALPHA_OPAQUE);

        // render the frame
        SDL_RenderPresent(m_renderer);
    }


    return true;
}
