#include "SlimeBox.h"

SlimeBox::SlimeBox(){
    boxSprite   = cocos2d::Sprite::create("box.png");
    slimeSprite = NULL;
}

SlimeBox::~SlimeBox(){}

void SlimeBox::setColorNumber(int inputNumber){colorNumber = inputNumber;}
int  SlimeBox::getColorNumber(){return colorNumber;}
