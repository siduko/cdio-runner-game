#include "AppDelegate.h"
#include "MenuLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Egg Catcher");
        director->setOpenGLView(glview);
    }

	Size designSize = Size(800, 480);
	Size screenSize = glview->getFrameSize();
	glview->setDesignResolutionSize(designSize.width, designSize.height,ResolutionPolicy::SHOW_ALL);

	//director->setContentScaleFactor(screenSize.width / designSize.width);
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audios/background.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Audios/background.mp3", true);

    // create a scene. it's an autorelease object
    //auto scene = PlayLayer::createScene("Levels/level1.tmx");
	auto scene = MenuLayer::createScene();
    // run
    director->runWithScene(scene);

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
