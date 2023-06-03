#ifndef UISaveDialog_hpp
#define UISaveDialog_hpp

#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include "UIBase.hpp"


using namespace std;

class UISaveDialog
{
public:
    UISaveDialog() = default;
    ~UISaveDialog() = default;

    std::string getFileName();
private:
};

#endif /* UISaveDialog_hpp */
