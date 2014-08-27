#ifndef __mixSlime__SlimeBox__
#define __mixSlime__SlimeBox__

#include "cocos2d.h"

class SlimeBox : public cocos2d::Node
{
private:
    int colorNumber;
public:
    SlimeBox();
    ~SlimeBox();
    cocos2d::Sprite* boxSprite;
    cocos2d::Sprite* slimeSprite;
    void setColorNumber(int);
    int getColorNumber();
};

#endif
