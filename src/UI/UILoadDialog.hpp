#ifndef UILoadDialog_hpp
#define UILoadDialog_hpp

#include <stdio.h>


#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include "UIBase.hpp"


using namespace std;

class UILoadDialog
{
public:
  UILoadDialog() = default;
  ~UILoadDialog() = default;

  static std::string getFileName();
  static int getLvl();
private:
};

#endif /* UILoadDialog_hpp */
