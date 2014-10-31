#include "resultLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

ResultLayer::ResultLayer()
{
    
}

ResultLayer::~ResultLayer()
{
    
}

SEL_MenuHandler ResultLayer::onResolveCCBCCMenuItemSelector(Ref * pTarget, const char * pSelectorName)
{
    CCLOG("%s", pSelectorName);
    return NULL;
}

Control::Handler ResultLayer::onResolveCCBCCControlSelector(Ref * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool ResultLayer::onAssignCCBMemberVariable(cocos2d::Ref * pTarget, const char * pMemberVariableName, cocos2d::Node * node)
{
    return false;
}

bool ResultLayer::onAssignCCBCustomProperty(Ref* pTarget, const char* pMemberVariableName, const cocos2d::Value& pCCBValue)
{
    return false;
}

void ResultLayer::onNodeLoaded(cocos2d::Node * node, cocosbuilder::NodeLoader * nodeLoader)
{
}

