#ifndef ChapterLayer_h__
#define ChapterLayer_h__
#include "cocos2d.h"
#include "DataController.h"
#include "LevelsLayer.h"
#include "MenuLayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class ChapterLayer :public Layer
{
public:
	ChapterLayer();
	~ChapterLayer();
	virtual bool init();
	CREATE_FUNC(ChapterLayer);
	static Scene* createScene();
};

#endif // ChapterLayer_h__