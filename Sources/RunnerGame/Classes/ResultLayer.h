#ifndef ResultLayer_h__
#define ResultLayer_h__
#include "cocos2d.h"
#include "PlayLayer.h"
#include "LevelsLayer.h"
#include "MenuLayer.h"
#include "MenuLayer.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

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