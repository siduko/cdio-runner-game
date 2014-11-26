#ifndef ResultLayer_h__
#define ResultLayer_h__
#include "cocos2d.h"
#include "DataController.h"
#include "PlayLayer.h"
#include "MenuLayer.h"
#include "ChapterLayer.h"
#include "LevelsLayer.h"


#include"LoadingLayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
using namespace std;

class ResultLayer :public Layer
{
public:
	Label *lbLevelInfo, *lbScore, *lbStar;
	static Scene* createScene();
	virtual bool init();
	ResultLayer();
	~ResultLayer();
	CREATE_FUNC(ResultLayer);
};

#endif // GameoverLayer_h__