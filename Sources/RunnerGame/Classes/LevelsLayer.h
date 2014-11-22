#ifndef LevelsLayer_h__
#define LevelsLayer_h__
#include "cocos2d.h"
#include "PlayLayer.h"
#include "ChapterLayer.h"
#include "Utils.h"
#include "ui/CocosGUI.h"
#include "DataController.h"
USING_NS_CC;
using namespace ui;

namespace Layers{
class LevelsLayer:public Layer
{
public:
	LevelsLayer();
	~LevelsLayer();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LevelsLayer);
};
}
#endif // LevelsLayer_h__

