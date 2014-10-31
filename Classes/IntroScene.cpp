#include "IntroScene.h"
#include "MainScene.h"

using namespace cocos2d;

Scene* IntroScene::createScene()
{
    auto scene = Scene::create();
    auto layer = IntroScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool IntroScene::init()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* teamLogo = Sprite::create("slime/green.png");
    teamLogo->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    teamLogo->setOpacity(0);
    addChild(teamLogo);
    
    CallFunc* moveScene = CallFunc::create([&] (){
        Director* director = Director::getInstance();
        Scene* moveScene = MainScene::createScene();
        director->replaceScene(moveScene);
    });
    
    Sequence* logFade = Sequence::create(FadeIn::create(1.0f),
                                         FadeOut::create(1.0f),
                                         moveScene,
                                         NULL);
    teamLogo->runAction(logFade);
    
    return true;
}