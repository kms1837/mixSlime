#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__

#include "cocos2d.h"
#include "SlimeBox.h"
#include "extensions/cocos-ext.h"

#define GAMESETHEIGHTSPACE 1500

class GameMain : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void setSlimeSprite(SlimeBox*);
    CREATE_FUNC(GameMain);
    
private:
    bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);  //터치중
    void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);  //터치에서 손뗌
    void onToucheMoved(cocos2d::Touch*, cocos2d::Event*); //터치중 이동
    void autoRemoveSprite(cocos2d::Node*);
    void gameSetAllClear();
    void gameSetting();
    int slimeColorMix(int, int); //슬라임 색을 섞음
    void slimeMove(SlimeBox*, SlimeBox*); //슬라임 이동과 색을 섞음
    
    SlimeBox** gameSet;
    int boxWeitht; //박스 넓이
    int boxHeight; //박스 높이
    int setColNumber; //설정 행
    int setRowNumber; //설정 열
    int startColNumber; //타겟팅한 행
    int startRowNumber; //타겟팅한 열
    
    void valueChanged(Ref*, cocos2d::extension::Control::EventType);
    void valueChanged2(Ref*, cocos2d::extension::Control::EventType);
    
    void timeAttackTimer(float);
    int setEndTimeNumber; //종료 시간
    
    void gravity(int, int);
    
    ////
    cocos2d::Label* colLabel;
    cocos2d::Label* rowLabel;
    float spriteSize;
    ////
};

#endif // __HELLOWORLD_SCENE_H__
