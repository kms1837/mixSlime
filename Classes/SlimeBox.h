#ifndef __mixSlime__SlimeBox__
#define __mixSlime__SlimeBox__

#include "cocos2d.h"

class SlimeBox
{
private:
    int colorNumber;
    int colNumber;
    int rowNumber;
public:
    SlimeBox();
    ~SlimeBox();
    cocos2d::Sprite* boxSprite;
    cocos2d::Sprite* slimeSprite;
    void setSlimeColor(int);
    int getSlimeColor();
    void setArrayPosition(int, int);
    int getColNumber();
    int getRowNumber();
};

#endif
