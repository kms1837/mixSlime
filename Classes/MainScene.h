#ifndef __mixSlime__MainScene__
#define __mixSlime__MainScene__

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainScene);
private:
};

#endif
