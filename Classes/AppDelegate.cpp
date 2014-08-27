#include "AppDelegate.h"
#include "GameMain.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    if(!glview) {
        glview = GLView::create("mixSlime");
        director->setOpenGLView(glview);
    }
    
    glview->setDesignResolutionSize(1080, 1920, ResolutionPolicy::NO_BORDER);
    
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);
    
    // create a scene. it's an autorelease object
    auto firstscene = GameMain::createScene();

    // run
    director->runWithScene(firstscene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
