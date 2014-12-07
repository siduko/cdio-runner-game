#ifndef MenuLayer_h__
#define MenuLayer_h__
#include "cocos2d.h"
#include "ChapterLayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class MenuLayer:public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	CREATE_FUNC(MenuLayer);
};

#endif // MenuLayer_h__
