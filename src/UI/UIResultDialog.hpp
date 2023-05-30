#ifndef UIResultDialog_hpp
#define UIResultDialog_hpp

#include <stdio.h>


#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include "UIBase.hpp"


using namespace std;

class UIResultDialog
{
public:
  UIResultDialog() = default;
  ~UIResultDialog() = default;
  
  static void run(int score, bool res);
private:
};

#endif /* UIResultDialog_hpp */
