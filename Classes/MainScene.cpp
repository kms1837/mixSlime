#include "MainScene.h"

using namespace cocos2d;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool MainScene::init()
{
    Sprite* test = Sprite::create("slime/red.png");
    addChild(test);
    return true;
}