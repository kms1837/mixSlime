#include "GameMain.h"

USING_NS_CC;

/*
 색상코드
 0 - 투명, 1 - 빨강, 2 - 파랑, 3 - 노랑, 4 - 보라, 5 - 초록, 6 - 주황, 7 - 검정
 */

Scene* GameMain::createScene()
{
    auto scene = Scene::create();
    auto layer = GameMain::create();
    scene->addChild(layer);
    
    return scene;
}

// on "init" you need to initialize your instance
bool GameMain::init()
{
    if ( !Layer::init() ) return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(GameMain::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameMain::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameMain::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    ////////////
    cocos2d::extension::ControlStepper* stepper = cocos2d::extension::ControlStepper::create(Sprite::create("stepper-minus.png"), Sprite::create("stepper-plus.png"));
    stepper->setPosition(Point(visibleSize.width/2, 200));
    stepper->addTargetWithActionForControlEvents(this, cccontrol_selector(GameMain::valueChanged), cocos2d::extension::Control::EventType::VALUE_CHANGED);
    addChild(stepper);
    
    cocos2d::extension::ControlStepper* stepper2 = cocos2d::extension::ControlStepper::create(Sprite::create("stepper-minus.png"), Sprite::create("stepper-plus.png"));
    stepper2->setPosition(Point(visibleSize.width/2, 150));
    stepper2->addTargetWithActionForControlEvents(this, cccontrol_selector(GameMain::valueChanged2), cocos2d::extension::Control::EventType::VALUE_CHANGED);
    addChild(stepper2);
    //////////////
    
    setColNumber = 6;
    setRowNumber = 6;
    gameSetting();
    
    return true;
}

//////////////////////테스트용/////////////////////////

void GameMain::valueChanged(Ref *sender, cocos2d::extension::Control::EventType controlEvent)
{
    cocos2d::extension::ControlStepper* pControl = (cocos2d::extension::ControlStepper*)sender;
    gameSetAllClear();
    
    setColNumber = 6 + (int)pControl->getValue();
    gameSetting();
    //colLabel->setString(String::createWithFormat("%d", (int)pControl->getValue())->getCString());
}

void GameMain::valueChanged2(Ref *sender, cocos2d::extension::Control::EventType controlEvent)
{
    cocos2d::extension::ControlStepper* pControl = (cocos2d::extension::ControlStepper*)sender;
    gameSetAllClear();
    
    setRowNumber = 6 + (int)pControl->getValue();
    gameSetting();
    //rowLabel->setString(String::createWithFormat("%d", (int)pControl->getValue())->getCString());
}
//////////////////////////////////////////////////////

void GameMain::gameSetting()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    gameSet = new SlimeBox*[setColNumber];
    for (int i=0; i<setColNumber; i++) gameSet[i] = new SlimeBox[setRowNumber];
    
    boxWeitht = visibleSize.width/setRowNumber; //가로사이즈
    boxHeight = boxWeitht; //세로사이즈
    
    for (int i=0; i<setColNumber; i++) {
        for (int j=0; j<setRowNumber; j++) {
            ///
            spriteSize = 180/boxWeitht;
            if(setRowNumber>6) gameSet[i][j].boxSprite->setScale(spriteSize);
            ///
            gameSet[i][j].boxSprite->setPosition(Point(boxWeitht*j+(boxWeitht/2), GAMESETHEIGHTSPACE-(boxWeitht*i+(boxWeitht/2))));
            gameSet[i][j].setColorNumber((rand()%3)+1);
            addChild(gameSet[i][j].boxSprite, 1);
            setSlimeColor(&gameSet[i][j]);
        }
    }
}
void GameMain::gameSetAllClear()
{
    for(int i=0; i<setColNumber; i++){
        for(int j=0; j<setRowNumber; j++){
            removeChild(gameSet[i][j].boxSprite);
            if(gameSet[i][j].slimeSprite != NULL) removeChild(gameSet[i][j].slimeSprite);
        }
    }
    for(int i=0; i<setColNumber; i++) delete[] gameSet[i];
    delete[] gameSet;
    gameSet = NULL;
}

void GameMain::setSlimeColor(SlimeBox* slimeNode)
{
    int slimeColor = slimeNode->getColorNumber();
    
    std::string slimeResourceName[] = {" ", "red.png", "blue.png", "yellow.png", "purple.png", "green.png", "orange.png"};
    Vec2 slimeBoxPosition = slimeNode->boxSprite->getPosition();
    
    if(slimeNode->slimeSprite == NULL){
        slimeNode->slimeSprite = Sprite::create(slimeResourceName[slimeColor]);
        
        ///
        slimeNode->slimeSprite->setScale(spriteSize-0.1);
        ///
        
        slimeNode->slimeSprite->setOpacity(0);
        slimeNode->slimeSprite->setPosition(Point(slimeBoxPosition.x, slimeBoxPosition.y));
        addChild(slimeNode->slimeSprite, 2);
        slimeNode->slimeSprite->runAction(FadeIn::create(0.3));
        
    }else{
        CallFuncN* removeFunc = CallFuncN::create(CC_CALLBACK_1(GameMain::autoRemoveSprite, this));
        if(slimeColor!=7){
            Texture2D* originalTexture = slimeNode->slimeSprite->getTexture();
            Sprite* tempSprite = Sprite::createWithTexture(originalTexture);
            tempSprite->setPosition(Point(slimeBoxPosition.x, slimeBoxPosition.y));
            
            ///
            tempSprite->setScale(spriteSize-0.1);
            ///
            
            addChild(tempSprite, 3);
        
            tempSprite->runAction(Sequence::create(FadeOut::create(0.3), removeFunc, NULL));
            slimeNode->slimeSprite->setTexture(slimeResourceName[slimeColor]);
            
        }else{
            slimeNode->slimeSprite->runAction(Sequence::create(FadeOut::create(0.3), removeFunc, NULL));
            slimeNode->slimeSprite = NULL;
        }
    }
}

void GameMain::autoRemoveSprite(Node* removeTarget)
{
    removeChild(removeTarget);
}

int GameMain::slimeColorMix(int moveColor, int targetColor)
{
    if((moveColor==1 && targetColor==2) || (moveColor==2 && targetColor==1)){
        return 4;
    }else if((moveColor==2 && targetColor==3) || (moveColor==3 && targetColor==2)){
        return 5;
    }else if((moveColor==1 && targetColor==3) || (moveColor==3 && targetColor==1)){
        return 6;
    }else if((moveColor==4 && targetColor==3) || (moveColor==5 && targetColor==1)|| (moveColor==6 && targetColor==2) ||
             (moveColor==3 && targetColor==4) || (moveColor==1 && targetColor==5)|| (moveColor==2 && targetColor==6)){
        return 7;
    }
    
    return 0;
}
/*
int slimeColorMix(int moveColor, int targetColor)
믹스 실패시 -1을 리턴 성공시는 해당 색상값을 넘겨준다.
 moveColor   - 이동하는 슬라임 색상값
 targetColor - 섞으려는 슬라임 색상값
*/

bool GameMain::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event)
{
    Vec2 touchLocation =  touch->getLocation();
    if(touchLocation.y < GAMESETHEIGHTSPACE && touchLocation.y > (GAMESETHEIGHTSPACE-boxWeitht*setColNumber)){
        targetingColNumber = (int)(GAMESETHEIGHTSPACE-touchLocation.y)/boxHeight;
        targetingRowNumber = (int)touchLocation.x/boxWeitht;
        CCLOG("클릭위치(%d, %d)", targetingColNumber, targetingRowNumber);
        return true;
    }
    
    return false;
}

void GameMain::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event  *event)
{
    Vec2 touchLocation =  touch->getLocation();
    if(touchLocation.y < GAMESETHEIGHTSPACE && touchLocation.y > (GAMESETHEIGHTSPACE-boxWeitht*setColNumber)){
        int moveColNumber = (int)(GAMESETHEIGHTSPACE-touchLocation.y)/boxHeight;
        int moveRowNumber = (int)touchLocation.x/boxWeitht;
        CCLOG("이동위치(%d, %d)", moveColNumber, moveRowNumber);
        if(moveColNumber!=targetingColNumber || moveRowNumber!=targetingRowNumber){
            int mixColor;
            MoveTo* slimeMoveAction = NULL;
            CCLOG("이동위치 같지않음!");
            if(moveColNumber==targetingColNumber){
                if(abs(moveRowNumber-targetingRowNumber)==1){
                    
                    mixColor = slimeColorMix(gameSet[targetingColNumber][targetingRowNumber].getColorNumber(),
                                             gameSet[moveColNumber][moveRowNumber].getColorNumber());
                    if(mixColor){
                        Vec2 targetSlimePosition = gameSet[moveColNumber][moveRowNumber].boxSprite->getPosition();
                        gameSet[moveColNumber][moveRowNumber].setColorNumber(mixColor);
                        slimeMoveAction = MoveTo::create(0.2, Point(targetSlimePosition.x, targetSlimePosition.y));
                    }
                }
            }else if(moveRowNumber==targetingRowNumber){
                if(abs(moveColNumber-targetingColNumber)==1){
                    mixColor = slimeColorMix(gameSet[targetingColNumber][targetingRowNumber].getColorNumber(),
                                             gameSet[moveColNumber][moveRowNumber].getColorNumber());
                    if(mixColor){
                        Vec2 targetSlimePosition = gameSet[moveColNumber][moveRowNumber].boxSprite->getPosition();
                        gameSet[moveColNumber][moveRowNumber].setColorNumber(mixColor);
                        slimeMoveAction = MoveTo::create(0.2, Point(targetSlimePosition.x, targetSlimePosition.y));
                    }
                }
            }
            
            if(slimeMoveAction!=NULL){
                Sprite* moveSlime = gameSet[targetingColNumber][targetingRowNumber].slimeSprite;
                CallFuncN* removeFunc = CallFuncN::create(CC_CALLBACK_1(GameMain::autoRemoveSprite, this));
                moveSlime->runAction(Sequence::create(slimeMoveAction, removeFunc, NULL));
                gameSet[targetingColNumber][targetingRowNumber].slimeSprite = NULL;
                CCLOG("바뀐색상 : %d", mixColor);
                gameSet[targetingColNumber][targetingRowNumber].setColorNumber(0);
                gameSet[moveColNumber][moveRowNumber].setColorNumber(mixColor);
                setSlimeColor(&gameSet[moveColNumber][moveRowNumber]);
            }
            
        }
    }
}

void GameMain::onToucheMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    /*
    Vec2 touchLocation =  touch->getLocation();
    if(touchLocation.y < GAMESETHEIGHTSPACE && touchLocation.y > (GAMESETHEIGHTSPACE-boxWeitht*setColNumber)){
        int moveColNumber = (int)(GAMESETHEIGHTSPACE-touchLocation.y)/boxHeight;
        int moveRowNumber = (int)touchLocation.x/boxWeitht;
    }
     */
}


