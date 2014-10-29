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
    
    //////////// 판 늘리기 패널 ////////////
    cocos2d::extension::ControlStepper* stepper = cocos2d::extension::ControlStepper::create(Sprite::create("stepper-minus.png"), Sprite::create("stepper-plus.png"));
    stepper->setPosition(Point(visibleSize.width/2, 200));
    stepper->addTargetWithActionForControlEvents(this, cccontrol_selector(GameMain::valueChanged), cocos2d::extension::Control::EventType::VALUE_CHANGED);
    addChild(stepper);
    
    cocos2d::extension::ControlStepper* stepper2 = cocos2d::extension::ControlStepper::create(Sprite::create("stepper-minus.png"), Sprite::create("stepper-plus.png"));
    stepper2->setPosition(Point(visibleSize.width/2, 150));
    stepper2->addTargetWithActionForControlEvents(this, cccontrol_selector(GameMain::valueChanged2), cocos2d::extension::Control::EventType::VALUE_CHANGED);
    addChild(stepper2);
    //////////////////////////////////////
    
    
    //////////// 타이머 & 게이지 ////////////
    schedule(schedule_selector(GameMain::timeAttackTimer), 1);
    setEndTimeNumber = 10;
    
    auto left = ProgressTimer::create(Sprite::create("test_gaugebar.png"));
    left->setType(ProgressTimer::Type::BAR);
    // Setup for a bar starting from the left since the midpoint is 0 for the x
    left->setMidpoint(Vec2(0, 0));
    // Setup for a horizontal bar since the bar change rate is 0 for y meaning no vertical change
    left->setBarChangeRate(Vec2(1, 0));
    addChild(left);
    
    left->setPosition(Vec2(visibleSize.width/2, 260));
    
    left->runAction(ProgressFromTo::create(setEndTimeNumber, 100, 0)); //setEndTimeNumber
    
    //////////////////////////////////////
    
    gameScore = 0;
    
    setColNumber = 6;
    setRowNumber = 6;
    
    gameSetting();
    
    return true;
}

void GameMain::timeAttackTimer(float f)
{
    count++;
    CCLOG("타이머 : %d", count);
    if(setEndTimeNumber <= count){
        CCLOG("끝!");
        unschedule(schedule_selector(GameMain::timeAttackTimer));
    }
    
    //CCLOG("시간 : %d", count++);
}

void GameMain::getTheScore()
{
    gameScore = gameScore + 1;
    CCLOG("현재스코어 %l", gameScore);
}//게임스코어에 대한 로직

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
            
            int setColorNumber = 0;
            while(setColorNumber==3 || setColorNumber==0) setColorNumber = (rand()%4)+1; //3이 혼합색인 보라컬러이기때문에 3을 제외한 숫자가 나올때까지 돌린다.
            
            gameSet[i][j].setSlimeColor(setColorNumber);
            gameSet[i][j].setArrayPosition(i, j);
            addChild(gameSet[i][j].boxSprite, 1);
            setSlimeSprite(&gameSet[i][j]);
        }
    }
}//function gameSetting - 게임을 셋팅해줌(빈공간에 슬라임을 채우는 등)

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

void GameMain::setSlimeSprite(SlimeBox* slimeNode)
{
    int slimeColor = slimeNode->getSlimeColor();
    
    std::string slimeResourceName[] = {" ", "red.png", "blue.png", "purple.png", "yellow.png", "orange.png", "green.png"};
    Vec2 slimeBoxPosition = slimeNode->boxSprite->getPosition();
    CallFuncN* removeFunc = CallFuncN::create(CC_CALLBACK_1(GameMain::autoRemoveSprite, this));
    
    if(slimeColor < 7 && slimeColor != 0){
        if(slimeNode->slimeSprite == NULL){
            slimeNode->slimeSprite = Sprite::create(slimeResourceName[slimeColor]);
            slimeNode->slimeSprite->setOpacity(0);
            slimeNode->slimeSprite->setPosition(slimeBoxPosition);
            addChild(slimeNode->slimeSprite, 2);
            slimeNode->slimeSprite->runAction(FadeIn::create(0.3));
        
            //
            slimeNode->slimeSprite->setScale(spriteSize-0.1);
            //
        
        }else{
            Texture2D* originalTexture = slimeNode->slimeSprite->getTexture();
            Sprite* tempSprite = Sprite::createWithTexture(originalTexture);
            tempSprite->setPosition(slimeBoxPosition);
            addChild(tempSprite, 3);
            
            ///
            tempSprite->setScale(spriteSize-0.1);
            ///
            
            tempSprite->runAction(Sequence::create(FadeOut::create(0.3), removeFunc, NULL));
            slimeNode->slimeSprite->setTexture(slimeResourceName[slimeColor]);
        }
    }else{
        if(slimeNode->slimeSprite != NULL){
            slimeNode->slimeSprite->runAction(Sequence::create(FadeOut::create(0.3), removeFunc, NULL));
            slimeNode->slimeSprite = NULL;
            gravity(slimeNode->getColNumber(), slimeNode->getRowNumber());
        }//슬라임 제거
    }
}

void GameMain::autoRemoveSprite(Node* removeTarget)
{
    removeChild(removeTarget);
}

bool GameMain::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event)
{
    Vec2 touchLocation =  touch->getLocation();
    if(touchLocation.y < GAMESETHEIGHTSPACE && touchLocation.y > (GAMESETHEIGHTSPACE-boxWeitht*setColNumber)){
        startColNumber = (int)(GAMESETHEIGHTSPACE-touchLocation.y)/boxHeight;
        startRowNumber = (int)touchLocation.x/boxWeitht;
        CCLOG("클릭위치(%d, %d)", startColNumber, startRowNumber);
        return true;
    }
    
    return false;
}
/*
 onTouchBegan - 터치시 콜백 함수
 현재위치를 알려준다.
 */

void GameMain::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event  *event)
{
    Vec2 touchLocation =  touch->getLocation();
    if(touchLocation.y < GAMESETHEIGHTSPACE && touchLocation.y > (GAMESETHEIGHTSPACE-boxWeitht*setColNumber)){
        int endedColNumber = (int)(GAMESETHEIGHTSPACE-touchLocation.y)/boxHeight;
        int endedRowNumber = (int)touchLocation.x/boxWeitht;
        CCLOG("이동위치(%d, %d)", endedColNumber, endedRowNumber);
        
        if(endedColNumber!=startColNumber || endedRowNumber!=startRowNumber) {
            if(endedColNumber==startColNumber && abs(endedRowNumber-startRowNumber)==1) {
                slimeMove(&gameSet[startColNumber][startRowNumber],&gameSet[endedColNumber][endedRowNumber]);
            }else if(endedRowNumber==startRowNumber && abs(endedColNumber-startColNumber)==1) {
                slimeMove(&gameSet[startColNumber][startRowNumber],&gameSet[endedColNumber][endedRowNumber]);
            }//이동위치 차이가 1일경우 바로 옆칸을 의미
        }
    }
}
/*
 onTouchEnded - 터치 종료시 콜백함수
 객체의 색을 섞어주고 이동시켜준다.
 */

void GameMain::onToucheMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
}


int GameMain::slimeColorMix(int inputColor1, int inputColor2)
{
    CCLOG("%d 와 %d를 섞습니다.", inputColor1, inputColor2);
    if(inputColor1<5 && inputColor2<5 && inputColor1!=3 && inputColor2!=3){
        return inputColor1 + inputColor2;
    }else{
        if((inputColor1==3 && inputColor2==4) || (inputColor1==4 && inputColor2==3) ||
           (inputColor1==5 && inputColor2==2) || (inputColor1==2 && inputColor2==5) ||
           (inputColor1==6 && inputColor2==1) || (inputColor1==1 && inputColor2==6))
        {
            return 7;
        }
    }
    
    return 0;
}
/*
 int slimeColorMix(int inputColor1, int inputColor2)
 두개의 색상 코드를 입력받아 형식에따라 섞는다.
 색섞기가 불가능하면 0을 리턴해준다.
 
 참고 슬라임 색상코드)
 0 - 투명
 1 - 빨강
 2 - 파랑
 3 - 보라(빨강+파랑)
 4 - 노랑
 5 - 주황(빨강+노랑)
 6 - 초록(파랑+노랑)
*/

void GameMain::slimeMove(SlimeBox* moveSlime, SlimeBox* targetSlime)
{
    int moveSlimeColor   = moveSlime->getSlimeColor();
    int targetSlimeColor = targetSlime->getSlimeColor();
    int mixColor =  slimeColorMix(moveSlimeColor, targetSlimeColor);
    
    if(mixColor){
        Sprite* moveSlimeMotion = Sprite::createWithTexture(moveSlime->slimeSprite->getTexture());
        CallFuncN* removeFunc   = CallFuncN::create(CC_CALLBACK_1(GameMain::autoRemoveSprite, this));
        MoveTo* slimeMoveAction = MoveTo::create(0.2, targetSlime->boxSprite->getPosition());
        
        addChild(moveSlimeMotion);
        moveSlimeMotion->setPosition(moveSlime->boxSprite->getPosition());
        moveSlimeMotion->runAction(Sequence::create(slimeMoveAction, removeFunc, NULL));
        
        targetSlime->setSlimeColor(mixColor);
        moveSlime->setSlimeColor(0);
        setSlimeSprite(targetSlime);
        setSlimeSprite(moveSlime);
        if(mixColor==7) getTheScore();
    }
}
/*
 void slimeMove(SlimeBox* moveSlime, SlimeBox* targetSlime)
 슬라임을 섞는다.
 moveSlime   - 이동하는 슬라임
 targetSlime - 섞여지는 슬라임
 */

void GameMain::gravity(int targetColNumber, int targetRowNumber)
{
    int i;
    Vec2 emptySlimePosition, targetSlimePosition;
    for (i=targetColNumber; i>0; i--) {
        CCLOG("위치이동 (%d, %d) -> (%d, %d)", i, targetRowNumber, i-1, targetRowNumber);
        targetSlimePosition = gameSet[i-1][targetRowNumber].boxSprite->getPosition();
        emptySlimePosition  = gameSet[i][targetRowNumber].boxSprite->getPosition();
        gameSet[i][targetRowNumber].setSlimeColor(gameSet[i-1][targetRowNumber].getSlimeColor());
        setSlimeSprite(&gameSet[i][targetRowNumber]);
        gameSet[i][targetRowNumber].slimeSprite->setPosition(targetSlimePosition);
        gameSet[i][targetRowNumber].slimeSprite->runAction(MoveTo::create(0.2, emptySlimePosition));
    }//맨위를 제외하고 아래로 내림
    
    emptySlimePosition = gameSet[0][targetRowNumber].boxSprite->getPosition();
    
    int setColorNumber = 0;
    while(setColorNumber==3 || setColorNumber==0) setColorNumber = (rand()%4)+1;
    
    gameSet[0][targetRowNumber].setSlimeColor(setColorNumber);
    setSlimeSprite(&gameSet[0][targetRowNumber]);
    gameSet[0][targetRowNumber].slimeSprite->setPosition(Point(emptySlimePosition.x, emptySlimePosition.y+boxHeight));
    gameSet[0][targetRowNumber].slimeSprite->runAction(MoveTo::create(0.2, Point(emptySlimePosition)));
    
    CCLOG("위치이동 끝");
    
    /*
     issue(2014/10/01) - 상하 슬라임 이동후 검은색 슬라임이 되어 사라지고 중력작용을 적용시키면 i-1이 빈곳이 되어 결국 한칸이 비어버리는 사태가 발생했다.
    */
    
}//슬라임 중력작용(위의 슬라임을 아래로 이동)




