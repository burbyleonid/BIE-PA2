#include <cstdio>
#include <iostream>
#include "MainWindow.hpp"

int main(int argc, char** argv){
  MainWindow mw;

  mw.initialize();

  int menuRes = mw.menu();

  while (menuRes) {
    switch (menuRes) {
      case 1: {
        int lvl = mw.lvlMenu();
        if (lvl > 0) {
          mw.run(lvl);
        }
        break;
      }
      case 2:{
        auto res = mw.loadMenu();
        if (res.size() != 0) {
          mw.run(0, res+".game");
        }
        break;
      }
      case 0:
        return 0;
      default:
        break;
    }
    menuRes = mw.menu();
  }

  return 0;
}
