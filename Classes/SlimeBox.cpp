#include "SlimeBox.h"

SlimeBox::SlimeBox(){
    boxSprite   = cocos2d::Sprite::create();
    slimeSprite = NULL;
}

SlimeBox::~SlimeBox(){}

void SlimeBox::setSlimeColor(int inputNumber){colorNumber = inputNumber;}
int  SlimeBox::getSlimeColor(){return colorNumber;}
void SlimeBox::setArrayPosition(int inputColNumber, int inputRowNumber)
{
    colNumber = inputColNumber;
    rowNumber = inputRowNumber;
}

int SlimeBox::getColNumber(){return colNumber;}
int SlimeBox::getRowNumber(){return rowNumber;}
